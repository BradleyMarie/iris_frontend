load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "matte",
    srcs = ["matte.cc"],
    hdrs = ["matte.h"],
    deps = [
        ":result",
        "//src/common:named_texture_manager",
        "//src/common:parameters",
        "//src/param_matchers:float_texture",
        "//src/param_matchers:reflector_texture",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":matte",
        ":plastic",
        ":result",
        "//src/common:directive",
        "//src/common:named_texture_manager",
    ],
)

cc_library(
    name = "plastic",
    srcs = ["plastic.cc"],
    hdrs = ["plastic.h"],
    deps = [
        ":result",
        "//src/common:named_texture_manager",
        "//src/common:parameters",
        "//src/param_matchers:float_texture",
        "//src/param_matchers:reflector_texture",
        "//src/param_matchers:single",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        "//src/common:material_manager",
        "//src/common:named_texture_manager",
        "//src/common:normal_map_manager",
        "//src/common:parameters",
        "//src/common:spectrum_manager",
        "//src/common:texture_manager",
    ],
)