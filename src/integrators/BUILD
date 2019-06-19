package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":path",
        ":result",
        "//src:directive_parser",
        "//src:tokenizer",
    ],
)

cc_library(
    name = "path",
    hdrs = ["path.h"],
    srcs = ["path.cc"],
    deps = [
        ":result",
        "//src:param_matcher",
        "//src:tokenizer",
        "//src/integrators/lightstrategy:parser",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:path_tracer",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    deps = [
        "//src:pointer_types",
        "//src/integrators/lightstrategy:result",
    ],
)