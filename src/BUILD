package(default_visibility = ["//visibility:private"])

cc_library(
    name = "camera_parser",
    srcs = ["camera_parser.cc"],
    hdrs = ["camera_parser.h"],
    deps = [
        ":matrix_manager",
        ":matrix_parser",
        ":pointer_types",
        ":tokenizer",
        "//src/cameras:parser",
        "//src/color_integrators:parser",
        "//src/films:parser",
        "//src/integrators:parser",
        "//src/randoms:parser",
        "//src/samplers:parser",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "directive_parser",
    hdrs = ["directive_parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":matrix_manager",
        ":quoted_string",
        ":tokenizer",
    ],
)

cc_library(
    name = "finite_float_t",
    srcs = ["finite_float_t.cc"],
    hdrs = ["finite_float_t.h"],
    deps = [
        "@com_github_bradleymarie_iris//iris",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
    ],
)

cc_binary(
    name = "iris",
    srcs = ["iris.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":camera_parser",
        ":ostream",
        ":scene_parser",
        ":tokenizer",
        "@com_github_bradleymarie_iris//iris_physx",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:sample_tracer",
        "@com_google_absl//absl/flags:config",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_library(
    name = "matrix_manager",
    srcs = ["matrix_manager.cc"],
    hdrs = ["matrix_manager.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":finite_float_t",
        ":pointer_types",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "matrix_parser",
    srcs = ["matrix_parser.cc"],
    hdrs = ["matrix_parser.h"],
    deps = [
        ":matrix_manager",
        ":quoted_string",
        ":tokenizer",
    ],
)

cc_library(
    name = "ostream",
    srcs = ["ostream.cc"],
    hdrs = ["ostream.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        "@com_github_bradleymarie_iris//iris",
    ],
)

cc_library(
    name = "param_parser",
    srcs = ["param_parser.cc"],
    hdrs = ["param_parser.h"],
    deps = [
        ":pointer_types",
        ":quoted_string",
        ":tokenizer",
        "@com_github_bradleymarie_iris//iris_physx",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:interpolated_spectrum",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/types:variant",
    ],
)

cc_library(
    name = "param_matcher",
    srcs = ["param_matcher.cc"],
    hdrs = ["param_matcher.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":param_parser",
    ],
)

cc_library(
    name = "pointer_types",
    hdrs = ["pointer_types.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":shared_ptr",
        ":unique_ptr",
        "@com_github_bradleymarie_iris//iris_camera",
        "@com_github_bradleymarie_iris//iris_physx",
    ],
)

cc_library(
    name = "quoted_string",
    srcs = ["quoted_string.cc"],
    hdrs = ["quoted_string.h"],
    deps = [
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
    ],
)

cc_library(
    name = "scene_parser",
    srcs = ["scene_parser.cc"],
    hdrs = ["scene_parser.h"],
    deps = [
        ":matrix_manager",
        ":matrix_parser",
        ":pointer_types",
        ":tokenizer",
        "//src/area_lights:parser",
        "//src/materials:parser",
        "//src/shapes:parser",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:kd_tree_scene",
    ],
)

cc_library(
    name = "shared_ptr",
    hdrs = ["shared_ptr.h"],
)

cc_library(
    name = "tokenizer",
    srcs = ["tokenizer.cc"],
    hdrs = ["tokenizer.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
    ],
)

cc_library(
    name = "unique_ptr",
    hdrs = ["unique_ptr.h"],
)
