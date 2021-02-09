load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "directive",
    srcs = ["directive.cc"],
    hdrs = ["directive.h"],
    deps = [
        ":parameters",
        ":quoted_string",
        ":tokenizer",
        "//src/param_matchers:single",
        "@com_google_absl//absl/container:inlined_vector",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "error",
    srcs = ["error.cc"],
    hdrs = ["error.h"],
    deps = [
        "@com_github_bradleymarie_iris//iris",
    ],
)

cc_library(
    name = "material_manager",
    srcs = ["material_manager.cc"],
    hdrs = ["material_manager.h"],
    deps = [
        ":error",
        ":pointer_types",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:matte_material",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:plastic_material",
    ],
)

cc_library(
    name = "named_texture_manager",
    srcs = ["named_texture_manager.cc"],
    hdrs = ["named_texture_manager.h"],
    deps = [
        ":pointer_types",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "normal_map_manager",
    srcs = ["normal_map_manager.cc"],
    hdrs = ["normal_map_manager.h"],
    deps = [
        ":error",
        ":pointer_types",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:bump_map",
    ],
)

cc_library(
    name = "ostream",
    srcs = ["ostream.cc"],
    hdrs = ["ostream.h"],
    deps = [
        "@com_github_bradleymarie_iris//iris",
    ],
)

cc_library(
    name = "parameter",
    hdrs = ["parameter.h"],
    deps = [
        "@com_github_bradleymarie_iris//iris_camera",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/types:variant",
    ],
)

cc_library(
    name = "parameter_matcher",
    srcs = ["parameter_matcher.cc"],
    hdrs = ["parameter_matcher.h"],
    deps = [
        ":parameter",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:span",
    ],
)

cc_library(
    name = "parameters",
    srcs = ["parameters.cc"],
    hdrs = ["parameters.h"],
    deps = [
        ":parameter_matcher",
        ":quoted_string",
        ":tokenizer",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
    ],
)

cc_library(
    name = "pointer_types",
    hdrs = ["pointer_types.h"],
    deps = [
        ":shared_ptr",
        ":unique_ptr",
        "@com_github_bradleymarie_iris//iris_advanced_toolkit:low_discrepancy_sequence",
        "@com_github_bradleymarie_iris//iris_camera",
        "@com_github_bradleymarie_iris//iris_physx",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:color_extrapolator",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:float_texture",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:mipmap",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:reflector_texture",
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
    name = "shared_ptr",
    hdrs = ["shared_ptr.h"],
)

cc_library(
    name = "spectrum_manager",
    srcs = ["spectrum_manager.cc"],
    hdrs = ["spectrum_manager.h"],
    deps = [
        ":error",
        ":pointer_types",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:interpolated_spectrum",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:uniform_reflector",
        "@com_google_absl//absl/types:optional",
    ],
)

cc_library(
    name = "texture_manager",
    srcs = ["texture_manager.cc"],
    hdrs = ["texture_manager.h"],
    deps = [
        ":error",
        ":pointer_types",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:constant_texture",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:image_texture",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:product_texture",
    ],
)

cc_library(
    name = "tokenizer",
    srcs = ["tokenizer.cc"],
    hdrs = ["tokenizer.h"],
    deps = [
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
    ],
)

cc_library(
    name = "unique_ptr",
    hdrs = ["unique_ptr.h"],
)
