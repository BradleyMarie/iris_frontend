load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "distant",
    srcs = ["distant.cc"],
    hdrs = ["distant.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:parameters",
        "//src/common:pointer_types",
        "//src/common:spectrum_manager",
        "//src/param_matchers:single",
        "//src/param_matchers:spectrum",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:directional_light",
    ],
)

cc_library(
    name = "infinite",
    srcs = ["infinite.cc"],
    hdrs = ["infinite.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:parameters",
        "//src/common:pointer_types",
        "//src/common:spectrum_manager",
        "//src/param_matchers:file",
        "@com_github_bradleymarie_iris//iris_advanced_toolkit:color_io",
        "@com_github_bradleymarie_iris//iris_advanced_toolkit:lanczos_upscale",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:infinite_environmental_light",
        "@com_google_absl//absl/strings",
        "@tinyexr",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":distant",
        ":infinite",
        ":point",
        ":result",
        "//src/common:directive",
        "//src/common:spectrum_manager",
    ],
)

cc_library(
    name = "point",
    srcs = ["point.cc"],
    hdrs = ["point.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:parameters",
        "//src/common:pointer_types",
        "//src/common:spectrum_manager",
        "//src/param_matchers:single",
        "//src/param_matchers:spectrum",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:point_light",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    deps = [
        "//src/common:pointer_types",
    ],
)