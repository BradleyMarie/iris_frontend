load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "matcher",
    hdrs = ["matcher.h"],
    deps = [
        "//src/param_matchers:matcher",
        "//src/param_matchers:single",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":matte",
        "//src/common:call_directive",
        "//src/common:material_manager",
        "//src/common:named_texture_manager",
        "//src/common:texture_manager",
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "matte",
    srcs = ["matte.cc"],
    hdrs = ["matte.h"],
    deps = [
        ":matcher",
        "//src/common:material_manager",
        "//src/common:named_texture_manager",
        "//src/common:texture_manager",
        "//src/common:tokenizer",
        "//src/param_matchers:reflector_texture",
    ],
)
