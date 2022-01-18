package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

filegroup(
    name = "headers_for_export",
    srcs = glob(
        ["**/*.h"],
        exclude = ["jni_bind_release.h"],
    ),
)

exports_files(["LICENSE"])

################################################################################
# JNI Bind
#
# Depend on this target to include JNI Bind in your own target.
# For further documentation see https://github.com/google/jni-bind.
################################################################################
cc_library(
    name = "jni_bind",
    hdrs = ["jni_bind.h"],
    deps = [
        ":jni_dep",
        "//class_defs:java_lang_classes",
        "//class_defs:java_util_classes",
        "//implementation:array",
        "//implementation:class",
        "//implementation:class_loader",
        "//implementation:constructor",
        "//implementation:field",
        "//implementation:global_class_loader",
        "//implementation:global_object",
        "//implementation:jvm",
        "//implementation:jvm_ref",
        "//implementation:local_array",
        "//implementation:local_class_loader",
        "//implementation:local_object",
        "//implementation:method",
        "//implementation:params",
        "//implementation:return",
        "//implementation:string",
        "//implementation:string_ref",
        "//implementation:supported_class_set",
    ],
)

# Intentionally placed at root because of issues in Bazel.
cc_library(
    name = "jni_dep",
    srcs = [
        "@local_jdk//:jni_header",
        "@local_jdk//:jni_md_header-linux",
    ],
    hdrs = ["jni_dep.h"],
    includes = [
        "external/local_jdk/include",
        "external/local_jdk/include/linux",
    ],
)

################################################################################
# Testing Targets.
################################################################################
cc_library(
    name = "jni_test",
    testonly = 1,
    hdrs = ["jni_test.h"],
    deps = [
        ":jni_bind",
        ":jni_dep",
        ":mock_jni_env",
        ":mock_jvm",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "mock_jni_env",
    testonly = 1,
    hdrs = ["mock_jni_env.h"],
    deps = [
        "//:jni_dep",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "mock_jvm",
    testonly = 1,
    hdrs = ["mock_jvm.h"],
    deps = [
        "//:jni_dep",
        "@googletest//:gtest_main",
    ],
)

################################################################################
# Release targets.
################################################################################

filegroup(
    name = "jni_bind_decorative_text",
    srcs = [
        "jni_bind_release_leader.inc",
        "jni_bind_release_trailer.inc",
    ],
    data = [
        "jni_bind_release_leader.inc",
        "jni_bind_release_trailer.inc",
    ],
)

# Use :jni_bind, this is solely for build validation of the release header.
cc_library(
    name = "jni_bind_release_target",
    hdrs = ["jni_bind_release.h"],
    visibility = ["//visibility:private"],
)
