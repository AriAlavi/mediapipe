#include "mediapipe/framework/calculator_framework.h"
#include "tensorflow/lite/interpreter.h"
#include "mediapipe/calculators/signn/dynamic_tflite_tensors_to_character_calculator.pb.h"

namespace mediapipe {
    constexpr char STRING[] = "STRING";

  class DynamicTfLiteTensorsToCharacterCalculator : public CalculatorBase {
  public:
    static ::mediapipe::Status GetContract(CalculatorContract* cc);

    ::mediapipe::Status Open(CalculatorContext* cc) override;

    ::mediapipe::Status Process(CalculatorContext* cc) override;
  private:
    float unknown_threshold = 0;
    float last_character_bias = .001;
    int last_character = -1;
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
    unknown_threshold = options.unknown_threshold();
    last_character_bias = options.last_character_bias();
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

    float highest_confidence = 0;
    int confidence_index = 0;
    if(last_character >= 0 && last_character_bias > 0){
      output_floats->at(last_character) += last_character_bias;
    }
    for(int i = 0; i < 2; i++){
        if(highest_confidence < output_floats->at(i)){
            highest_confidence = output_floats->at(i);
            confidence_index = i;
        }
    }
    const std::string DATA_MAP[2] = {"J", "Z"};
    std::string signn;
    if(highest_confidence >= unknown_threshold){
      signn = DATA_MAP[confidence_index];
      last_character = confidence_index;
    }else{
      signn = "Unknown";
    }
    LOG(INFO) << signn << ", " << highest_confidence;
    

    std::unique_ptr<std::string> output_stream_collection = std::make_unique<std::string>(signn); 
    cc -> Outputs().Tag(STRING).Add(output_stream_collection.release(), cc->InputTimestamp());

    return ::mediapipe::OkStatus();
  }
  
}  // namespace mediapipe
