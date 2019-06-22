package(default_visibility = ["//visibility:private"])

cc_library(
    name = "math",
    srcs = ["math.cc"],
    hdrs = ["math.h"],
    deps = [
        "//src/common:pointer_types",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":perspective",
        ":result",
        "//src:directive_parser",
        "//src/common:matrix_manager",
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "perspective",
    srcs = ["perspective.cc"],
    hdrs = ["perspective.h"],
    deps = [
        ":math",
        ":result",
        "//src:param_matcher",
        "//src/common:matrix_manager",
        "//src/common:tokenizer",
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
