package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src/integrators:__subpackages__"],
    deps = [
        ":result",
        ":uniform",
        "@com_google_absl//absl/strings",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    visibility = ["//src/integrators:__subpackages__"],
    deps = [
        "//src/common:pointer_types",
    ],
)

cc_library(
    name = "uniform",
    srcs = ["uniform.cc"],
    hdrs = ["uniform.h"],
    deps = [
        ":result",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:one_light_sampler",
    ],
)
