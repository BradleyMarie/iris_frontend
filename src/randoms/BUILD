package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":pcg",
        "//src:directive_parser",
        "//src:pointer_types",
        "//src:tokenizer",
    ],
)

cc_library(
    name = "pcg",
    srcs = ["pcg.cc"],
    hdrs = ["pcg.h"],
    deps = [
        "//src:param_matcher",
        "//src:pointer_types",
        "//src:tokenizer",
        "@com_github_bradleymarie_iris//iris_advanced_toolkit:pcg_random",
    ],
)
