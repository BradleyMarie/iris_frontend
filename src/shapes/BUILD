package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":result",
        ":trianglemesh",
        "//src/common:call_directive",
        "//src/common:tokenizer",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    deps = [
        "//src/common:pointer_types",
    ],
)

cc_library(
    name = "trianglemesh",
    srcs = ["trianglemesh.cc"],
    hdrs = ["trianglemesh.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:ostream",
        "//src/common:tokenizer",
        "//src/param_matchers:list",
        "//src/param_matchers:matcher",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:triangle",
    ],
)
