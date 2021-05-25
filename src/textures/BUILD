load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "constant",
    srcs = ["constant.cc"],
    hdrs = ["constant.h"],
    deps = [
        "//src/common:parameters",
        "//src/common:texture_manager",
        "//src/param_matchers:float_texture",
        "//src/param_matchers:reflector_texture",
    ],
)

cc_library(
    name = "imagemap",
    srcs = ["imagemap.cc"],
    hdrs = ["imagemap.h"],
    deps = [
        "//src/common:parameters",
        "//src/common:texture_manager",
        "//src/param_matchers:file",
        "//src/param_matchers:float_single",
        "//src/param_matchers:float_texture",
        "//src/param_matchers:reflector_texture",
        "//src/param_matchers:single",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:png_mipmap",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":constant",
        ":imagemap",
        ":scale",
        ":windy",
        "//src/common:directive",
        "//src/common:texture_manager",
    ],
)

cc_library(
    name = "scale",
    srcs = ["scale.cc"],
    hdrs = ["scale.h"],
    deps = [
        "//src/common:parameters",
        "//src/common:texture_manager",
        "//src/param_matchers:float_texture",
        "//src/param_matchers:reflector_texture",
    ],
)

cc_library(
    name = "windy",
    srcs = ["windy.cc"],
    hdrs = ["windy.h"],
    deps = [
        "//src/common:parameters",
        "//src/common:texture_manager",
        "//src/param_matchers:float_texture",
        "//src/param_matchers:reflector_texture",
    ],
)