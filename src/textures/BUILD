load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "constant",
    srcs = ["constant.cc"],
    hdrs = ["constant.h"],
    deps = [
        "//src/common:texture_manager",
        "//src/common:tokenizer",
        "//src/param_matchers:float_texture",
        "//src/param_matchers:matcher",
        "//src/param_matchers:reflector_texture",
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
