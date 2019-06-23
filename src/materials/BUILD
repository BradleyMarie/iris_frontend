package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":matte",
        ":result",
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
    name = "matte",
    srcs = ["matte.cc"],
    hdrs = ["matte.h"],
    deps = [
        ":result",
        "//src/common:tokenizer",
        "//src/param_matchers:matcher",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:constant_material",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:lambertian_bsdf",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:uniform_reflector",
        "@com_google_absl//absl/strings",  # TODO: Remove
    ],
)
