load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "color_space",
    srcs = ["color_space.cc"],
    hdrs = ["color_space.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        "@com_github_bradleymarie_iris//iris_advanced",
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
        ":named_material_manager",
        ":pbrt_workaround",
        ":rgb_color_space_parser",
        ":scene_builder",
        ":spectral_representation",
        ":spectral_representation_parser",
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
    name = "pbrt_workaround",
    srcs = ["pbrt_workaround.cc"],
    hdrs = ["pbrt_workaround.h"],
    deps = [
        "//src/common:directive",
    ],
)

cc_library(
    name = "rgb_color_space_parser",
    srcs = ["rgb_color_space_parser.cc"],
    hdrs = ["rgb_color_space_parser.h"],
    deps = [
        ":color_space",
        "//src/common:directive",
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
        "@com_github_bradleymarie_iris//iris_physx_toolkit:aggregate_environmental_light",
        "@com_github_bradleymarie_iris//iris_physx_toolkit/scenes:kd_tree",
        "@com_google_absl//absl/container:flat_hash_map",
    ],
)

cc_library(
    name = "spectral_representation",
    srcs = ["spectral_representation.cc"],
    hdrs = ["spectral_representation.h"],
    deps = [
        ":color_space",
        "@com_github_bradleymarie_iris//iris_advanced",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
    ],
)

cc_library(
    name = "spectral_representation_parser",
    srcs = ["spectral_representation_parser.cc"],
    hdrs = ["spectral_representation_parser.h"],
    deps = [
        ":spectral_representation",
        "//src/common:directive",
    ],
)
