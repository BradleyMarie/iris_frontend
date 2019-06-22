package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":pcg",
        "//src:directive_parser",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "pcg",
    srcs = ["pcg.cc"],
    hdrs = ["pcg.h"],
    deps = [
        "//src/common:pointer_types",
        "//src/common:tokenizer",
        "//src/param_matchers:matcher",
        "@com_github_bradleymarie_iris//iris_advanced_toolkit:pcg_random",
    ],
)
