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
    name = "float_texture",
    srcs = ["float_texture.cc"],
    hdrs = ["float_texture.h"],
    deps = [
        ":matcher",
        "//src/common:error",
        "//src/common:named_texture_manager",
        "//src/common:pointer_types",
        "//src/common:texture_manager",
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
        "//src/common:pointer_types",
        "//src/common:quoted_string",
        "//src/common:tokenizer",
        "@com_github_bradleymarie_iris//iris_physx",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/types:optional",
        "@com_google_absl//absl/types:variant",
    ],
)

cc_library(
    name = "reflector_texture",
    srcs = ["reflector_texture.cc"],
    hdrs = ["reflector_texture.h"],
    deps = [
        ":matcher",
        ":spd_file",
        "//src/common:error",
        "//src/common:named_texture_manager",
        "//src/common:pointer_types",
        "//src/common:spectrum_manager",
        "//src/common:texture_manager",
    ],
)

cc_library(
    name = "single",
    hdrs = ["single.h"],
    deps = [
        ":matcher",
    ],
)

cc_library(
    name = "spd_file",
    srcs = ["spd_file.cc"],
    hdrs = ["spd_file.h"],
    deps = [
        "//src/common:tokenizer",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "spectrum",
    srcs = ["spectrum.cc"],
    hdrs = ["spectrum.h"],
    deps = [
        ":matcher",
        ":spd_file",
        "//src/common:pointer_types",
        "//src/common:spectrum_manager",
    ],
)
