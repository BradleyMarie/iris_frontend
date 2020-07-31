load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "float_single",
    srcs = ["float_single.cc"],
    hdrs = ["float_single.h"],
    deps = [
        "//src/common:parameter_matcher",
    ],
)

cc_library(
    name = "float_texture",
    srcs = ["float_texture.cc"],
    hdrs = ["float_texture.h"],
    deps = [
        "//src/common:error",
        "//src/common:named_texture_manager",
        "//src/common:parameter_matcher",
        "//src/common:pointer_types",
        "//src/common:texture_manager",
    ],
)

cc_library(
    name = "integral_single",
    hdrs = ["integral_single.h"],
    deps = [
        "//src/common:parameter_matcher",
    ],
)

cc_library(
    name = "list",
    hdrs = ["list.h"],
    deps = [
        "//src/common:parameter_matcher",
    ],
)

cc_library(
    name = "reflector_texture",
    srcs = ["reflector_texture.cc"],
    hdrs = ["reflector_texture.h"],
    deps = [
        ":spd_file",
        "//src/common:error",
        "//src/common:named_texture_manager",
        "//src/common:parameter_matcher",
        "//src/common:pointer_types",
        "//src/common:spectrum_manager",
        "//src/common:texture_manager",
    ],
)

cc_library(
    name = "single",
    hdrs = ["single.h"],
    deps = [
        "//src/common:parameter_matcher",
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
        ":spd_file",
        "//src/common:flags",
        "//src/common:parameter_matcher",
        "//src/common:pointer_types",
        "//src/common:spectrum_manager",
    ],
)
