# Copyright 2019 The MediaPipe Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

licenses(["notice"])  # Apache 2.0

package(default_visibility = ["//visibility:private"])

load("//mediapipe/framework/port:build_config.bzl", "mediapipe_cc_proto_library")


proto_library(
    name = "coordinate_logger_calculator_proto",
    srcs = ["coordinate_logger_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_proto",
    ],
)
proto_library(
    name = "average_double_calculator_proto",
    srcs = ["average_double_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_proto",
    ],
)
proto_library(
    name = "tflite_tensors_to_character_calculator_proto",
    srcs = ["tflite_tensors_to_character_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_proto",
    ],
)
proto_library(
    name = "dynamic_tflite_tensors_to_character_calculator_proto",
    srcs = ["dynamic_tflite_tensors_to_character_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_proto",
    ],
)
proto_library(
    name = "time_average_calculator_proto",
    srcs = ["time_average_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_proto",
    ],
)
proto_library(
    name = "hand_gate_calculator_proto",
    srcs = ["hand_gate_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_proto",
    ],
)
proto_library(
    name = "string_emitter_calculator_proto",
    srcs = ["string_emitter_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_proto",
    ],
)
proto_library(
    name = "landmark_history_calculator_proto",
    srcs = ["landmark_history_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_proto",
    ],
)
proto_library(
    name = "fps_gate_calculator_proto",
    srcs = ["fps_gate_calculator.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_proto",
    ],
)

proto_library(
    name = "static_dynamic_gate_options_proto",
    srcs = ["static_dynamic_gate_options.proto"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_proto",
    ],
)

