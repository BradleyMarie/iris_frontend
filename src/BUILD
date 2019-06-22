package(default_visibility = ["//visibility:private"])

cc_library(
    name = "camera_parser",
    srcs = ["camera_parser.cc"],
    hdrs = ["camera_parser.h"],
    deps = [
        ":matrix_parser",
        "//src/cameras:parser",
        "//src/color_integrators:parser",
        "//src/common:matrix_manager",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
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
        "//src/common:matrix_manager",
        "//src/common:quoted_string",
        "//src/common:tokenizer",
    ],
)

cc_binary(
    name = "iris",
    srcs = ["iris.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":camera_parser",
        ":scene_parser",
        "//src/common:ostream",
        "//src/common:tokenizer",
        "@com_github_bradleymarie_iris//iris_physx",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:sample_tracer",
        "@com_google_absl//absl/flags:config",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_library(
    name = "matrix_parser",
    srcs = ["matrix_parser.cc"],
    hdrs = ["matrix_parser.h"],
    deps = [
        "//src/common:matrix_manager",
        "//src/common:quoted_string",
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "scene_parser",
    srcs = ["scene_parser.cc"],
    hdrs = ["scene_parser.h"],
    deps = [
        ":matrix_parser",
        "//src/area_lights:parser",
        "//src/common:matrix_manager",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
        "//src/materials:parser",
        "//src/shapes:parser",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:kd_tree_scene",
    ],
)
