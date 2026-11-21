load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_cc//cc:cc_test.bzl", "cc_test")

# Language-agnostic proxy machinery shared by the binding libraries. Targets here
# must never depend on `//third_party/jni_wrapper/implementation` (JNI Bind).
package(
    default_visibility = ["//:__subpackages__"],
)

licenses(["notice"])

filegroup(
    name = "headers_for_export",
    srcs = glob(["*.h"]),
)

cc_library(
    name = "is_convertible_key",
    hdrs = ["is_convertible_key.h"],
)

cc_library(
    name = "proxy_base",
    hdrs = ["proxy_base.h"],
    deps = [
        ":is_convertible_key",
        "//bind_lang:ref_base",
    ],
)

cc_library(
    name = "generic_proxy_helper",
    hdrs = ["generic_proxy_helper.h"],
    deps = [
        ":is_convertible_key",
        "//bind_lang/metaprogramming:cartesian_product",
        "//bind_lang/metaprogramming:combine",
        "//bind_lang/metaprogramming:concatenate",
        "//bind_lang/metaprogramming:flatten",
        "//bind_lang/metaprogramming:invoke",
        "//bind_lang/metaprogramming:reduce",
        "//bind_lang/metaprogramming:type_to_type_map",
        "//bind_lang/metaprogramming:unique_set",
    ],
)

cc_test(
    name = "proxy_test",
    srcs = ["proxy_test.cc"],
    deps = [
        ":generic_proxy_helper",
        ":is_convertible_key",
        ":proxy_base",
        "//bind_lang:ref_base",
        "@googletest//:gtest_main",
    ],
)
