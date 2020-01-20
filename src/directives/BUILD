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
        "//src/color_extrapolators:parser",
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
        ":include",
        ":scene_builder",
        ":transform",
        "//src/area_lights:parser",
        "//src/common:error",
        "//src/common:material_factory",
        "//src/common:matrix_manager",
        "//src/common:named_material_manager",
        "//src/common:named_texture_manager",
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
    name = "parse",
    srcs = ["parse.cc"],
    hdrs = ["parse.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":geometry",
        ":global",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
        "//src/films/output_writers:result",
    ],
)

cc_library(
    name = "scene_builder",
    srcs = ["scene_builder.cc"],
    hdrs = ["scene_builder.h"],
    deps = [
        "//src/common:error",
        "//src/common:pointer_types",
        "//src/common:quoted_string",
        "//src/common:tokenizer",
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