mediapipe_cc_proto_library(
    name = "coordinate_logger_calculator_cc_proto",
    srcs = ["coordinate_logger_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":coordinate_logger_calculator_proto"],
)
mediapipe_cc_proto_library(
    name = "average_double_calculator_cc_proto",
    srcs = ["average_double_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":average_double_calculator_proto"],
)
mediapipe_cc_proto_library(
    name = "tflite_tensors_to_character_calculator_cc_proto",
    srcs = ["tflite_tensors_to_character_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":tflite_tensors_to_character_calculator_proto"],
)
mediapipe_cc_proto_library(
    name = "dynamic_tflite_tensors_to_character_calculator_cc_proto",
    srcs = ["dynamic_tflite_tensors_to_character_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":dynamic_tflite_tensors_to_character_calculator_proto"],
)
mediapipe_cc_proto_library(
    name = "time_average_calculator_cc_proto",
    srcs = ["time_average_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":time_average_calculator_proto"],
)
mediapipe_cc_proto_library(
    name = "hand_gate_calculator_cc_proto",
    srcs = ["hand_gate_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":hand_gate_calculator_proto"],
)
mediapipe_cc_proto_library(
    name = "string_emitter_calculator_cc_proto",
    srcs = ["string_emitter_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":string_emitter_calculator_proto"],
)
mediapipe_cc_proto_library(
    name = "landmark_history_calculator_cc_proto",
    srcs = ["landmark_history_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":landmark_history_calculator_proto"],
)
mediapipe_cc_proto_library(
    name = "fps_gate_calculator_cc_proto",
    srcs = ["fps_gate_calculator.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":fps_gate_calculator_proto"],
)
mediapipe_cc_proto_library(
    name = "static_dynamic_gate_options_cc_proto",
    srcs = ["static_dynamic_gate_options.proto"],
    cc_deps = ["//mediapipe/framework:calculator_cc_proto"],
    visibility = ["//visibility:public"],
    deps = [":static_dynamic_gate_options_proto"],
)



cc_library(
    name = "logger_calculator",
    srcs = ["coordinate_logger.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework/formats:location_data_cc_proto",
        "//mediapipe/framework/formats:detection_cc_proto",
        "//mediapipe/framework/deps:file_path",
        ":coordinate_logger_calculator_cc_proto"
    ] + select({
        "//conditions:default": [
            "//mediapipe/framework/port:file_helpers",
        ],
    }),
    alwayslink = 1,
)

cc_library(
    name = "console_logger_calculator",
    srcs = ["coordinate_console_logger.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework/formats:location_data_cc_proto",
        "//mediapipe/framework/formats:detection_cc_proto",
        "//mediapipe/framework/deps:file_path",    
    ],
    alwayslink = 1
)

cc_library(
    name = "coordinate_modifiers",
    srcs = ["coordinate_modifiers.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework/formats:location_data_cc_proto",
        "//mediapipe/framework/formats:detection_cc_proto",
    ],
    alwayslink = 1
)

cc_library(
    name = "z_score_calculator",
    srcs = ["z_score_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework/formats:location_data_cc_proto",
        "//mediapipe/framework/formats:detection_cc_proto",
    ],
    alwayslink = 1
)

cc_library(
    name = "fps_classes",
    srcs = ["fps_classes.h", "fps_classes.cc"],
    visibility = ["//visibility:public"],
    deps = [
    ],
    alwayslink = 1
)
cc_library(
    name = "double_logger",
    srcs = ["double_logger.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
    ],
    alwayslink = 1
)
cc_library(
    name = "fps_calculator",
    srcs = ["fps_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/calculators/signn:fps_classes",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework/formats:location_data_cc_proto",
        "//mediapipe/framework/formats:detection_cc_proto",
        "//mediapipe/framework/deps:file_path",    
    ],
    alwayslink = 1
)
cc_library(
    name = "average_double_calculator",
    srcs = ["average_double_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        ":average_double_calculator_cc_proto",
    ],
    alwayslink = 1
)

cc_library(
    name = "point_velocity_calculator",
    srcs = ["point_velocity_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/calculators/signn:fps_classes",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework/formats:location_data_cc_proto",
        "//mediapipe/framework/formats:detection_cc_proto",
        "//mediapipe/framework/deps:file_path",    
    ],
    alwayslink = 1
)

cc_library(
    name = "double_to_string_calculator",
    srcs = ["double_to_string_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
    ],
    alwayslink = 1
)

cc_library(
    name = "string_to_render_data_calculator",
    srcs = ["string_to_render_data_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_options_cc_proto",
        "//mediapipe/framework/formats:detection_cc_proto",
        "//mediapipe/framework/formats:location_data_cc_proto",
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/util:color_cc_proto",
        "//mediapipe/util:render_data_cc_proto",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
    ],
    alwayslink = 1
)

cc_library(
    name = "landmarks_to_tensor_calculator",
    srcs = ["landmarks_to_tensor_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:status",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework/formats:detection_cc_proto",
        "//mediapipe/framework/formats:matrix",
    ],
    alwayslink = 1,
)

cc_library(
    name = "tflite_tensors_to_character_calculator",
    srcs = ["tflite_tensors_to_character_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/framework:calculator_options_cc_proto",
        ":tflite_tensors_to_character_calculator_cc_proto",
        "@org_tensorflow//tensorflow/lite:framework",
    ],
    alwayslink = 1,
)

cc_library(
    name = "time_average_calculator",
    srcs = ["time_average_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_options_cc_proto",
        ":time_average_calculator_cc_proto",
        ":timed_queue",
    ],
    alwayslink = 1,
)

cc_library(
    name = "timed_queue",
    srcs = ["timed_queue.h", "timed_queue.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework:calculator_framework",
    ],
    alwayslink = 1,
)

cc_library(
    name = "hand_gate_calculator",
    srcs = ["hand_gate_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/calculators/signn:timed_queue",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_options_cc_proto",
        ":hand_gate_calculator_cc_proto",
    ],
    alwayslink = 1,
)


cc_library(
    name = "signn_end_gate_calculator",
    srcs = ["signn_end_gate_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_options_cc_proto",
        "//mediapipe/util:render_data_cc_proto",
    ],
    alwayslink = 1,
)

cc_library(
    name = "string_emitter_calculator",
    srcs = ["string_emitter_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_options_cc_proto",
        ":string_emitter_calculator_cc_proto",
    ],
    alwayslink = 1,
)

cc_library(
    name = "fps_gate_calculator",
    srcs = ["fps_gate_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_options_cc_proto",
        "//mediapipe/calculators/signn:timed_queue",
        "//mediapipe/framework/formats:landmark_cc_proto",
        ":fps_gate_calculator_cc_proto",
    ],
    alwayslink = 1,
)

cc_library(
    name = "landmark_history_calculator",
    srcs = ["landmark_history_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_options_cc_proto",
        ":timed_queue",
        "//mediapipe/framework/formats:landmark_cc_proto",
        ":landmark_history_calculator_cc_proto",
    ],
    alwayslink = 1,
)

cc_library(
    name = "regulation_calculator",
    srcs = ["regulation_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_options_cc_proto",
        ":timed_queue",
        "//mediapipe/framework/formats:landmark_cc_proto",
    ],
    alwayslink = 1,
)

cc_library(
    name = "landmarkhistory_to_tensor_calculator",
    srcs = ["landmarkhistory_to_tensor_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:status",
        "//mediapipe/framework/formats:landmark_cc_proto",
        "//mediapipe/framework/formats:detection_cc_proto",
        "//mediapipe/framework/formats:matrix",
    ],
    alwayslink = 1,
)

cc_library(
    name = "dynamic_tflite_tensors_to_character_calculator",
    srcs = ["dynamic_tflite_tensors_to_character_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/port:ret_check",
        "//mediapipe/framework:calculator_options_cc_proto",
        ":dynamic_tflite_tensors_to_character_calculator_cc_proto",
        "@org_tensorflow//tensorflow/lite:framework",
    ],
    alwayslink = 1,
)

cc_library(
    name = "static_dynamic_gate_calculator",
    srcs = ["static_dynamic_gate_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_options_cc_proto",
        "//mediapipe/framework/formats:landmark_cc_proto",
        ":timed_queue",
        ":static_dynamic_gate_options_cc_proto",
    ],
    alwayslink = 1,
)

cc_library(
    name = "zscore_per_video_calculator",
    srcs = ["zscore_per_video_calculator.cc"],
    visibility = ["//visibility:public"],
    deps = [
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework:calculator_options_cc_proto",
    ],
    alwayslink = 1,
)