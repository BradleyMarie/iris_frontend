load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "constant",
    srcs = ["constant.cc"],
    hdrs = ["constant.h"],
    deps = [
        "//src/common:error",
        "//src/common:texture_manager",
        "//src/common:tokenizer",
        "//src/param_matchers:float_single",  # TODO: Remove
        "//src/param_matchers:matcher",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:constant_texture",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:uniform_reflector",
        "@com_google_absl//absl/strings",  # TODO: Remove
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":constant",
        "//src/common:call_directive",
        "//src/common:texture_manager",
        "//src/common:tokenizer",
    ],
)
