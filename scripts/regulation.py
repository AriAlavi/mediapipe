from fractions import Fraction
def regulation_func(input, output_frame_count) -> list:
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
  print("SLOPES", slopes)
  print("OFFSETS", offsets)
  print("STEP_COUNT", step_count)
  while pointer <= len(input) - 1:
    print("CURRENT POINTER:", pointer)
    current_index = int(pointer // 1)
    current_value = slopes[current_index] * (pointer-current_index) + offsets[current_index]
    iterpolated_list.append(current_value)
    pointer += step_count

  return iterpolated_list

#print("OUTPUT:", regulation_func([1,2,3,4,5,6,7], 5))
#print("OUTPUT:", regulation_func([1, 5/2, 4, 11/2, 7], 7))

print("OUTPUT:", regulation_func([2], 7))
