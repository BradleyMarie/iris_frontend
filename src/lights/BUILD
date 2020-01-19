load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "distant",
    srcs = ["distant.cc"],
    hdrs = ["distant.h"],
    deps = [
        "//src/common:error",
        "//src/common:pointer_types",
        "//src/common:spectrum_manager",
        "//src/common:tokenizer",
        "//src/param_matchers:matcher",
        "//src/param_matchers:single",
        "//src/param_matchers:spectrum",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:directional_light",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":point",
        "//src/common:call_directive",
        "//src/common:pointer_types",
        "//src/common:spectrum_manager",
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "point",
    srcs = ["point.cc"],
    hdrs = ["point.h"],
    deps = [
        "//src/common:error",
        "//src/common:pointer_types",
        "//src/common:spectrum_manager",
        "//src/common:tokenizer",
        "//src/param_matchers:matcher",
        "//src/param_matchers:single",
        "//src/param_matchers:spectrum",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:point_light",
    ],
)
