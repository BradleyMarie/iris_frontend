load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "call_directive",
    hdrs = ["call_directive.h"],
    deps = [
        ":matrix_manager",
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
    name = "material_factory",
    hdrs = ["material_factory.h"],
    deps = [
        ":material_manager",
        ":named_texture_manager",
        ":spectrum_manager",
        ":texture_manager",
        "//src/param_matchers:parser",
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
    ],
)

cc_library(
    name = "named_material_manager",
    srcs = ["named_material_manager.cc"],
    hdrs = ["named_material_manager.h"],
    deps = [
        ":material_factory",
        ":pointer_types",
        "@com_google_absl//absl/container:flat_hash_map",
        "@com_google_absl//absl/strings",
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
    name = "ostream",
    srcs = ["ostream.cc"],
    hdrs = ["ostream.h"],
    deps = [
        "@com_github_bradleymarie_iris//iris",
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
        "@com_github_bradleymarie_iris//iris_physx_toolkit:xyz_spectra",
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
