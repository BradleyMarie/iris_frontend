package(default_visibility = ["//visibility:private"])

cc_binary(
    name = "iris",
    srcs = ["iris.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":render",
        "//src/common:tokenizer",
        "@com_google_absl//absl/flags:config",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
    ],
)

cc_library(
    name = "render",
    srcs = ["render.cc"],
    hdrs = ["render.h"],
    visibility = ["//test:__subpackages__"],
    deps = [
        "//src/common:error",
        "//src/common:ostream",
        "//src/common:tokenizer",
        "//src/directives:parse",
        "@com_github_bradleymarie_iris//iris_physx:iris_physx",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:sample_tracer",
    ],
)