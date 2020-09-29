#include "mediapipe/framework/calculator_framework.h"
#include "tensorflow/lite/interpreter.h"
#include "mediapipe/calculators/signn/dynamic_tflite_tensors_to_character_calculator.pb.h"
#include "mediapipe/calculators/signn/timed_queue.h"
#include <vector>
namespace mediapipe {
    constexpr char STRING[] = "STRING";

  class DynamicTfLiteTensorsToCharacterCalculator : public CalculatorBase {
  public:
    static ::mediapipe::Status GetContract(CalculatorContract* cc);

    ::mediapipe::Status Open(CalculatorContext* cc) override;

    ::mediapipe::Status Process(CalculatorContext* cc) override;
  private:
    float unknown_threshold = 0;
    float memory_length = 2;
    float last_character_bias = .001;
    int last_character = -1;
    TimedQueue<bool> J_probs;
    TimedQueue<bool> Z_probs;
  };

  REGISTER_CALCULATOR(DynamicTfLiteTensorsToCharacterCalculator);

  ::mediapipe::Status DynamicTfLiteTensorsToCharacterCalculator::GetContract(
      CalculatorContract* cc) {
    cc->Inputs().Tag("TENSORS").Set<std::vector<TfLiteTensor>>();
    cc->Outputs().Tag(STRING).Set<std::string>();
    return ::mediapipe::OkStatus();
  }

  ::mediapipe::Status DynamicTfLiteTensorsToCharacterCalculator::Open(
      CalculatorContext* cc) {
    const auto& options = cc->Options<::mediapipe::DynamicTfliteTensorsToCharacterCalculatorOptions>();
    // unknown_threshold = options.unknown_threshold();
    // last_character_bias = options.memo();
    unknown_threshold = options.unknown_threshold();
    memory_length = options.memory_length();
    J_probs = TimedQueue<bool>(memory_length);
    Z_probs = TimedQueue<bool>(memory_length);
    return ::mediapipe::OkStatus();
  }

  ::mediapipe::Status DynamicTfLiteTensorsToCharacterCalculator::Process(
      CalculatorContext* cc) {
    const auto& input_tensors = cc->Inputs().Tag("TENSORS").Get<std::vector<TfLiteTensor>>();
    const TfLiteTensor* raw_tensor = &input_tensors[0];
    

    const float* raw_floats = raw_tensor->data.f;
    int num_values = 1;
    for (int i = 0; i < raw_tensor->dims->size; ++i) {
      RET_CHECK_GT(raw_tensor->dims->data[i], 0);
      num_values *= raw_tensor->dims->data[i];
    }

    auto output_floats = absl::make_unique<std::vector<float>>(
        raw_floats, raw_floats + num_values);
    
    if(output_floats->at(0) > output_floats->at(1)){
      J_probs.add(true);
    }else{
      Z_probs.add(true);
    }

    int J_size = J_probs.get().size();
    int Z_size = Z_probs.get().size();
    int higher_size = 0;
    int total_size = J_size + Z_size;
    std::string signn;
    if(total_size == 0){
      total_size == 1;
    }
    float confidence = 0;
    if(J_size > Z_size){
      signn = "J";
      higher_size = J_size;
    }else{
      signn = "Z";
      higher_size = Z_size;
    }
    confidence = (float)higher_size / (float)total_size;
    if(confidence < unknown_threshold){
      signn = "Unknown";
    }
    LOG(INFO) << signn << ": " << confidence << "%";

    std::unique_ptr<std::string> output_stream_collection = std::make_unique<std::string>(signn); 
    cc -> Outputs().Tag(STRING).Add(output_stream_collection.release(), cc->InputTimestamp());

    return ::mediapipe::OkStatus();
  }
  
}  // namespace mediapipe
