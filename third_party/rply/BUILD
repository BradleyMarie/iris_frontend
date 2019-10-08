load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

licenses(["notice"])

cc_library(
    name = "rply",
    srcs = ["rply.c"],
    hdrs = [
        "rply.h",
        "rplyfile.h",
    ],
    visibility = [
        "//src/shapes:__pkg__",
    ],
)
