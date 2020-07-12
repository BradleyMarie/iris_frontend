load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

genrule(
    name = "gperftools",
    srcs = glob([
        "**/*",
    ]),
    outs = ["libtcmalloc_and_profiler.a"],
    cmd = "pushd external/gperftools && ./autogen.sh && ./configure && make && popd && cp external/gperftools/.libs/libtcmalloc_and_profiler.a $@",
)

cc_library(
    name = "libtcmalloc_and_profiler",
    srcs = ["libtcmalloc_and_profiler.a"],
    hdrs = [
        "src/gperftools/heap-checker.h",
        "src/gperftools/heap-profiler.h",
        "src/gperftools/malloc_extension.h",
        "src/gperftools/malloc_extension_c.h",
        "src/gperftools/malloc_hook.h",
        "src/gperftools/malloc_hook_c.h",
        "src/gperftools/nallocx.h",
        "src/gperftools/profiler.h",
        "src/gperftools/stacktrace.h",
        "src/gperftools/tcmalloc.h.in",
    ],
    strip_include_prefix = "src",
)
