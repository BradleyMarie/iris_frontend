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
    name = "matrix_manager",
    hdrs = ["matrix_manager.h"],
    srcs = ["matrix_manager.cc"],
    deps = [
        ":pointer_types",
    ],
)

cc_library(
    name = "pointer_types",
    hdrs = ["pointer_types.h"],
    deps = [
        ":shared_ptr",
        "@com_github_bradleymarie_iris//iris_physx:iris_physx",
    ],
)

cc_library(
    name = "shared_ptr",
    hdrs = ["shared_ptr.h"],
)

cc_library(
    name = "tokenizer",
    hdrs = ["tokenizer.h"],
    srcs = ["tokenizer.cc"],
    deps = [
        "@com_github_abseil_abseil_cpp//absl/strings",
        "@com_github_abseil_abseil_cpp//absl/types:optional"
    ],
)