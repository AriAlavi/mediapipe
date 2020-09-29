#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/calculators/signn/timed_queue.h"
#include "mediapipe/framework/formats/landmark.pb.h"

#include "mediapipe/calculators/signn/static_dynamic_gate_options.pb.h"

#include <fstream>

namespace mediapipe{

    namespace{
        constexpr char Landmarks[] = "LANDMARKS";
        constexpr char LandmarksHistory[] = "LANDMARKS_HISTORY";
        constexpr char Double[] = "DOUBLE";
    }

    class StaticDynamicGateCalculator: public CalculatorBase {
        public:
        StaticDynamicGateCalculator(){};
        ~StaticDynamicGateCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(Landmarks).Set<NormalizedLandmarkList>();
            cc->Inputs().Tag(LandmarksHistory).Set<std::vector<NormalizedLandmarkList>>();
            cc->Inputs().Tag(Double).Set<double>();
            cc->Outputs().Tag(Landmarks).Set<NormalizedLandmarkList>();
            cc->Outputs().Tag(LandmarksHistory).Set<std::vector<NormalizedLandmarkList>>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            const auto& options = cc->Options<::mediapipe::StaticDynamicGateOptions>();
            DYNAMIC_THRESHOLD = options.dynamic_threshold();
            MAXIMUM_EXTRA_DYNAMIC_FRAMES = options.maximum_extra_dynamic_frames();
            double velocity_history = options.velocity_history();
            history = TimedQueue<double>(velocity_history);
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            auto static_data = cc->Inputs().Tag(Landmarks).Get<NormalizedLandmarkList>();
            auto dynamic_data = cc->Inputs().Tag(LandmarksHistory).Get<std::vector<NormalizedLandmarkList>>();
            double hand_velocity = cc->Inputs().Tag(Double).Get<double>();
            if(first_ten_frames_static > 0){
                first_ten_frames_static--;
                hand_velocity = 0;
            }
            history.add(hand_velocity);
            auto hand_history = history.get();
            double average_hand_history = 0;
            for(int i = 0; i < hand_history.size(); i++){
                if(hand_history.at(i) < 100){ // To prevent inf from being added - I am not sure how inf gets into the systen
                    average_hand_history += hand_history.at(i);
                }
                
            }
            average_hand_history /= hand_history.size();
            // LOG(INFO) << average_hand_history;
            if(average_hand_history < DYNAMIC_THRESHOLD && extra_dynamic_frames < 0){
                auto output_data = absl::make_unique<NormalizedLandmarkList>(static_data);
                cc->Outputs().Tag(Landmarks).Add(output_data.release(), cc->InputTimestamp());
            }else{
                if(average_hand_history >= DYNAMIC_THRESHOLD && extra_dynamic_frames < 10){
                    extra_dynamic_frames += 1;
                }else{
                    extra_dynamic_frames -= 1;
                }
                auto output_data = absl::make_unique<std::vector<NormalizedLandmarkList>>(dynamic_data);
                cc->Outputs().Tag(LandmarksHistory).Add(output_data.release(), cc->InputTimestamp());
            }
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:
        TimedQueue<double> history;
        double extra_dynamic_frames = 0;
        int MAXIMUM_EXTRA_DYNAMIC_FRAMES = 0;
        int first_ten_frames_static = 10;
        double DYNAMIC_THRESHOLD;

    };
    REGISTER_CALCULATOR(StaticDynamicGateCalculator);

}