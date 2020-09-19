#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/calculators/signn/timed_queue.h"
#include "mediapipe/framework/formats/landmark.pb.h"

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
            history = TimedQueue<double>(1);
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            auto static_data = cc->Inputs().Tag(Landmarks).Get<NormalizedLandmarkList>();
            auto dynamic_data = cc->Inputs().Tag(LandmarksHistory).Get<std::vector<NormalizedLandmarkList>>();
            double hand_velocity = cc->Inputs().Tag(Double).Get<double>();
            history.add(hand_velocity);
            auto hand_history = history.get();
            double average_hand_history = 0;
            for(int i = 0; i < hand_history.size(); i++){
                average_hand_history += hand_history.at(i);
            }
            average_hand_history /= hand_history.size();
            if(average_hand_history < .1){
                auto output_data = absl::make_unique<NormalizedLandmarkList>(static_data);
                cc->Outputs().Tag(Landmarks).Add(output_data.release(), cc->InputTimestamp());
            }else{
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

    };
    REGISTER_CALCULATOR(StaticDynamicGateCalculator);

}