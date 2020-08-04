load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "global",
    srcs = ["global.cc"],
    hdrs = ["global.h"],
    deps = [
        ":include",
        ":transform",
        "//src/cameras:parser",
        "//src/color_integrators:parser",
        "//src/common:matrix_manager",
        "//src/common:parameters",
        "//src/common:pointer_types",
        "//src/common:quoted_string",
        "//src/common:tokenizer",
        "//src/films:parser",
        "//src/integrators:parser",
        "//src/light_propagation:parser",
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
        ":include",
        ":named_material_manager",
        ":scene_builder",
        ":transform",
        "//src/area_lights:parser",
        "//src/common:error",
        "//src/common:matrix_manager",
        "//src/common:named_texture_manager",
        "//src/common:normal_map_manager",
        "//src/common:pointer_types",
        "//src/common:texture_manager",
        "//src/common:tokenizer",
        "//src/lights:parser",
        "//src/materials:parser",
        "//src/shapes:parser",
        "//src/textures:parser",
    ],
)

cc_library(
    name = "include",
    srcs = ["include.cc"],
    hdrs = ["include.h"],
    deps = [
        "//src/common:quoted_string",
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "named_material_manager",
    srcs = ["named_material_manager.cc"],
    hdrs = ["named_material_manager.h"],
    deps = [
        "//src/common:pointer_types",
        "//src/materials:result",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "parse",
    srcs = ["parse.cc"],
    hdrs = ["parse.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":geometry",
        ":global",
        ":parser",
        "//src/common:pointer_types",
        "//src/films/output_writers:result",
    ],
)

cc_library(
    name = "parser",
    hdrs = ["parser.h"],
    srcs = ["parser.cc"],
    visibility = ["//src:__subpackages__"],
    deps = [
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "scene_builder",
    srcs = ["scene_builder.cc"],
    hdrs = ["scene_builder.h"],
    deps = [
        "//src/common:error",
        "//src/common:directive",
        "//src/common:pointer_types",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:kd_tree_scene",
        "@com_google_absl//absl/container:flat_hash_map",
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
