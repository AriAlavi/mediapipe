#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/port/status.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/formats/matrix.h"

#include <vector>


namespace mediapipe{

    namespace{
        constexpr char LandmarksHistory[] = "DOUBLE_LANDMARKS_HISTORY";
        constexpr char LandmarkMatrix[] = "MATRIX";
    }

    class LandmarkHistoryToTensorCalculator : public CalculatorBase {
        public:
        LandmarkHistoryToTensorCalculator(){};
        ~LandmarkHistoryToTensorCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(LandmarksHistory).Set<std::vector<std::vector<double>>>();
            cc->Outputs().Tag(LandmarkMatrix).Set<Matrix>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){

            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            std::vector<std::vector<double>> frames = cc->Inputs().Tag(LandmarksHistory).Get<std::vector<std::vector<double>>>();
            Matrix bob;

            bob.resize(60, 42);
            for(int i = 0; i < frames.size(); i++){
                auto frame = frames.at(i);
                for(int j = 0; j < frame.size(); j++){
                    bob(i, j) = frame.at(j);
                }
            }


            // for(int i = 0; i < 21; i++){     
            //     bob(0, i*2) = hand.landmark(i).x();
            //     bob(0, i*2+1) = hand.landmark(i).y();
            // }
            
            std::unique_ptr<Matrix> output_stream_collection = std::make_unique<Matrix>(bob); 
            cc -> Outputs().Tag(LandmarkMatrix).Add(output_stream_collection.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:

    };
    
    REGISTER_CALCULATOR(LandmarkHistoryToTensorCalculator);
}