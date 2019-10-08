load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":stratified",
        "//src/common:call_directive",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "stratified",
    srcs = ["stratified.cc"],
    hdrs = ["stratified.h"],
    deps = [
        "//src/common:error",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
        "//src/param_matchers:integral_single",
        "//src/param_matchers:matcher",
        "//src/param_matchers:single",
        "@com_github_bradleymarie_iris//iris_camera_toolkit:grid_pixel_sampler",
    ],
)
