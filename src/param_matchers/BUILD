package(default_visibility = ["//src:__subpackages__"])

cc_library(
    name = "float_single",
    srcs = ["float_single.cc"],
    hdrs = ["float_single.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":matcher",
    ],
)

cc_library(
    name = "integral_single",
    hdrs = ["integral_single.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":matcher",
    ],
)

cc_library(
    name = "list",
    hdrs = ["list.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":matcher",
    ],
)

cc_library(
    name = "matcher",
    srcs = ["matcher.cc"],
    hdrs = ["matcher.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        "//src:param_parser",
    ],
)

cc_library(
    name = "single",
    hdrs = ["single.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":matcher",
    ],
)