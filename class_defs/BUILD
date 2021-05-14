package(default_visibility = ["//visibility:public"])

licenses(["notice"])

exports_files(["LICENSE"])

cc_library(
    name = "java_lang_classes",
    hdrs = ["java_lang_classes.h"],
    deps = [
        "//:class",
        "//:jni_dep",
        "//:method",
        "//:params",
        "//:return",
    ],
)

cc_library(
    name = "java_util_classes",
    hdrs = ["java_util_classes.h"],
    deps = [
        ":java_lang_classes",
        "//:class",
        "//:jni_dep",
        "//:method",
        "//:params",
        "//:return",
    ],
)
