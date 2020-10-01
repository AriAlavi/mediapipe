#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"

#include "mediapipe/calculators/signn/timed_queue.h"

namespace mediapipe{

    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
        constexpr char SIGNAL[] = "SIGNAL";
    }


    class LeftHandGateCalculator : public CalculatorBase {
        public:
        LeftHandGateCalculator(){};
        ~LeftHandGateCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(NormalizedLandmarks).Set<NormalizedLandmarkList>();
            cc->Outputs().Tag(NormalizedLandmarks).Set<NormalizedLandmarkList>();
            cc->Outputs().Tag(SIGNAL).Set<bool>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            first_x_seconds = TimedQueue<int>(3);
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            NormalizedLandmarkList hands = cc->Inputs().Tag(NormalizedLandmarks).Get<NormalizedLandmarkList>();
            if(done){
                std::unique_ptr<NormalizedLandmarkList> output_stream_collection = std::make_unique<NormalizedLandmarkList>(hands);
                cc -> Outputs().Tag(NormalizedLandmarks).Add(output_stream_collection.release(), cc->InputTimestamp());
                return ::mediapipe::OkStatus();
            }
            else if(already_added){
                auto got = first_x_seconds.get();
                if(got.size() == 0){
                    done = true;
                }
            }else{
                first_x_seconds.add(0);
                already_added = true;
            }
            std::unique_ptr<bool> output_stream_collection = std::make_unique<bool>(true); 
            cc -> Outputs().Tag(SIGNAL).Add(output_stream_collection.release(), cc->InputTimestamp()); 
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }

        private:
        TimedQueue<int> first_x_seconds;
        bool already_added = false;
        bool done = false;
    };
    REGISTER_CALCULATOR(LeftHandGateCalculator);
}