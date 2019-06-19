package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":stratified",
        "//src:directive_parser",
        "//src:pointer_types",
        "//src:tokenizer",
    ],
)

cc_library(
    name = "stratified",
    srcs = ["stratified.cc"],
    hdrs = ["stratified.h"],
    deps = [
        "//src:param_matcher",
        "//src:pointer_types",
        "//src:tokenizer",
        "@com_github_bradleymarie_iris//iris_camera_toolkit:grid_pixel_sampler",
    ],
)
