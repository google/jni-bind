load("@rules_cc//cc:cc_library.bzl", "cc_library")

# `bind_lang` is the language-agnostic core of the binding libraries (JNI Bind,
# Proto Bind). Targets here must never depend on `//third_party/jni_wrapper/implementation`
# (JNI Bind) or any other language-specific package.
package(
    default_visibility = ["//:__subpackages__"],
)

licenses(["notice"])

filegroup(
    name = "headers_for_export",
    srcs = glob(["*.h"]),
)

cc_library(
    name = "id_type",
    hdrs = ["id_type.h"],
)

cc_library(
    name = "no_idx",
    hdrs = ["no_idx.h"],
)

cc_library(
    name = "lifecycle",
    hdrs = ["lifecycle.h"],
)

cc_library(
    name = "ref_base",
    hdrs = ["ref_base.h"],
)
