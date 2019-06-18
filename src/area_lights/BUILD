package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":result",
        ":diffuse",
        "//src:directive_parser",
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
    name = "diffuse",
    hdrs = ["diffuse.h"],
    srcs = ["diffuse.cc"],
    deps = [
        ":result",
        "//src:param_matcher",
        "//src:tokenizer",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:constant_emissive_material",
    ],
)