load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_binary(
    name = "iris",
    srcs = ["iris.cc"],
    visibility = ["//visibility:public"],
    deps = [
        ":render",
        "//src/directives:color_space",
        "//src/directives:parser",
        "@com_google_absl//absl/flags:config",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/flags:usage",
    ],
)

cc_binary(
    name = "iris_instrumented",
    defines = ["INSTRUMENTED_BUILD"],
    srcs = ["iris.cc"],
    visibility = ["//visibility:public"],
    linkopts = ["-lprofiler"],
    deps = [
        ":render",
        "//src/directives:color_space",
        "//src/directives:parser",
        "@com_google_absl//absl/flags:config",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/flags:parse",
        "@com_google_absl//absl/flags:usage",
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
        "//src/directives:parser",
        "@com_github_bradleymarie_iris//iris_camera_toolkit:status_bar_progress_reporter",
        "@com_github_bradleymarie_iris//iris_physx",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:sample_tracer",
    ],
)
