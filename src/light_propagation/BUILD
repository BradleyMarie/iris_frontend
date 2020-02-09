load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":color",
        ":result",
        ":spectrum",
        "//src/common:call_directive",
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "color",
    srcs = ["color.cc"],
    hdrs = ["color.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:tokenizer",
        "//src/param_matchers:matcher",
        "//src/param_matchers:single",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:color_spectra",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    deps = [
        "//src/common:pointer_types",
        "//src/common:spectrum_manager",
    ],
)

cc_library(
    name = "spectrum",
    srcs = ["spectrum.cc"],
    hdrs = ["spectrum.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:tokenizer",
        "//src/param_matchers:matcher",
        "//src/param_matchers:single",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:smits_color_extrapolator",
    ],
)
