load("@rules_cc//cc:cc_library.bzl", "cc_library")

licenses(["notice"])

cc_library(
    name = "application",
    hdrs = ["application.h"],
    visibility = ["//:__subpackages__"],
    deps = [
        "//:jni_dep",
        "//class_defs:java_lang_classes",
        "//implementation:class",
        "//implementation:method",
        "//implementation:return",
    ],
)

cc_library(
    name = "activity_thread",
    hdrs = ["activity_thread.h"],
    visibility = ["//:__subpackages__"],
    deps = [
        ":application",
        "//:jni_dep",
        "//implementation:class",
        "//implementation:method",
        "//implementation:return",
        "//implementation:self",
        "//implementation:static",
    ],
)

filegroup(
    name = "headers_for_export",
    srcs = glob(["*.h"]),
    visibility = ["//visibility:public"],
)
