load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":path",
        ":result",
        "//src/common:directive",
    ],
)

cc_library(
    name = "path",
    srcs = ["path.cc"],
    hdrs = ["path.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:parameters",
        "//src/integrators/lightstrategy:parser",
        "//src/param_matchers:float_single",
        "//src/param_matchers:integral_single",
        "//src/param_matchers:single",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:path_tracer",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    deps = [
        "//src/common:pointer_types",
        "//src/integrators/lightstrategy:result",
    ],
)
