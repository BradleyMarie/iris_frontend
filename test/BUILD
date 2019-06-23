package(default_visibility = ["//visibility:private"])

cc_test(
    name = "render_tests",
    shard_count = 2,
    srcs = ["render_tests.cc"],
    data = [
        "cornell_box.pbrt",
        "cornell_box.pfm",
    ],
    deps = [
        "//src:render",
        "@com_google_googletest//:gtest_main",
    ],
)