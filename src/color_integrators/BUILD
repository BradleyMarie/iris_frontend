load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "cie",
    srcs = ["cie.cc"],
    hdrs = ["cie.h"],
    deps = [
        "//src/common:error",
        "//src/common:parameters",
        "//src/common:pointer_types",
        "//src/param_matchers:matcher",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:cie_color_integrator",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":cie",
        "//src/common:call_directive",
        "//src/common:error",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
    ],
)
