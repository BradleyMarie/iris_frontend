load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "float_single",
    srcs = ["float_single.cc"],
    hdrs = ["float_single.h"],
    deps = [
        ":matcher",
    ],
)

cc_library(
    name = "integral_single",
    hdrs = ["integral_single.h"],
    deps = [
        ":matcher",
    ],
)

cc_library(
    name = "list",
    hdrs = ["list.h"],
    deps = [
        ":matcher",
    ],
)

cc_library(
    name = "matcher",
    srcs = ["matcher.cc"],
    hdrs = ["matcher.h"],
    deps = [
        ":parser",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    deps = [
        "//src/common:error",
        "//src/common:pointer_types",
        "//src/common:quoted_string",
        "//src/common:tokenizer",
        "@com_github_bradleymarie_iris//iris_physx",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:interpolated_spectrum",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/types:variant",
    ],
)

cc_library(
    name = "single",
    hdrs = ["single.h"],
    deps = [
        ":matcher",
    ],
)
