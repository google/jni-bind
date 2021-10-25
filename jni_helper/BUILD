package(default_visibility = ["//visibility:public"])

licenses(["notice"])

exports_files(["LICENSE"])

cc_library(
    name = "field_value_getter",
    hdrs = ["field_value.h"],
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
        ":jni_helper",
        "//:jni_dep",
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
        "//:field",
        "//:jni_dep",
        "//:jni_test",
        "//:method",
        "//:mock_jni_env",
        "//:params",
        "//:return",
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
    name = "jni_method_invoke",
    hdrs = ["jni_method_invoke.h"],
    deps = [
        ":jni_env",
        "//:jni_dep",
    ],
)

cc_test(
    name = "jni_method_invoke_test",
    srcs = ["jni_method_invoke_test.cc"],
    deps = [
        ":jni_method_invoke",
        "//:jni_dep",
        "//:jni_test",
        "//:mock_jni_env",
        "@googletest//:gtest_main",
    ],
)
