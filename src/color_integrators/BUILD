package(default_visibility = ["//visibility:private"])

cc_library(
    name = "cie",
    srcs = ["cie.cc"],
    hdrs = ["cie.h"],
    deps = [
        "//src:param_matcher",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:cie_color_integrator",
    ],
)

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":cie",
        "//src:directive_parser",
        "//src/common:pointer_types",
        "//src/common:tokenizer",
    ],
)
