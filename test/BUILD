package(default_visibility = ["//visibility:private"])

cc_test(
    name = "render_tests",
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