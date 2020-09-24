/*Rafael Trinidad worked on this script */
#include <vector>
#include <iostream>

using namespace std;

vector<double> sublistRegulation(vector<double> input, int output_frame_count){
  vector<double> slopes;
  vector<double> offsets;
  int input_len = input.size();
  int i = 0;
  for(int j = 0; j < input_len; j++){
    offsets.push_back(input[i]);

    if(i == input_len - 1)
      slopes.push_back(0);
    else
      slopes.push_back(input[i+1] - input[i]);

    i += 1;
  }

  vector<double> iterpolated_list;
  double step_count = double(input_len - 1)/double(output_frame_count - 1);
  double pointer = 0;
  while(pointer <= input_len - 1){
    int current_index = int(pointer)/1;
    double current_value = slopes[current_index] * double(pointer - current_index) + offsets[current_index];
    iterpolated_list.push_back(current_value);
    pointer += step_count;
  }

  return iterpolated_list;

}

vector<vector<double>> regulation(vector<vector<double>> input_frames, int output_frame_count){
  int coordinates_per_frame = input_frames[0].size();
  vector<vector<double>> master_list;
  vector<vector<double>> slices;

  for(int s = 0; s < coordinates_per_frame; s++){
    vector<double> sliced;
    for(int i = 0; i < input_frames.size(); i++)
      sliced.push_back(input_frames[i][s]);
    slices.push_back(sublistRegulation(sliced,output_frame_count));
  }
  for(int j = 0; j < output_frame_count; j++){
    vector<double> newFrames;
    for(int t = 0; t < coordinates_per_frame; t++){
      newFrames.push_back(slices[t][j]);
    }
    master_list.push_back(newFrames);
  }
  return master_list;
}

int main(){
  vector<double> one;
  one.push_back(1.0);
  one.push_back(2.0);

  vector<double> two;
  two.push_back(2.0);
  two.push_back(3.0);

  vector<double> three;
  three.push_back(3.0);
  three.push_back(4.0);

  vector<double> four;
  four.push_back(4.0);
  four.push_back(5.0);

  vector<double> five;
  five.push_back(5.0);
  five.push_back(6.0);

  vector<double> six;
  six.push_back(6.0);
  six.push_back(7.0);

  vector<double> seven;
  seven.push_back(7.0);
  seven.push_back(8.0);

  vector<vector<double>> vec;
  vec.push_back(one);
  vec.push_back(two);
  vec.push_back(three);
  vec.push_back(four);
  vec.push_back(five);
  vec.push_back(six);
  vec.push_back(seven);

  vector<vector<double>> regulation_vec = regulation(vec,5);

  return 1;
}
