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
    TimedQueue<bool> J_Probs;
    TimedQueue<bool> Z_Probs;
    std::string J = "J";
    std::string Z = "Z";
    std::vector<std::string> DATA_MAP;
    std::vector<TimedQueue<bool>> probs = {J_Probs, Z_Probs};
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
    DATA_MAP.push_back(J);
    DATA_MAP.push_back(Z);
    const auto& options = cc->Options<::mediapipe::DynamicTfliteTensorsToCharacterCalculatorOptions>();
    unknown_threshold = options.unknown_threshold();
    memory_length = options.memory_length();
    for(int i = 0; i < probs.size(); i++){
      probs.at(i) = TimedQueue<bool>(memory_length);
      // probs.at(i).add(true);
    }
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
      int highest_confidence_index = 0;
      for(int i = 0; i < output_floats->size(); i++){
        if(output_floats->at(i) > highest_confidence_index){
          highest_confidence = output_floats->at(i);
          highest_confidence_index = i;
        }
      }
      probs.at(highest_confidence_index).add(true);

      int most_common_index = 0;
      int most_common_count = 0;
      int total_count = 0;
      for(int i = 0; i < DATA_MAP.size(); i++){
        int current_size = probs.at(i).get().size();
        if(current_size > most_common_count){
          most_common_count = current_size;
          total_count += current_size;
          most_common_index = i;
        }
      }
      float certainty = (float)most_common_count / (float)total_count;
      std::string signn;
      if(certainty >= unknown_threshold){
        signn = DATA_MAP[most_common_index];
      }else{
        signn = "Unknown";
      }

    LOG(INFO) << DATA_MAP[most_common_index] << ": " << certainty << "% count: " << most_common_count;



    // float highest_confidence = 0;
    // int highest_confidence_index = -1;
    // if(output_floats->size() != DATA_MAP.size()){
    //   LOG(INFO) << output_floats->size() << " OUTPUT FLOATS BUT " << DATA_MAP.size() << " IN DATA MAP";
    // }
    // for(int i = 0; i < output_floats->size(); i++){
    //   if(output_floats->at(i) > highest_confidence){
    //     highest_confidence = output_floats->at(i);
    //     highest_confidence_index = i;
    //   }
    // }
    // probs.at(highest_confidence_index).add(true);
    
    // int most_frames = 0;
    // int most_frames_index = 0;
    // int total_frames = 0;


    // for(int i = 0; i < probs.size(); i++){
    //   if(probs.at(i).get().size() > most_frames){
    //     most_frames = probs.at(i).get().size();
    //     most_frames_index = i;
    //   }
    //   total_frames += probs.at(i).get().size();
    // }
    // if(total_frames == 0){
    //   total_frames = 1;
    // }


    // std::string signn;
    // if(most_frames / total_frames >= unknown_threshold){
    //   signn = DATA_MAP[most_frames_index];
    // }else{
    //   signn = "Unknown";
    // }
    // LOG(INFO) << signn << ", " <<  most_frames / total_frames;

    std::unique_ptr<std::string> output_stream_collection = std::make_unique<std::string>(signn); 
    cc -> Outputs().Tag(STRING).Add(output_stream_collection.release(), cc->InputTimestamp());

    return ::mediapipe::OkStatus();
  }
  
}  // namespace mediapipe
