package(default_visibility = ["//visibility:private"])

cc_library(
    name = "camera_parser",
    hdrs = ["camera_parser.h"],
    srcs = ["camera_parser.cc"],
    deps = [
        ":matrix_manager",
        ":pointer_types",
        ":tokenizer",
        "@com_github_abseil_abseil_cpp//absl/strings",
    ],
)

cc_binary(
    name = "iris",
    srcs = ["iris.cc"],
    deps = [
        ":camera_parser",
        ":output_writer",
        ":scene_parser",
        ":tokenizer",
        "@com_github_bradleymarie_iris//iris_advanced_toolkit:pcg_random",
        "@com_github_bradleymarie_iris//iris_physx:iris_physx",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:sample_tracer",
        "@com_github_gflags_gflags//:gflags",
    ],
)

cc_library(
    name = "matrix_manager",
    hdrs = ["matrix_manager.h"],
    srcs = ["matrix_manager.cc"],
    deps = [
        ":pointer_types",
    ],
)

cc_library(
    name = "output_writer",
    hdrs = ["output_writer.h"],
    srcs = ["output_writer.cc"],
    deps = [
        ":pointer_types",
    ],
)

cc_library(
    name = "param_parser",
    hdrs = ["param_parser.h"],
    srcs = ["param_parser.cc"],
    deps = [
        ":pointer_types",
        ":tokenizer",
        ":quoted_string",
        "@com_github_abseil_abseil_cpp//absl/strings",
        "@com_github_abseil_abseil_cpp//absl/types:optional",
        "@com_github_abseil_abseil_cpp//absl/types:variant",
        "@com_github_bradleymarie_iris//iris_physx:iris_physx",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:interpolated_spectrum",
    ],
)

cc_library(
    name = "pointer_types",
    hdrs = ["pointer_types.h"],
    deps = [
        ":shared_ptr",
        ":unique_ptr",
        "@com_github_bradleymarie_iris//iris_camera:iris_camera",
        "@com_github_bradleymarie_iris//iris_physx:iris_physx",
    ],
)

cc_library(
    name = "quoted_string",
    hdrs = ["quoted_string.h"],
    srcs = ["quoted_string.cc"],
    deps = [
        "@com_github_abseil_abseil_cpp//absl/strings",
        "@com_github_abseil_abseil_cpp//absl/types:optional",
    ],
)

cc_library(
    name = "scene_parser",
    hdrs = ["scene_parser.h"],
    srcs = ["scene_parser.cc"],
    deps = [
        ":matrix_manager",
        ":pointer_types",
        ":tokenizer",
        "@com_github_abseil_abseil_cpp//absl/strings",
    ],
)

cc_library(
    name = "shared_ptr",
    hdrs = ["shared_ptr.h"],
)

cc_library(
    name = "tokenizer",
    hdrs = ["tokenizer.h"],
    srcs = ["tokenizer.cc"],
    deps = [
        "@com_github_abseil_abseil_cpp//absl/strings",
        "@com_github_abseil_abseil_cpp//absl/types:optional",
    ],
)

cc_library(
    name = "unique_ptr",
    hdrs = ["unique_ptr.h"],
)