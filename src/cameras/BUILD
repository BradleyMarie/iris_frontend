load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":perspective",
        ":result",
        "//src/common:directive",
    ],
)

cc_library(
    name = "perspective",
    srcs = ["perspective.cc"],
    hdrs = ["perspective.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:parameters",
        "//src/param_matchers:float_single",
        "@com_github_bradleymarie_iris//iris_camera_toolkit:pinhole_camera",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    deps = [
        "//src/common:pointer_types",
    ],
)
