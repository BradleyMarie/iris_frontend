package(default_visibility = ["//visibility:private"])

cc_binary(
    name = "iris",
    srcs = ["main.cc"],
    deps = [
        ":tokenizer",
        "@com_github_bradleymarie_iris//iris_physx:iris_physx",
        "@com_github_gflags_gflags//:gflags"
    ],
)

cc_library(
    name = "tokenizer",
    srcs = ["tokenizer.cc"],
    hdrs = ["tokenizer.h"],
    deps = [
        "@com_github_abseil_abseil_cpp//absl/strings",
        "@com_github_abseil_abseil_cpp//absl/types:optional"
    ],
)