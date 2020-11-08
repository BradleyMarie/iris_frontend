load("@rules_cc//cc:defs.bzl", "cc_test")

package(default_visibility = ["//visibility:private"])

cc_test(
    name = "render_tests",
    srcs = ["render_tests.cc"],
    data = glob(["**/*.p*"]),
    shard_count = 3,
    deps = [
        "//src:render",
        "@com_google_googletest//:gtest_main",
    ],
)
