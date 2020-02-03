load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "smits",
    srcs = ["smits.cc"],
    hdrs = ["smits.h"],
    deps = [
        "//src/common:error",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
        "//src/param_matchers:matcher",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:smits_color_extrapolator",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":smits",
        "//src/common:call_directive",
        "//src/common:error",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
    ],
)
