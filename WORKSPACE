load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

git_repository(
    name = "com_github_bradleymarie_iris",
    commit = "ee34b215f8d50f6f1a7181304856c1bdf4267b7c",
    remote = "https://github.com/BradleyMarie/iris",
)

git_repository(
    name = "com_google_absl",
    remote = "https://github.com/abseil/abseil-cpp",
    tag = "20190808",
)

git_repository(
    name = "com_google_googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.8.1",
)

new_git_repository(
    name = "rply",
    build_file = "rply.BUILD",
    commit = "4296cc91b5c8c26d4e7d7aac0cee2b194ffc5800",
    remote = "https://github.com/diegonehab/rply.git",
)

new_git_repository(
    name = "tinyexr",
    build_file = "tinyexr.BUILD",
    commit = "569b35454bebf9e70185303bb66d78ca07606b9e",
    remote = "https://github.com/syoyo/tinyexr.git",
)

# From Iris

new_git_repository(
    name = "pcg_c_basic",
    build_file = "pcg_c_basic.BUILD",
    commit = "bc39cd76ac3d541e618606bcc6e1e5ba5e5e6aa3",
    remote = "https://github.com/imneme/pcg-c-basic.git",
)

new_git_repository(
    name = "stb",
    build_file = "stb.BUILD",
    commit = "f54acd4e13430c5122cab4ca657705c84aa61b08",
    remote = "https://github.com/nothings/stb.git",
)
