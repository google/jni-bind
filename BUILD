load("@rules_cc//cc:cc_library.bzl", "cc_library")
load("@rules_cc//cc:cc_test.bzl", "cc_test")

package(licenses = ["notice"])

filegroup(
    name = "headers_for_export",
    srcs = glob(
        ["*.h"],
        exclude = [
            "jni_bind_release.h",
            "jni_bind_release_for_testing.h",
            ":jni_bind_release_target",
        ],
    ),
    visibility = ["//visibility:public"],
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
    visibility = ["//visibility:public"],
    deps = [
        ":jni_dep",
        "//class_defs:java_lang_classes",
        "//class_defs:java_lang_exception",
        "//class_defs:java_lang_throwable",
        "//class_defs:java_util_array_list",
        "//class_defs:java_util_classes",
        "//implementation:array",
        "//implementation:array_type_conversion",
        "//implementation:array_view",
        "//implementation:class",
        "//implementation:class_loader",
        "//implementation:configuration",
        "//implementation:constructor",
        "//implementation:default_class_loader",
        "//implementation:extends",
        "//implementation:field",
        "//implementation:find_class_fallback",
        "//implementation:forward_declarations",
        "//implementation:global_class_loader",
        "//implementation:global_exception",
        "//implementation:global_object",
        "//implementation:global_string",
        "//implementation:id",
        "//implementation:id_type",
        "//implementation:jni_type",
        "//implementation:jvm",
        "//implementation:jvm_ref",
        "//implementation:loaded_by",
        "//implementation:local_array",
        "//implementation:local_array_string",
        "//implementation:local_class_loader",
        "//implementation:local_exception",
        "//implementation:local_object",
        "//implementation:local_string",
        "//implementation:method",
        "//implementation:no_idx",
        "//implementation:params",
        "//implementation:promotion_mechanics",
        "//implementation:promotion_mechanics_tags",
        "//implementation:proxy_convenience_aliases",
        "//implementation:proxy_temporary",
        "//implementation:ref_base",
        "//implementation:return",
        "//implementation:selector_static_info",
        "//implementation:self",
        "//implementation:static",
        "//implementation:static_ref",
        "//implementation:string_ref",
        "//implementation:supported_class_set",
        "//implementation:thread_guard",
        "//implementation/jni_helper",
        "//implementation/jni_helper:fake_test_constants",
        "//implementation/jni_helper:field_value_getter",
        "//implementation/jni_helper:invoke_static",
        "//implementation/jni_helper:jni_env",
        "//implementation/jni_helper:lifecycle",
        "//implementation/jni_helper:static_field_value",
        "//metaprogramming:corpus",
        "//metaprogramming:corpus_tag",
        "//metaprogramming:string_literal",
    ],
)

# Intentionally placed at root because of issues in Bazel.
# Note: This target is mangled for 3rd party usage on export.
# In the future, hopefully Google add Android support, although, unlikely.
cc_library(
    name = "jni_dep",
    srcs = ["@bazel_tools//tools/jdk:current_java_runtime"],
    hdrs = ["jni_dep.h"],
    includes =
        select({
            "@platforms//os:linux": ["external/local_jdk/include/linux"],
            "@platforms//os:macos": ["external/local_jdk/include/darwin"],
            "//conditions:default": [],
        }) + [
            "external/local_jdk/include",
        ],
    visibility = [":__subpackages__"],
    deps = ["@bazel_tools//tools/jdk:jni"],
)

################################################################################
# Testing Targets.
################################################################################
cc_library(
    name = "jni_test",
    testonly = 1,
    hdrs = ["jni_test.h"],
    tags = ["nozapfhahn"],
    visibility = [":__subpackages__"],
    deps = [
        ":jni_bind",
        ":mock_jni_env",
        ":mock_jvm",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "mock_jni_env",
    testonly = 1,
    hdrs = ["mock_jni_env.h"],
    tags = ["nozapfhahn"],
    visibility = [":__subpackages__"],
    deps = [
        "//:jni_dep",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "mock_jvm",
    testonly = 1,
    hdrs = ["mock_jvm.h"],
    tags = ["nozapfhahn"],
    visibility = [":__subpackages__"],
    deps = [
        "//:jni_dep",
        "@googletest//:gtest_main",
    ],
)

################################################################################
# Release targets.
################################################################################

# for_testing release header for easy adoption to 3P Customers.
# Strictly used for building a copy for the public, do not depend on this.
genrule(
    name = "gen_jni_bind_release",
    outs = ["jni_bind_release.h"],
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
    visibility = ["//third_party/jni_wrapper/godbolt:__pkg__"],
)

# Copy of the release header (above) that uses a different filename.
# Strangely, if a file is present with the same name as what's in outs blaze
# will ignore the genrule and depend on the real header, but it will fail
# because there is no cc_library providing the header.  jni_bind_release.h is
# checked in, so the above *will* fail.
genrule(
    name = "gen_jni_bind_release_for_testing",
    outs = ["jni_bind_release_for_testing.h"],
    cmd = './$(location build_jni_bind_release.sh) $(location jni_bind_release_input) jni_wrapper $(locations :jni_bind_decorative_text) | sed "s/__JNI_BIND_VERSION__/`cat $(locations JNI_BIND_VERSION.inc)`/" >$@',
    tools = [
        ":JNI_BIND_VERSION.inc",
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

genquery(
    name = "jni_bind_release_input",
    expression = "deps(//:jni_bind) except //:jni_dep except //:jni_bind.h",
    opts = ["--output=maxrank"],
    scope = [
        "//:jni_bind",
        "//:jni_dep",
    ],
)
