from fractions import Fraction
def sublistRegulation(input, output_frame_count) -> list:
      assert isinstance(input, list)
      assert isinstance(output_frame_count, int)
      assert all(isinstance(x, float) or isinstance(x, int) for x in input)

      slopes = []
      offsets = []
      i = 0
      for _ in input:
        offsets.append(input[i])

        if i == len(input) - 1:
          slopes.append(0)
        else:
          slopes.append(input[i+1] - input[i])

        i += 1

      iterpolated_list = []
      step_count = Fraction(len(input)-1, output_frame_count-1)
      pointer = 0
      while pointer <= len(input)-1:
        current_index = int(pointer // 1)
        current_value = slopes[current_index] * (pointer-current_index) + offsets[current_index]
        iterpolated_list.append(current_value)
        pointer += step_count

      return iterpolated_list

def regulation(input_frames, output_frame_count) -> list:
  assert isinstance(input_frames, list)
  assert isinstance(output_frame_count, int)
  coordinates_per_frame = len(input_frames[0])
  assert all(len(x) == coordinates_per_frame for x in input_frames)
  master_list = []
  slices = []

  for s in range(coordinates_per_frame):
    sliced = []
    for i in range(0, len(input_frames)):
      sliced.append(input_frames[i][s])
    slices.append(sublistRegulation(sliced, output_frame_count))

  for j in range(output_frame_count):
    newFrames = []
    for t in range(coordinates_per_frame):
      newFrames.append(slices[t][j])
    master_list.append(newFrames)

  return master_list

print("OUTPUT: ", regulation([(1,2),(2,3),(3,4),(4,5),(5,6),(6,7),(7,8)],5))
