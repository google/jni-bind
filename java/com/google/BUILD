load("@rules_java//java:defs.bzl", "java_library")

licenses(["notice"])

java_library(
    name = "random_string_java",
    srcs = ["RandomString.java"],
)

cc_library(
    name = "java_runtime_environment",
    hdrs = ["java_runtime_environment.h"],
    deps = ["//:jni_bind"],
)

cc_binary(
    name = "main",
    srcs = ["main.cc"],
    data = [":random_string_java"],
    deps = [
        ":java_runtime_environment",
        "//:jni_bind",
    ],
)
