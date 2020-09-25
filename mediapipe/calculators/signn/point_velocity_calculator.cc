#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/formats/detection.pb.h"
#include "mediapipe/framework/port/ret_check.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>


namespace mediapipe{

    namespace{
        constexpr char NormalizedLandmarks[] = "LANDMARKS";
        constexpr char DOUBLE[] = "DOUBLE";
    }

    class PointVelocityCalculator : public CalculatorBase {
        public:
        PointVelocityCalculator(){};
        ~PointVelocityCalculator(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(NormalizedLandmarks).Set<std::vector<NormalizedLandmarkList>>();
            cc->Inputs().Tag(DOUBLE).Set<double>();
            cc->Outputs().Tag(DOUBLE).Set<double>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            for(int i = 0; i <= 63; i++){
                hand_history.push_back(0);
            }
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            std::vector<NormalizedLandmarkList> hands = cc->Inputs().Tag(NormalizedLandmarks).Get<std::vector<NormalizedLandmarkList>>();
            double fps = cc->Inputs().Tag(DOUBLE).Get<double>();
            double time_since_last_frame = 1/fps;

            if(hands.size() == 0){
                return ::mediapipe::OkStatus();
            }
            auto hand = hands.at(0);
            double distance = 0;
            for(int i = 0; i < hand.landmark_size(); i++){
                distance += abs(hand.landmark(i).x() - hand_history.at(i * 3));
                distance += abs(hand.landmark(i).y() - hand_history.at(i * 3 + 1));
                distance += abs(hand.landmark(i).z() - hand_history.at(i * 3 + 2));
                hand_history.at(i * 3) = hand.landmark(i).x();
                hand_history.at(i * 3 + 1) = hand.landmark(i).y();
                hand_history.at(i * 3 + 2) = hand.landmark(i).z();
            }
            distance /= fps;

            std::unique_ptr<double> output_stream_collection = std::make_unique<double>(distance); 
            cc -> Outputs().Tag(DOUBLE).Add(output_stream_collection.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        private:
        std::vector<double> hand_history;


    };
    REGISTER_CALCULATOR(PointVelocityCalculator);

    class PointVelocityCalculatorOneHand : public CalculatorBase {
        public:
        PointVelocityCalculatorOneHand(){};
        ~PointVelocityCalculatorOneHand(){};

        static ::mediapipe::Status GetContract(CalculatorContract* cc){
            cc->Inputs().Tag(NormalizedLandmarks).Set<NormalizedLandmarkList>();
            cc->Inputs().Tag(DOUBLE).Set<double>();
            cc->Outputs().Tag(DOUBLE).Set<double>();
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Open(CalculatorContext* cc){
            for(int i = 0; i <= 42; i++){
                hand_history.push_back(0);
            }
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Process(CalculatorContext* cc){
            NormalizedLandmarkList hand = cc->Inputs().Tag(NormalizedLandmarks).Get<NormalizedLandmarkList>();
            double fps = cc->Inputs().Tag(DOUBLE).Get<double>();
            double time_since_last_frame = 1/fps;

            double distance = 0;
            for(int i = 0; i < hand.landmark_size(); i++){
                distance += abs(hand.landmark(i).x() - hand_history.at(i * 2));
                distance += abs(hand.landmark(i).y() - hand_history.at(i * 2 + 1));
                hand_history.at(i * 2) = hand.landmark(i).x();
                hand_history.at(i * 2 + 1) = hand.landmark(i).y();
            }
            distance /= fps;
            LOG(INFO) << "DISTANCE: " << distance;

            std::unique_ptr<double> output_stream_collection = std::make_unique<double>(distance); 
            cc -> Outputs().Tag(DOUBLE).Add(output_stream_collection.release(), cc->InputTimestamp());
            return ::mediapipe::OkStatus();
        }
        ::mediapipe::Status Close(CalculatorContext* cc){
            return ::mediapipe::OkStatus();
        }
        private:
        std::vector<double> hand_history;


    };
    REGISTER_CALCULATOR(PointVelocityCalculatorOneHand);
}