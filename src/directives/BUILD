load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

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

cc_library(
    name = "matrix_manager",
    srcs = ["matrix_manager.cc"],
    hdrs = ["matrix_manager.h"],
    deps = [
        ":finite_float_t",
        "//src/common:error",
        "//src/common:pointer_types",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
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
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":matrix_manager",
        ":named_material_manager",
        ":scene_builder",
        ":transform",
        "//src/area_lights:parser",
        "//src/cameras:parser",
        "//src/color_integrators:parser",
        "//src/common:error",
        "//src/common:named_texture_manager",
        "//src/common:normal_map_manager",
        "//src/common:parameters",
        "//src/common:pointer_types",
        "//src/common:quoted_string",
        "//src/common:texture_manager",
        "//src/common:tokenizer",
        "//src/films:parser",
        "//src/films/output_writers:result",
        "//src/integrators:parser",
        "//src/light_propagation:parser",
        "//src/lights:parser",
        "//src/materials:parser",
        "//src/randoms:parser",
        "//src/samplers:parser",
        "//src/shapes:parser",
        "//src/textures:parser",
    ],
)

cc_library(
    name = "scene_builder",
    srcs = ["scene_builder.cc"],
    hdrs = ["scene_builder.h"],
    deps = [
        "//src/common:directive",
        "//src/common:error",
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
        ":matrix_manager",
        "//src/common:quoted_string",
        "//src/common:tokenizer",
    ],
)
