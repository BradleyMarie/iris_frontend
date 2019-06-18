package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    deps = [
        ":result",
        "//src:tokenizer",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    deps = [
        "//src:pointer_types",
    ],
)

cc_library(
    name = "trianglemesh",
    hdrs = ["trianglemesh.h"],
    srcs = ["trianglemesh.cc"],
    deps = [
        ":result",
        "//src:ostream",
        "//src:param_matcher",
        "//src:tokenizer",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:triangle",
    ],
)