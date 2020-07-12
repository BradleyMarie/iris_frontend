load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository", "new_git_repository")

git_repository(
    name = "com_github_bradleymarie_iris",
    commit = "65a255ae2a9c0d1424efc133efc975d6230e0518",
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
    name = "gperftools",
    build_file = "gperftools.BUILD",
    remote = "https://github.com/gperftools/gperftools.git",
    tag = "gperftools-2.8",
)

new_git_repository(
    name = "rply",
    build_file = "rply.BUILD",
    remote = "https://github.com/diegonehab/rply.git",
    tag = "v1.1.4",
)

new_git_repository(
    name = "tinyexr",
    build_file = "tinyexr.BUILD",
    commit = "569b35454bebf9e70185303bb66d78ca07606b9e",
    remote = "https://github.com/syoyo/tinyexr.git",
)
