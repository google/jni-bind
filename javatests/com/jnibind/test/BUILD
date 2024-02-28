licenses(["notice"])

################################################################################
# Array Test: Fields Rank 1.
################################################################################
cc_library(
    name = "array_test_field_rank_1_jni_impl",
    testonly = True,
    srcs = ["array_test_field_rank_1_jni.cc"],
    deps = [
        ":array_test_helpers_native",
        ":object_test_helper_jni",
        "//:jni_bind",
        "//metaprogramming:lambda_string",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libarray_test_field_rank_1_jni.so",
    testonly = True,
    linkshared = True,
    deps = [":array_test_field_rank_1_jni_impl"],
)

java_test(
    name = "ArrayTestFieldRank1",
    testonly = True,
    srcs = ["ArrayTestFieldRank1.java"],
    data = [":libarray_test_field_rank_1_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    runtime_deps = [
        ":array_test_helpers",
    ],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
    ],
)

################################################################################
# Array Test: Fields Rank 2.
################################################################################
cc_library(
    name = "array_test_field_rank_2_jni_impl",
    testonly = True,
    srcs = ["array_test_field_rank_2_jni.cc"],
    deps = [
        ":array_test_helpers_native",
        ":modulo",
        ":object_test_helper_jni",
        "//:jni_bind",
        "//metaprogramming:lambda_string",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libarray_test_field_rank_2_jni.so",
    testonly = True,
    linkshared = True,
    deps = [":array_test_field_rank_2_jni_impl"],
)

java_test(
    name = "ArrayTestFieldRank2",
    testonly = True,
    srcs = ["ArrayTestFieldRank2.java"],
    data = [":libarray_test_field_rank_2_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    runtime_deps = [
        ":array_test_helpers",
    ],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
    ],
)

################################################################################
# Array Test Helpers.
################################################################################
java_library(
    name = "array_test_helpers",
    testonly = True,
    srcs = ["ArrayTestHelpers.java"],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
    ],
)

cc_library(
    name = "array_test_helpers_native",
    hdrs = ["array_test_helpers_native.h"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
        "//implementation:method",
    ],
)

################################################################################
# Array Test: Methods Rank 1.
################################################################################
cc_library(
    name = "array_test_method_rank_1_jni_impl",
    testonly = True,
    srcs = ["array_test_method_rank_1_jni.cc"],
    deps = [
        ":array_test_helpers_native",
        ":object_test_helper_jni",
        "//:jni_bind",
        "//metaprogramming:lambda_string",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libarray_test_method_rank_1_jni.so",
    testonly = True,
    linkshared = True,
    deps = [":array_test_method_rank_1_jni_impl"],
)

java_test(
    name = "ArrayTestMethodRank1",
    testonly = True,
    srcs = ["ArrayTestMethodRank1.java"],
    data = [":libarray_test_method_rank_1_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    runtime_deps = [
        ":array_test_helpers",
    ],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
    ],
)

################################################################################
# Array Test: Methods Rank 2.
################################################################################
cc_library(
    name = "array_test_method_rank_2_jni_impl",
    testonly = True,
    srcs = ["array_test_method_rank_2_jni.cc"],
    deps = [
        ":array_test_helpers_native",
        ":modulo",
        ":object_test_helper_jni",
        "//:jni_bind",
        "//metaprogramming:lambda_string",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libarray_test_method_rank_2_jni.so",
    testonly = True,
    linkshared = True,
    deps = [":array_test_method_rank_2_jni_impl"],
)

java_test(
    name = "ArrayTestMethodRank2",
    testonly = True,
    srcs = ["ArrayTestMethodRank2.java"],
    data = [":libarray_test_method_rank_2_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    runtime_deps = [
        ":array_test_helpers",
    ],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
    ],
)

