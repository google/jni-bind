# Do not depend on any targets in this package.  Instead, depend //third_party/jni_wrapper:jni_bind
package(
    default_visibility = ["//:__subpackages__"],
)

licenses(["notice"])

exports_files(["LICENSE"])

################################################################################
# ArgPrint.
################################################################################
cc_library(
    name = "arg_string",
    hdrs = ["arg_string.h"],
    deps = ["//:jni_dep"],
)

cc_test(
    name = "arg_string_test",
    srcs = ["arg_string_test.cc"],
    deps = [
        ":arg_string",
        "//:jni_dep",
        "@googletest//:gtest_main",
    ],
)

################################################################################
# DryRun.
################################################################################
cc_test(
    name = "dry_run_test",
    srcs = ["dry_run_test.cc"],
    defines = [
        "DRY_RUN",
        "ENABLE_DEBUG_OUTPUT",
    ],
    deps = [
        "//:jni_bind",
        "@googletest//:gtest_main",
    ],
)

################################################################################
# Fake Test Constants.
################################################################################
cc_library(
    name = "fake_test_constants",
    hdrs = ["fake_test_constants.h"],
    deps = [
        ":get_array_element_result",
        "//:jni_dep",
    ],
)

################################################################################
# FieldValue.
################################################################################
cc_library(
    name = "field_value_getter",
    hdrs = ["field_value.h"],
    deps = [
        ":jni_env",
        ":trace",
        "//:jni_dep",
        "//metaprogramming:lambda_string",
    ],
)

################################################################################
# GetArrayResult.
################################################################################
cc_library(
    name = "get_array_element_result",
    hdrs = ["get_array_element_result.h"],
    deps = ["//:jni_dep"],
)

################################################################################
# Headers for export.
################################################################################
filegroup(
    name = "headers_for_export",
    srcs = glob(["**/*.h"]),
    visibility = ["//visibility:public"],
)

################################################################################
# Invoke.
################################################################################
cc_library(
    name = "invoke",
    hdrs = ["invoke.h"],
    deps = [
        ":jni_env",
        ":trace",
        "//:jni_dep",
        "//metaprogramming:lambda_string",
    ],
)

cc_test(
    name = "invoke_test",
    srcs = ["invoke_test.cc"],
    defines = [
        "ENABLE_DEBUG_OUTPUT",
    ],
    deps = [
        ":invoke",
        "//:jni_dep",
        "//:jni_test",
        "//:mock_jni_env",
        "//implementation/jni_helper:fake_test_constants",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "invoke_static",
    hdrs = ["invoke_static.h"],
    deps = [
        ":invoke",
        ":jni_env",
        ":trace",
        "//:jni_dep",
        "//metaprogramming:lambda_string",
    ],
)

################################################################################
# JniArrayHelper.
################################################################################
cc_library(
    name = "jni_array_helper",
    hdrs = ["jni_array_helper.h"],
    deps = [
        ":get_array_element_result",
        ":jni_env",
        ":trace",
        "//:jni_dep",
        "//metaprogramming:lambda_string",
    ],
)

################################################################################
# JniEnv.
################################################################################
cc_library(
    name = "jni_env",
    hdrs = ["jni_env.h"],
    # Visibility will be constrained after SetEnv is done by JvmRef only.
    visibility = ["//visibility:public"],
    deps = ["//:jni_dep"],
)

cc_test(
    name = "jni_env_test",
    srcs = ["jni_env_test.cc"],
    deps = [
        ":jni_env",
        "//:jni_dep",
        "//implementation/jni_helper:fake_test_constants",
        "@googletest//:gtest_main",
    ],
)

################################################################################
# JniHelper.
################################################################################
cc_library(
    name = "jni_helper",
    hdrs = ["jni_helper.h"],
    deps = [
        ":fake_test_constants",
        ":jni_env",
        ":trace",
        "//:jni_dep",
        "//metaprogramming:lambda_string",
    ],
)

cc_test(
    name = "jni_helper_test",
    srcs = ["jni_helper_test.cc"],
    deps = [
        ":jni_helper",
        ":lifecycle_object",
        "//:jni_bind",
        "//:jni_dep",
        "//:jni_test",
        "//:mock_jni_env",
        "//implementation/jni_helper:fake_test_constants",
        "@googletest//:gtest_main",
    ],
)

################################################################################
# JniTypenameToString.
################################################################################
cc_library(
    name = "jni_typename_to_string",
    hdrs = ["jni_typename_to_string.h"],
    deps = ["//:jni_dep"],
)

cc_test(
    name = "jni_typename_to_string_test",
    srcs = ["jni_typename_to_string_test.cc"],
    deps = [
        ":jni_typename_to_string",
        "//:jni_dep",
        "@googletest//:gtest_main",
    ],
)

################################################################################
# Lifecycle.
################################################################################
cc_library(
    name = "lifecycle",
    hdrs = ["lifecycle.h"],
    deps = [
        ":fake_test_constants",
        ":jni_env",
        ":trace",
        "//:jni_dep",
        "//metaprogramming:lambda_string",
    ],
)

cc_library(
    name = "lifecycle_object",
    hdrs = ["lifecycle_object.h"],
    deps = [
        ":lifecycle",
        ":trace",
        "//metaprogramming:lambda_string",
    ],
)

cc_test(
    name = "lifecycle_object_test",
    srcs = ["lifecycle_object_test.cc"],
    deps = [
        ":lifecycle_object",
        "//:jni_bind",
        "//:jni_test",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "lifecycle_string",
    hdrs = ["lifecycle_string.h"],
    deps = [
        ":lifecycle",
        ":lifecycle_object",
        ":trace",
        "//metaprogramming:lambda_string",
    ],
)

cc_test(
    name = "lifecycle_string_test",
    srcs = ["lifecycle_string_test.cc"],
    deps = [
        ":lifecycle_string",
        "//:jni_bind",
        "//:jni_test",
        "@googletest//:gtest_main",
    ],
)

################################################################################
# Static field.
################################################################################
cc_library(
    name = "static_field_value",
    hdrs = ["static_field_value.h"],
    deps = [
        ":field_value_getter",
        ":jni_env",
        ":trace",
        "//:jni_dep",
        "//metaprogramming:lambda_string",
    ],
)

################################################################################
# Trace.
################################################################################
cc_library(
    name = "trace",
    hdrs = ["trace.h"],
    deps = [
        ":arg_string",
        "//metaprogramming:color",
        "//metaprogramming:lambda_string",
    ],
)
