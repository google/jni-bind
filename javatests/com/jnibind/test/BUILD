licenses(["notice"])

################################################################################
# Array Test: Methods rank 1.
################################################################################
cc_library(
    name = "array_test_method_rank_1_jni_impl",
    testonly = 1,
    srcs = ["array_test_method_rank_1_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libarray_test_method_rank_1_jni.so",
    testonly = 1,
    linkshared = True,
    deps = [":array_test_method_rank_1_jni_impl"],
)

java_test(
    name = "ArrayTestMethodRank1",
    testonly = 1,
    srcs = ["ArrayTestMethodRank1.java"],
    data = [":libarray_test_method_rank_1_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
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
    testonly = 1,
    srcs = [
        "ClassLoaderTest.java",
    ],
    data = [
        "ClassLoaderHelperClass_deploy.jar",
        ":libclass_loader_test_jni.so",
    ],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
    deps = ["@maven//:com_google_truth_truth_1_1"],
)

################################################################################
# Array Test: Fields.
################################################################################
cc_library(
    name = "array_field_test_jni_impl",
    testonly = 1,
    srcs = ["array_field_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libarray_field_test_jni.so",
    testonly = 1,
    linkshared = True,
    deps = [":array_field_test_jni_impl"],
)

java_test(
    name = "ArrayFieldTest",
    testonly = 1,
    srcs = ["ArrayFieldTest.java"],
    data = [":libarray_field_test_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
    ],
)

################################################################################
# Array Test: Methods.
################################################################################
cc_library(
    name = "array_test_jni_impl",
    testonly = 1,
    srcs = ["array_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libarray_test_jni.so",
    testonly = 1,
    linkshared = True,
    deps = [":array_test_jni_impl"],
)

java_test(
    name = "ArrayTest",
    testonly = 1,
    srcs = ["ArrayTest.java"],
    data = [":libarray_test_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
    ],
)

################################################################################
# Context Test
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
    testonly = 1,
    srcs = ["ContextTest.java"],
    data = [":libcontext_test_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
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
    testonly = 1,
    srcs = ["field_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libfield_test_jni.so",
    testonly = 1,
    linkshared = True,
    deps = [":field_test_jni_impl"],
)

java_test(
    name = "FieldTest",
    testonly = 1,
    srcs = [
        "FieldTest.java",
        "FieldTestHelper.java",
    ],
    data = [":libfield_test_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
    deps = [
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
    ],
)

################################################################################
# Local Object Tests.
################################################################################
cc_library(
    name = "local_object_test_jni_impl",
    testonly = 1,
    srcs = ["local_object_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "liblocal_object_test_jni.so",
    testonly = 1,
    linkshared = True,
    deps = [":local_object_test_jni_impl"],
)

java_test(
    name = "LocalObjectTest",
    testonly = 1,
    srcs = ["LocalObjectTest.java"],
    data = [":liblocal_object_test_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
    ],
)

################################################################################
# Global Object Tests
################################################################################
cc_library(
    name = "global_object_test_jni_impl",
    testonly = 1,
    srcs = ["global_object_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libglobal_object_test_jni.so",
    testonly = 1,
    linkshared = True,
    deps = [":global_object_test_jni_impl"],
)

java_test(
    name = "GlobalObjectTest",
    testonly = 1,
    srcs = ["GlobalObjectTest.java"],
    data = [":libglobal_object_test_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
    ],
)

################################################################################
# Local Object Tests
################################################################################
cc_library(
    name = "local_object_test_jni_impl",
    testonly = 1,
    srcs = ["local_object_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "liblocal_object_test_jni.so",
    testonly = 1,
    linkshared = True,
    deps = [":local_object_test_jni_impl"],
)

java_test(
    name = "LocalObjectTest",
    testonly = 1,
    srcs = ["LocalObjectTest.java"],
    data = [":liblocal_object_test_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
    ],
)

################################################################################
# Method Test
################################################################################
cc_library(
    name = "method_test_jni_impl",
    testonly = 1,
    srcs = ["method_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libmethod_test_jni.so",
    testonly = 1,
    linkshared = True,
    deps = [":method_test_jni_impl"],
)

java_test(
    name = "MethodTest",
    testonly = 1,
    srcs = [
        "MethodTest.java",
        "MethodTestHelper.java",
    ],
    data = [":libmethod_test_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
    deps = [
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:junit_junit_4_13_1",
        "@maven//:org_mockito_mockito_core_2_28_2",
    ],
)

################################################################################
# ObjectTestHelper (used as a return value for testing).
################################################################################
java_library(
    name = "object_test_helper",
    srcs = ["ObjectTestHelper.java"],
)

#################################################################################
# String Test
#################################################################################
cc_library(
    name = "string_test_jni_impl",
    testonly = 1,
    srcs = ["string_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
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
    testonly = 1,
    srcs = ["static_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libstatic_test_jni.so",
    testonly = 1,
    linkshared = True,
    deps = [":static_test_jni_impl"],
)

java_test(
    name = "StaticTest",
    testonly = 1,
    srcs = [
        "StaticTest.java",
        "StaticTestHelper.java",
    ],
    data = [":libstatic_test_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
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
    testonly = 1,
    srcs = ["string_test_jni.cc"],
    deps = [
        ":object_test_helper_jni",
        "//:jni_bind",
    ],
    alwayslink = True,
)

cc_binary(
    name = "libstring_test_jni.so",
    testonly = 1,
    linkshared = True,
    deps = [":string_test_jni_impl"],
)

java_test(
    name = "StringTest",
    testonly = 1,
    srcs = [
        "StringTest.java",
        "StringTestHelper.java",
    ],
    data = [":libstring_test_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
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
    testonly = 1,
    srcs = ["ThreadTest.java"],
    data = [":libthread_test_jni.so"],
    jvm_flags = ["-Djava.library.path=./javatests/com/jnibind/test"],
    deps = [
        ":object_test_helper",
        "@maven//:com_google_truth_truth_1_1",
        "@maven//:org_mockito_mockito_core_2_28_2",
    ],
)
