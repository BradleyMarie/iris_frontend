load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:private"])

cc_library(
    name = "parser",
    srcs = ["parser.cc"],
    hdrs = ["parser.h"],
    visibility = ["//src:__subpackages__"],
    deps = [
        ":plymesh",
        ":result",
        ":sphere",
        ":trianglemesh",
        "//src/common:directive",
        "//src/materials:result",
    ],
)

cc_library(
    name = "result",
    hdrs = ["result.h"],
    deps = [
        "//src/common:pointer_types",
    ],
)

cc_library(
    name = "plymesh",
    srcs = ["plymesh.cc"],
    hdrs = ["plymesh.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:ostream",
        "//src/common:parameters",
        "//src/materials:result",
        "//src/param_matchers:file",
        "//src/param_matchers:float_texture",
        "@com_github_bradleymarie_iris//iris_physx_toolkit/shapes:triangle_mesh",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:triangle_mesh_normal_map",
        "@com_github_bradleymarie_iris//iris_physx_toolkit:triangle_mesh_texture_coordinate_map",
        "@rply",
    ],
)

cc_library(
    name = "sphere",
    srcs = ["sphere.cc"],
    hdrs = ["sphere.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:ostream",
        "//src/common:parameters",
        "//src/materials:result",
        "//src/param_matchers:float_single",
        "@com_github_bradleymarie_iris//iris_physx_toolkit/shapes:sphere",
    ],
)

cc_library(
    name = "trianglemesh",
    srcs = ["trianglemesh.cc"],
    hdrs = ["trianglemesh.h"],
    deps = [
        ":result",
        "//src/common:error",
        "//src/common:ostream",
        "//src/common:parameters",
        "//src/materials:result",
        "//src/param_matchers:float_texture",
        "//src/param_matchers:list",
        "@com_github_bradleymarie_iris//iris_physx_toolkit/shapes:triangle_mesh",
    ],
)
