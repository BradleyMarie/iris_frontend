package(default_visibility = ["//visibility:private"])

cc_binary(
    name = "pfm_matches",
    testonly = 1,
    srcs = ["pfm_matches.cc"],
)

genrule(
    name = "cornell_box",
    srcs = [
        "cornell_box.pbrt",
    ],
    outs = ["cornell_box_latest.pfm"],
    cmd = "$(location //src:iris) --default_output=$@ $(location cornell_box.pbrt)",
    tools = ["//src:iris"],
)

sh_test(
    name = "check_cornell_box",
    srcs = [":pfm_matches"],
    args = [
        "exact",
        "$(location cornell_box.pfm)",
        "$(location :cornell_box_latest.pfm)",
    ],
    data = [
        "cornell_box.pfm",
        ":cornell_box_latest.pfm",
    ],
)