################################################################################
# Builder Test.
################################################################################
cc_library(
    name = "builder_jni_impl",
    testonly = True,
    srcs = ["builder_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
        "//metaprogramming:lambda_string",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libbuilder_jni.so",
    testonly = True,
    linkshared = True,
    deps = [":builder_jni_impl"],
)

java_test(
    name = "BuilderTest",
    testonly = True,
    srcs = ["BuilderTest.java"],
    data = [":libbuilder_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
    ],
)

################################################################################
# Classloader Test.
################################################################################
cc_library(
    name = "class_loader_test_jni_impl",
    srcs = ["class_loader_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libclass_loader_test_jni.so",
    linkshared = True,
    deps = [":class_loader_test_jni_impl"],
)

java_library(
    name = "ClassLoaderHelperClassImpl",
    srcs = [
        "ClassLoaderHelperClass.java",
    ],
)

java_binary(
    name = "ClassLoaderHelperClass",
    srcs = [
        "ClassLoaderHelperClass.java",
    ],
)

java_test(
    name = "ClassLoaderTest",
    testonly = True,
    srcs = [
        "ClassLoaderTest.java",
    ],
    data = [
        "ClassLoaderHelperClass_deploy.jar",
        ":libclass_loader_test_jni.so",
    ],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    deps = ["@maven//:com_google_truth_truth_1_1"],
)

################################################################################
# Context Test.
################################################################################
cc_library(
    name = "context_test_jni_impl",
    srcs = ["context_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libcontext_test_jni.so",
    linkshared = True,
    deps = [":context_test_jni_impl"],
)

java_test(
    name = "ContextTest",
    testonly = True,
    srcs = ["ContextTest.java"],
    data = [":libcontext_test_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
    ],
)

################################################################################
# Field Test.
################################################################################
cc_library(
    name = "field_test_jni_impl",
    testonly = True,
    srcs = ["field_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libfield_test_jni.so",
    testonly = True,
    linkshared = True,
    deps = [":field_test_jni_impl"],
)

java_test(
    name = "FieldTest",
    testonly = True,
    srcs = [
        "FieldTest.java",
        "FieldTestHelper.java",
    ],
    data = [":libfield_test_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    deps = [
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
    ],
)

################################################################################
# Global Object Tests.
################################################################################
cc_library(
    name = "global_object_test_jni_impl",
    testonly = True,
    srcs = ["global_object_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libglobal_object_test_jni.so",
    testonly = True,
    linkshared = True,
    deps = [":global_object_test_jni_impl"],
)

java_test(
    name = "GlobalObjectTest",
    testonly = True,
    srcs = ["GlobalObjectTest.java"],
    data = [":libglobal_object_test_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
    ],
)

################################################################################
# Local Object Tests.
################################################################################
cc_library(
    name = "local_object_test_jni_impl",
    testonly = True,
    srcs = ["local_object_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "liblocal_object_test_jni.so",
    testonly = True,
    linkshared = True,
    deps = [":local_object_test_jni_impl"],
)

java_test(
    name = "LocalObjectTest",
    testonly = True,
    srcs = ["LocalObjectTest.java"],
    data = [":liblocal_object_test_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
    ],
)

################################################################################
# Method Test.
################################################################################
cc_library(
    name = "method_test_jni_impl",
    testonly = True,
    srcs = ["method_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libmethod_test_jni.so",
    testonly = True,
    linkshared = True,
    deps = [":method_test_jni_impl"],
)

java_test(
    name = "MethodTest",
    testonly = True,
    srcs = [
        "MethodTest.java",
        "MethodTestHelper.java",
    ],
    data = [":libmethod_test_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    deps = [
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
        "@maven//:org_mockito_mockito_core_2_28_2",
    ],
)

################################################################################
# Modulo Helper.
################################################################################
cc_library(
    name = "modulo",
    testonly = True,
    hdrs = ["modulo.h"],
)

cc_test(
    name = "modulo_test",
    srcs = ["modulo_test.cc"],
    deps = [
        ":modulo",
        "@googletest//:gtest_main",
    ],
)

################################################################################
# Object Test Helper: Shared Object used as a return value for testing.
################################################################################
java_library(
    name = "object_test_helper",
    srcs = ["ObjectTestHelper.java"],
)

cc_library(
    name = "object_test_helper_jni",
    hdrs = ["object_test_helper_jni.h"],
    deps = ["//:jni_bind"],
)

#################################################################################
# Statics Test.
#################################################################################
cc_library(
    name = "static_test_jni_impl",
    testonly = True,
    srcs = ["static_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libstatic_test_jni.so",
    testonly = True,
    linkshared = True,
    deps = [":static_test_jni_impl"],
)

java_test(
    name = "StaticTest",
    testonly = True,
    srcs = [
        "StaticTest.java",
        "StaticTestHelper.java",
    ],
    data = [":libstatic_test_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
    ],
)

#################################################################################
# String Test.
#################################################################################
cc_library(
    name = "string_test_jni_impl",
    testonly = True,
    srcs = ["string_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libstring_test_jni.so",
    testonly = True,
    linkshared = True,
    deps = [":string_test_jni_impl"],
)

java_test(
    name = "StringTest",
    testonly = True,
    srcs = [
        "StringTest.java",
        "StringTestHelper.java",
    ],
    data = [":libstring_test_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    deps = [
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
        "@maven//:org_mockito_mockito_core_2_28_2",
    ],
)

################################################################################
# Thread Test.
################################################################################
cc_library(
    name = "thread_test_jni_impl",
    srcs = ["thread_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libthread_test_jni.so",
    linkshared = True,
    deps = [":thread_test_jni_impl"],
)

java_test(
    name = "ThreadTest",
    testonly = True,
    srcs = ["ThreadTest.java"],
    data = [":libthread_test_jni.so"],
    jvm_flags = [
        "-Djava.library.path=./javatests/com/jnibind/test",
        "-Djava.security.manager=allow",
    ],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:org_mockito_mockito_core_2_28_2",
    ],
)
