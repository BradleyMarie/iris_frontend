load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "halton",
    srcs = ["halton.cc"],
    hdrs = ["halton.h"],
    deps = [
        "//src/common:error",
        "//src/common:parameters",
        "//src/common:pointer_types",
        "//src/param_matchers:integral_single",
        "@com_github_bradleymarie_iris//iris_advanced_toolkit:halton_sequence",
        "@com_github_bradleymarie_iris//iris_camera_toolkit:low_discrepancy_image_sampler",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":halton",
        ":sobol",
        ":stratified",
        "//src/common:directive",
    ],
)

cc_library(
    name = "sobol",
    srcs = ["sobol.cc"],
    hdrs = ["sobol.h"],
    deps = [
        "//src/common:error",
        "//src/common:parameters",
        "//src/common:pointer_types",
        "//src/param_matchers:integral_single",
        "@com_github_bradleymarie_iris//iris_advanced_toolkit:sobol_sequence",
        "@com_github_bradleymarie_iris//iris_camera_toolkit:low_discrepancy_image_sampler",
    ],
)

cc_library(
    name = "stratified",
    srcs = ["stratified.cc"],
    hdrs = ["stratified.h"],
    deps = [
        "//src/common:error",
        "//src/common:parameters",
        "//src/common:pointer_types",
        "//src/param_matchers:integral_single",
        "//src/param_matchers:single",
        "@com_github_bradleymarie_iris//iris_camera_toolkit:grid_image_sampler",
    ],
)
