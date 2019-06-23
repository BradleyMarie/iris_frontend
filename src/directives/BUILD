package(default_visibility = ["//visibility:private"])

cc_library(
    name = "global",
    srcs = ["global.cc"],
    hdrs = ["global.h"],
    deps = [
        ":transform",
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
    name = "geometry",
    srcs = ["geometry.cc"],
    hdrs = ["geometry.h"],
    deps = [
        ":transform",
        "//src/area_lights:parser",
        "//src/common:matrix_manager",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
        "//src/materials:parser",
        "//src/shapes:parser",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:kd_tree_scene",
    ],
)

cc_library(
    name = "parse",
    srcs = ["parse.cc"],
    hdrs = ["parse.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":global",
        ":geometry",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
        "//src/films/output_writers:result",
    ],
)

cc_library(
    name = "transform",
    srcs = ["transform.cc"],
    hdrs = ["transform.h"],
    deps = [
        "//src/common:matrix_manager",
        "//src/common:quoted_string",
        "//src/common:tokenizer",
    ],
)
