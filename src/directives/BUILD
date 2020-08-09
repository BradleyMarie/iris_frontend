load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

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
        ":named_material_manager",
        ":scene_builder",
        "//src/area_lights:parser",
        "//src/cameras:parser",
        "//src/color_extrapolators:parser",
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
        "@com_github_bradleymarie_iris//iris_physx_toolkit:color_spectra",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:reflective_color_integrator",
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
