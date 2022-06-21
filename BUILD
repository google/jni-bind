package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

filegroup(
    name = "headers_for_export",
    srcs = glob(
        ["**/*.h"],
        exclude = [
            "jni_bind_release.h",
            "jni_bind_release_for_testing.h",
            ":jni_bind_release_target",
        ],
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

# Copy of the release header (above) that uses a different filename.
# Strangely, if a file is present with the same name as what's in outs blaze
# will ignore the genrule and depend on the real header, but it will fail
# because there is no cc_library providing the header.  jni_bind_release.h is
# checked in, so the above *will* fail.
genrule(
    name = "gen_jni_bind_release_for_testing",
    outs = ["jni_bind_release_for_testing.h"],
    cmd = "./$(location build_jni_bind_release.sh) $(location jni_bind_release_input) jni_wrapper $(locations :jni_bind_decorative_text)>$@",
    tools = [
        ":build_jni_bind_release.sh",
        ":headers_for_export",
        ":jni_bind_decorative_text",
        ":jni_bind_release_input",
        "//class_defs:headers_for_export",
        "//implementation:headers_for_export",
        "//implementation/jni_helper:headers_for_export",
        "//metaprogramming:headers_for_export",
    ],
    visibility = ["//visibility:private"],
)

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
    hdrs = ["jni_bind_release_for_testing.h"],
    visibility = ["//visibility:private"],
    deps = ["//third_party/java/jdk:jni"],
)
