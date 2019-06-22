package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":image",
        ":result",
        "//src:directive_parser",
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "image",
    srcs = ["image.cc"],
    hdrs = ["image.h"],
    deps = [
        ":result",
        "//src:param_matcher",
        "//src/common:tokenizer",
        "//src/films/output_writers:parser",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    deps = [
        "//src/common:pointer_types",
        "//src/films/output_writers:result",
    ],
)
