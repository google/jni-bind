# Do not depend on any targets in this package.  Instead, depend //third_party/jni_wrapper:jni_bind
package(
    default_visibility = ["//implementation:__subpackages__"],
)

licenses(["notice"])

exports_files(["LICENSE"])

filegroup(
    name = "headers_for_export",
    srcs = glob(["**/*.h"]),
)

cc_library(
    name = "field_value_getter",
    hdrs = ["field_value.h"],
    deps = [
        ":jni_env",
        "//:jni_dep",
    ],
)

cc_library(
    name = "jni_array_helper",
    hdrs = ["jni_array_helper.h"],
    deps = [
        ":jni_env",
        "//:jni_dep",
    ],
)

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
        "//implementation:fake_test_constants",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "jni_helper",
    hdrs = ["jni_helper.h"],
    deps = [
        ":jni_env",
        "//:jni_dep",
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
        "//implementation:fake_test_constants",
        "@googletest//:gtest_main",
    ],
)

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

cc_library(
    name = "invoke",
    hdrs = ["invoke.h"],
    deps = [
        ":jni_env",
        "//:jni_dep",
    ],
)

cc_test(
    name = "invoke_test",
    srcs = ["invoke_test.cc"],
    deps = [
        ":invoke",
        "//:jni_dep",
        "//:jni_test",
        "//:mock_jni_env",
        "//implementation:fake_test_constants",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "invoke_static",
    hdrs = ["invoke_static.h"],
    deps = [
        ":invoke",
        ":jni_env",
        "//:jni_dep",
    ],
)

cc_library(
    name = "lifecycle",
    hdrs = ["lifecycle.h"],
    deps = [
        ":jni_env",
        "//:jni_dep",
    ],
)

cc_library(
    name = "lifecycle_object",
    hdrs = ["lifecycle_object.h"],
    deps = [
        ":jni_env",
        ":lifecycle",
        "//:jni_dep",
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

cc_library(
    name = "static_field_value",
    hdrs = ["static_field_value.h"],
    deps = [
        ":field_value_getter",
        ":jni_env",
        "//:jni_dep",
    ],
)

cc_library(
    name = "lifecycle_class",
    hdrs = ["lifecycle_class.h"],
    deps = [":lifecycle"],
)

cc_test(
    name = "lifecycle_class_test",
    srcs = ["lifecycle_class_test.cc"],
    deps = [
        ":lifecycle_class",
        "//:jni_bind",
        "//:jni_test",
        "@googletest//:gtest_main",
    ],
)
