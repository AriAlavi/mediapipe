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
    TimedQueue<int> last_probs;
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
    last_probs = TimedQueue<int>(memory_length);
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

    std::string signn = "Testing";

    int MINIMUM_SAMPLE_SIZE = 10;
    float probability = 0;
    float j_sum = 0;
    float z_sum = 0;

    if(output_floats->at(0) > output_floats->at(1)){
      last_probs.add(0);
    }else{
      last_probs.add(1);
    }
    std::vector<int> last_values = last_probs.get();
    for(int i = 0; i < last_values.size(); i++){
      if(last_values.at(0) == 0){
        j_sum++;
      }else{
        z_sum++;
      }
    }
    int sample_size = last_values.size();
    if(sample_size > MINIMUM_SAMPLE_SIZE){
      if(j_sum > z_sum){
        probability = j_sum / sample_size;
        signn = "J";
      }else{
        probability = z_sum / sample_size;
        signn = "Z";
      }
      if(probability < unknown_threshold){
        signn = "Unknown";
      }
    }else{
      signn = "Unknown";
    }




    LOG(INFO) << "J: " << j_sum << " Z: " << z_sum << " PROB: " << probability << "% SIZE:" << sample_size; 

      


    std::unique_ptr<std::string> output_stream_collection = std::make_unique<std::string>(signn); 
    cc -> Outputs().Tag(STRING).Add(output_stream_collection.release(), cc->InputTimestamp());

    return ::mediapipe::OkStatus();
  }
  
}  // namespace mediapipe
