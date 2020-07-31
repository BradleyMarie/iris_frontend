load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "diffuse",
    srcs = ["diffuse.cc"],
    hdrs = ["diffuse.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:parameters",
        "//src/param_matchers:single",
        "//src/param_matchers:spectrum",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:constant_emissive_material",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":diffuse",
        ":result",
        "//src/common:call_directive",
        "//src/common:parameters",
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    deps = [
        "//src/common:pointer_types",
    ],
)
