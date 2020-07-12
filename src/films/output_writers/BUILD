load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "exr",
    srcs = ["exr.cc"],
    hdrs = ["exr.h"],
    deps = [
        ":lock_file",
        ":result",
        "@com_google_absl//absl/strings",
        "@tinyexr",
    ],
)

cc_library(
    name = "lock_file",
    srcs = ["lock_file.cc"],
    hdrs = ["lock_file.h"],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src/films:__subpackages__"],
    deps = [
        ":exr",
        ":pfm",
        ":result",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        "//src/common:pointer_types",
    ],
)

cc_library(
    name = "pfm",
    srcs = ["pfm.cc"],
    hdrs = ["pfm.h"],
    deps = [
        ":lock_file",
        ":result",
        "@com_github_bradleymarie_iris//iris_camera_toolkit:pfm_writer",
        "@com_google_absl//absl/strings",
    ],
)
