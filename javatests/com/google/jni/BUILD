load("//tools/build_defs/android:rules.bzl", "android_binary", "android_instrumentation_test", "android_library")
load("//mobile/build:build_defs.bzl", "android_jni_library")

licenses(["notice"])

################################################################################
# Shared Object used as a return value for testing.
################################################################################
android_library(
    name = "object_test_helper",
    testonly = 1,
    srcs = ["ObjectTestHelper.java"],
)

cc_library(
    name = "object_test_helper_jni",
    hdrs = ["object_test_helper_jni.h"],
    deps = [
        "//:class",
        "//:field",
        "//:method",
        "//:object",
        "//:params",
        "//:return",
    ],
)

################################################################################
# Context Test
################################################################################
android_jni_library(
    name = "context_test_jni",
    testonly = 1,
    srcs = ["context_test_jni.cc"],
    force_android = True,
    deps = [
        ":object_test_helper_jni",
        "//:class",
        "//:field",
        "//:jni_dep",
        "//:jvm",
        "//:jvm_ref",
        "//:method",
        "//:object_ref",
        "//:params",
        "//:return",
        "//:string_ref",
    ],
)

android_library(
    name = "context_test_java_lib",
    testonly = 1,
    srcs = ["ContextTest.java"],
    deps = [
        ":object_test_helper",
        "//third_party/android/androidx_test/runner/android_junit_runner",
        "//third_party/java/junit:junit-android",
        "//third_party/java/truth:truth-android",
    ],
)

android_binary(
    name = "context_test_app",
    testonly = 1,
    manifest = "AndroidDefaultManifest.xml",
    multidex = "native",
    deps = [
        ":context_test_java_lib",
        ":context_test_jni",
        "//java/com/google/android/apps/common/testing/testrunner",
    ],
)

android_instrumentation_test(
    name = "context_instrumentation_test",
    target_device = "//tools/android/emulated_devices/generic_phone:android_26_x86",
    test_app = ":context_test_app",
)

################################################################################
# RJni Local Object Tests
################################################################################
android_jni_library(
    name = "local_object_test_jni",
    testonly = 1,
    srcs = ["local_object_test_jni.cc"],
    force_android = True,
    deps = [
        ":object_test_helper_jni",
        "//:class",
        "//:class_loader",
        "//:class_loader_ref",
        "//:constructor",
        "//:field",
        "//:jni_dep",
        "//:jvm_ref",
        "//:object",
        "//:object_ref",
        "//:params",
        "//:return",
        "//:string_ref",
    ],
)

android_library(
    name = "object_test_local_java_lib",
    testonly = 1,
    srcs = [
        "LocalObjectTest.java",
        "ObjectTestHelper.java",
    ],
    deps = [
        ":local_object_test_jni",
        "//java/com/google/android/apps/common/proguard:annotations",
        "//third_party/android/androidx_test/runner/android_junit_runner",
        "//third_party/java/junit:junit-android",
        "//third_party/java/truth:truth-android",
    ],
)

android_binary(
    name = "object_test_local_app",
    testonly = 1,
    manifest = "AndroidDefaultManifest.xml",
    multidex = "native",
    deps = [
        ":local_object_test_jni",
        ":object_test_local_java_lib",
        "//java/com/google/android/apps/common/testing/testrunner",
    ],
)

android_instrumentation_test(
    name = "object_local_instrumentation_test",
    target_device = "//tools/android/emulated_devices/generic_phone:android_26_x86",
    test_app = ":object_test_local_app",
)

################################################################################
# RJni Global Object Tests
################################################################################
android_jni_library(
    name = "global_object_test_jni",
    testonly = 1,
    srcs = ["global_object_test_jni.cc"],
    force_android = True,
    deps = [
        ":object_test_helper_jni",
        "//:class",
        "//:field",
        "//:jni_dep",
        "//:jvm_ref",
        "//:object",
        "//:object_ref",
        "//:params",
        "//:return",
        "//:string_ref",
    ],
)

android_library(
    name = "object_test_global_java_lib",
    testonly = 1,
    srcs = [
        "GlobalObjectTest.java",
        "ObjectTestHelper.java",
    ],
    deps = [
        ":global_object_test_jni",
        "//java/com/google/android/apps/common/proguard:annotations",
        "//third_party/android/androidx_test/runner/android_junit_runner",
        "//third_party/java/junit:junit-android",
        "//third_party/java/truth:truth-android",
    ],
)

android_binary(
    name = "object_test_global_app",
    testonly = 1,
    manifest = "AndroidDefaultManifest.xml",
    multidex = "native",
    deps = [
        ":global_object_test_jni",
        ":object_test_global_java_lib",
        "//java/com/google/android/apps/common/testing/testrunner",
    ],
)

android_instrumentation_test(
    name = "object_global_instrumentation_test",
    target_device = "//tools/android/emulated_devices/generic_phone:android_26_x86",
    test_app = ":object_test_global_app",
)

################################################################################
# RJni Field Test
################################################################################
android_jni_library(
    name = "field_test_jni",
    testonly = 1,
    srcs = ["field_test_jni.cc"],
    force_android = True,
    deps = [
        ":object_test_helper_jni",
        "//:class",
        "//:field",
        "//:jni_dep",
        "//:jvm_ref",
        "//:object_ref",
        "//:params",
        "//:return",
        "//:string_ref",
    ],
)

android_library(
    name = "field_test_java_lib",
    testonly = 1,
    srcs = [
        "FieldTest.java",
        "FieldTestHelper.java",
    ],
    deps = [
        "//java/com/google/android/apps/common/proguard:annotations",
        "//third_party/android/androidx_test/runner/android_junit_runner",
        "//third_party/java/junit:junit-android",
        "//third_party/java/truth:truth-android",
    ],
)

