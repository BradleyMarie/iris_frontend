load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":pcg",
        "//src/common:directive",
        "//src/common:pointer_types",
    ],
)

cc_library(
    name = "pcg",
    srcs = ["pcg.cc"],
    hdrs = ["pcg.h"],
    deps = [
        "//src/common:error",
        "//src/common:parameters",
        "//src/common:pointer_types",
        "@com_github_bradleymarie_iris//iris_advanced_toolkit:pcg_random",
    ],
)
