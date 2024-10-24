load("@rules_java//java:defs.bzl", "java_library")

licenses(["notice"])

java_library(
    name = "address_formatter_java",
    srcs = ["AddressFormatter.java"],
    data = [":address.txt"],
)

cc_library(
    name = "address_formatter_cc",
    hdrs = ["address_formatter.h"],
    deps = [
        "//:jni_bind",
        "//:jni_dep",
    ],
)

cc_binary(
    name = "address_formatter_main",
    srcs = ["address_formatter_main.cc"],
    data = [":address_formatter_java"],
    deps = [
        ":address_formatter_cc",
        "//:jni_bind",
        "//:jni_dep",
    ],
)
