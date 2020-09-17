#include <vector>
#include <string>

namespace mediapipe{

  namespace{
    constexpr char NormalizedLandmarks[] = "LANDMARKS";
  }

  class RegulationCalculator : public CalculatorBase{
    public:
    RegulationCalculator(){};
    ~RegulationCalculator(){};

    static ::mediapipe::Status GetContract(CalculatorContract* cc){
      cc->Inputs().Tag(NormalizedLandmarks).Set<std::vector<NormalizedLandmarkList>>();
      return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Open(CalculatorContext* cc){
      return ::mediapipe::OkStatus();
    }
    ::mediapipe::Status Process(CalculatorContext* cc){
      for(int i = 0; i < hands.size(); i++){
                auto hand = hands.at(i);
                for(int j = 0; j < hand.landmark_size(); j++){
                    std::cout << hand.landmark(j).x() << " " <<  hand.landmark(j).y() << " " << hand.landmark(j).z();
                    if(j + 1 < hand.landmark_size()){
                        std::cout << " ";
                    }
                }
            }
            std::cout << "\n";
            return ::mediapipe::OkStatus();
    }

    ::mediapipe::Status Close(CalculatorContext* cc){
      return ::mediapipe::OkStatus();
    }

  private:

  };
  REGISTER_CALCULATOR(Regulation);
}
