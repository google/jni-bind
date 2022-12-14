load("//tools/build_defs/license:license.bzl", "license")

package(
    default_applicable_licenses = ["//third_party/jni_wrapper/class_defs:license"],
    default_visibility = ["//visibility:public"],
)

license(
    name = "license",
    package_name = "class_defs",
)

licenses(["notice"])

filegroup(
    name = "headers_for_export",
    srcs = glob(["**/*.h"]),
)

exports_files(["LICENSE"])

cc_library(
    name = "java_lang_classes",
    hdrs = ["java_lang_classes.h"],
    deps = [
        "//:jni_dep",
        "//implementation:array",
        "//implementation:class",
        "//implementation:method",
        "//implementation:params",
        "//implementation:return",
    ],
)

cc_library(
    name = "java_util_classes",
    hdrs = ["java_util_classes.h"],
    deps = [
        ":java_lang_classes",
        "//:jni_dep",
        "//implementation:class",
        "//implementation:method",
        "//implementation:params",
        "//implementation:return",
    ],
)
