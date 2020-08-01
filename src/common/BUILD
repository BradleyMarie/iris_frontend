load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "call_directive",
    hdrs = ["call_directive.h"],
    deps = [
        ":matrix_manager",
        ":parameters",
        ":quoted_string",
        ":tokenizer",
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
    name = "flags",
    srcs = ["flags.cc"],
    hdrs = ["flags.h"],
    deps = [
        "@com_github_bradleymarie_iris//iris_advanced",
        "@com_google_absl//absl/flags:flag",
    ],
)

cc_library(
    name = "matrix_manager",
    srcs = ["matrix_manager.cc"],
    hdrs = ["matrix_manager.h"],
    deps = [
        ":error",
        ":finite_float_t",
        ":pointer_types",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
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
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/types:variant",
        "@com_github_bradleymarie_iris//iris_camera",
        "@com_google_absl//absl/strings",
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
        ":flags",
        ":tokenizer",
        ":parameter_matcher",
        ":quoted_string",
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "pointer_types",
    hdrs = ["pointer_types.h"],
    deps = [
        ":shared_ptr",
        ":unique_ptr",
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