android_binary(
    name = "field_test_app",
    testonly = 1,
    manifest = "AndroidDefaultManifest.xml",
    multidex = "native",
    deps = [
        ":field_test_java_lib",
        ":field_test_jni",
        "//java/com/google/android/apps/common/testing/testrunner",
    ],
)

android_instrumentation_test(
    name = "field_instrumentation_test",
    target_device = "//tools/android/emulated_devices/generic_phone:android_26_x86",
    test_app = ":field_test_app",
)

################################################################################
# RJni Method Test
################################################################################
android_jni_library(
    name = "method_test_jni",
    testonly = 1,
    srcs = ["method_test_jni.cc"],
    force_android = True,
    deps = [
        ":object_test_helper_jni",
        "//:class",
        "//:field",
        "//:jni_dep",
        "//:jvm_ref",
        "//:method",
        "//:object_ref",
        "//:params",
        "//:return",
        "//:string_ref",
    ],
)

android_library(
    name = "method_test_java_lib",
    testonly = 1,
    srcs = [
        "MethodTest.java",
        "MethodTestHelper.java",
    ],
    deps = [
        "//java/com/google/android/apps/common/proguard:annotations",
        "//third_party/android/androidx_test/runner/android_junit_runner",
        "//third_party/java/junit:junit-android",
        "//third_party/java/mockito:mockito-android",
        "//third_party/java/truth:truth-android",
    ],
)

android_binary(
    name = "method_test_app",
    testonly = 1,
    manifest = "AndroidDefaultManifest.xml",
    multidex = "native",
    deps = [
        ":method_test_java_lib",
        ":method_test_jni",
        "//java/com/google/android/apps/common/testing/testrunner",
    ],
)

android_instrumentation_test(
    name = "method_instrumentation_test",
    target_device = "//tools/android/emulated_devices/generic_phone:android_26_x86",
    test_app = ":method_test_app",
)

################################################################################
# RJni String Test
################################################################################
android_jni_library(
    name = "string_test_jni",
    testonly = 1,
    srcs = ["string_test_jni.cc"],
    force_android = True,
    deps = [
        ":object_test_helper_jni",
        "//:class",
        "//:field",
        "//:jni_dep",
        "//:jvm_ref",
        "//:object_ref",
        "//:params",
        "//:return",
        "//:string",
        "//:string_ref",
    ],
)

android_library(
    name = "string_test_java_lib",
    testonly = 1,
    srcs = [
        "StringTest.java",
        "StringTestHelper.java",
    ],
    deps = [
        "//java/com/google/android/apps/common/proguard:annotations",
        "//third_party/android/androidx_test/runner/android_junit_runner",
        "//third_party/java/junit:junit-android",
        "//third_party/java/mockito:mockito-android",
        "//third_party/java/truth:truth-android",
    ],
)

android_binary(
    name = "string_test_app",
    testonly = 1,
    manifest = "AndroidDefaultManifest.xml",
    multidex = "native",
    deps = [
        ":string_test_java_lib",
        ":string_test_jni",
        "//java/com/google/android/apps/common/testing/testrunner",
    ],
)

android_instrumentation_test(
    name = "string_instrumentation_test",
    target_device = "//tools/android/emulated_devices/generic_phone:android_26_x86",
    test_app = ":string_test_app",
)

################################################################################
# RJni ClassLoader Tests
################################################################################
android_jni_library(
    name = "class_loader_test_jni",
    testonly = 1,
    srcs = ["class_loader_test_jni.cc"],
    force_android = True,
    deps = [
        ":object_test_helper_jni",
        "//:class",
        "//:class_loader",
        "//:class_loader_ref",
        "//:constructor",
        "//:field",
        "//:jni_dep",
        "//:jvm",
        "//:jvm_ref",
        "//:object",
        "//:object_ref",
        "//:params",
        "//:return",
        "//:string_ref",
    ],
)

android_library(
    name = "class_loader_test_local_java_lib",
    testonly = 1,
    srcs = [
        "ClassLoaderTest.java",
        "ObjectTestHelper.java",
    ],
    deps = [
        "//java/com/google/android/apps/common/proguard:annotations",
        "//third_party/android/androidx_test/core",
        "//third_party/android/androidx_test/runner/android_junit_runner",
        "//third_party/android/androidx_test/services/storage",
        "//third_party/java/junit:junit-android",
        "//third_party/java/truth:truth-android",
    ],
)

android_binary(
    name = "class_loader_helpers",
    testonly = 1,
    srcs = [
        "ClassLoaderHelperClass.java",
        "ClassLoaderHelperSubclass.java",
    ],
    manifest = "AndroidDefaultManifest.xml",
    multidex = "native",
)

android_binary(
    name = "class_loader_test_local_app",
    testonly = 1,
    manifest = "AndroidDefaultManifest.xml",
    multidex = "native",
    deps = [
        ":class_loader_test_jni",
        ":class_loader_test_local_java_lib",
        "//java/com/google/android/apps/common/testing/testrunner",
    ],
)

android_instrumentation_test(
    name = "class_loader_local_instrumentation_test",
    args = [
        "--push_apks_to_sdcard",
        "--test_args=remote_class_path=%s" % "$(location :class_loader_helpers.apk)",
    ],
    data = [":class_loader_helpers.apk"],
    target_device = "//tools/android/emulated_devices/generic_phone:android_26_x86",
    test_app = ":class_loader_test_local_app",
)
