package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":result",
        ":matte",
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
    name = "matte",
    hdrs = ["matte.h"],
    srcs = ["matte.cc"],
    deps = [
        ":result",
        "//src:ostream",
        "//src:param_matcher",
        "//src:tokenizer",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:constant_material",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:lambertian_bsdf",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:uniform_reflector",
        "@com_google_absl//absl/strings", # TODO: Remove
    ],
)