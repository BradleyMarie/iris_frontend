load("@rules_cc//cc:defs.bzl", "cc_test")

package(default_visibility = ["//visibility:private"])

cc_test(
    name = "render_tests",
    srcs = ["render_tests.cc"],
    data = [
        "cornell_box/cornell_box.pbrt",
        "cornell_box/cornell_box.pfm",
    ],
    shard_count = 2,
    deps = [
        "//src:render",
        "@com_google_googletest//:gtest_main",
    ],
)
