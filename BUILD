package(default_visibility = ["//visibility:public"])

licenses(["notice"])

exports_files(["LICENSE"])

cc_library(
    name = "class",
    hdrs = ["class.h"],
    deps = [
        ":constructor",
        ":field",
        ":jni_dep",
        ":method",
        ":object",
        "//jni_helper",
        "//metaprogramming:all_unique",
        "//metaprogramming:base_filter",
    ],
)

cc_library(
    name = "class_ref",
    hdrs = ["class_ref.h"],
    deps = [
        ":class",
        ":class_loader",
        ":jni_dep",
        ":jvm",
        ":method",
        "//class_defs:java_lang_classes",
        "//jni_helper",
        "//jni_helper:jni_method_invoke",
        "//metaprogramming:double_locked_value",
    ],
)

cc_test(
    name = "class_test",
    srcs = ["class_test.cc"],
    deps = [
        ":class",
        ":jni_dep",
        ":jni_test",
        ":mock_jni_env",
        ":object_ref",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "class_loader",
    hdrs = ["class_loader.h"],
    deps = [
        ":class",
        ":default_class_loader",
        ":supported_class_set",
        "//class_defs:java_lang_classes",
        "//metaprogramming:all_unique",
        "//metaprogramming:base_filter",
    ],
)

cc_test(
    name = "class_loader_test",
    srcs = ["class_loader_test.cc"],
    deps = [
        ":class",
        ":class_loader",
        ":class_loader_ref",
        ":jni_test",
        ":supported_class_set",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "class_loader_ref",
    hdrs = [
        "class_loader_ref.h",
        "global_class_loader.h",
        "local_class_loader.h",
    ],
    deps = [
        ":class",
        ":class_loader",
        ":jni_dep",
        ":jvm",
        ":jvm_ref",
        ":method",
        ":method_ref",
        ":object_ref",
        ":params",
        ":return",
        ":string_ref",
        "//class_defs:java_lang_classes",
        "//jni_helper",
        "//jni_helper:jni_env",
        "//metaprogramming:contains",
        "//metaprogramming:queryable_map",
    ],
)

cc_test(
    name = "class_loader_ref_test",
    srcs = ["class_loader_ref_test.cc"],
    deps = [
        ":class",
        ":class_loader",
        ":class_loader_ref",
        ":jni_dep",
        ":jni_test",
        ":jvm",
        ":jvm_ref",
        ":method",
        ":method_ref",
        ":mock_jni_env",
        ":params",
        ":return",
        ":supported_class_set",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "constructor",
    hdrs = ["constructor.h"],
    deps = [
        ":method",
        ":params",
        ":return",
        "//jni_helper",
    ],
)

cc_test(
    name = "constructor_test",
    srcs = ["constructor_test.cc"],
    deps = [
        ":class",
        ":constructor",
        ":jni_dep",
        ":jni_test",
        ":mock_jni_env",
        ":object_ref",
        ":params",
        ":return",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "default_class_loader",
    hdrs = ["default_class_loader.h"],
)

cc_library(
    name = "field",
    hdrs = ["field.h"],
    deps = [
        ":params",
        ":signature",
    ],
)

cc_library(
    name = "field_ref",
    hdrs = ["field_ref.h"],
    deps = [
        ":class_ref",
        ":field",
        ":jni_dep",
        "//jni_helper",
        "//jni_helper:field_value_getter",
        "//metaprogramming:double_locked_value",
        "//metaprogramming:optional_wrap",
        "//metaprogramming:queryable_map",
    ],
)

cc_test(
    name = "field_ref_test",
    srcs = ["field_ref_test.cc"],
    deps = [
        ":class",
        ":field",
        ":field_ref",
        ":jni_dep",
        ":jni_test",
        ":mock_jni_env",
        ":object_ref",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "global_object_test",
    srcs = ["global_object_test.cc"],
    deps = [
        ":class",
        ":field",
        ":field_ref",
        ":jni_dep",
        ":jni_test",
        ":jvm_ref",
        ":method",
        ":method_ref",
        ":mock_jni_env",
        ":object_ref",
        ":params",
        ":return",
        "@googletest//:gtest_main",
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

cc_library(
    name = "jni_test",
    testonly = 1,
    hdrs = ["jni_test.h"],
    tags = ["nozapfhahn"],
    visibility = ["//visibility:public"],
    deps = [
        ":jni_dep",
        ":jvm",
        ":jvm_ref",
        ":mock_jni_env",
        ":mock_jvm",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "jni_type_proxy",
    hdrs = ["jni_type_proxy.h"],
    deps = [
        ":jni_dep",
        ":object",
        ":ref_base",
        "//jni_helper:jni_typename_to_string",
    ],
)

cc_test(
    name = "jni_type_proxy_test",
    srcs = ["jni_type_proxy_test.cc"],
    deps = [
        ":class",
        ":jni_dep",
        ":jni_test",
        ":jni_type_proxy",
        ":mock_jni_env",
        ":object_ref",
        "//jni_helper",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "jvm",
    hdrs = ["jvm.h"],
    deps = [
        ":class",
        ":class_loader",
        ":jni_dep",
        "//jni_helper:jni_env",
    ],
)

cc_library(
    name = "jvm_ref",
    hdrs = ["jvm_ref.h"],
    deps = [
        ":class_loader",
        ":class_ref",
        ":field_ref",
        ":jni_dep",
        ":jvm",
        ":method_ref",
        "//metaprogramming:double_locked_value",
        "//metaprogramming:function_traits",
    ],
)

cc_test(
    name = "jvm_test",
    srcs = ["jvm_test.cc"],
    deps = [
        ":class",
        ":class_loader",
        ":class_loader_ref",
        ":jni_dep",
        ":jni_test",
        ":jvm",
        ":jvm_ref",
        ":method",
        ":mock_jni_env",
        ":mock_jvm",
        ":object_ref",
        ":params",
        ":return",
        ":supported_class_set",
        "//jni_helper",
        "//jni_helper:jni_env",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "local_object_test",
    srcs = ["local_object_test.cc"],
    deps = [
        ":class",
        ":class_loader",
        ":field",
        ":field_ref",
        ":jni_dep",
        ":jni_test",
        ":jvm_ref",
        ":method",
        ":method_ref",
        ":mock_jni_env",
        ":object_ref",
        ":params",
        ":return",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "method",
    hdrs = ["method.h"],
    deps = [":params"],
)

cc_library(
    name = "method_ref",
    hdrs = ["method_ref.h"],
    deps = [
        ":class_loader",
        ":class_ref",
        ":jni_dep",
        ":jni_type_proxy",
        ":method",
        ":method_selection",
        ":params",
        ":proxy",
        ":ref_base",
        ":return",
        ":signature",
        ":string",
        "//jni_helper",
        "//jni_helper:jni_env",
        "//jni_helper:jni_method_invoke",
        "//metaprogramming:double_locked_value",
        "//metaprogramming:invocable_map",
        "//metaprogramming:optional_wrap",
    ],
)

cc_test(
    name = "method_ref_test",
    srcs = ["method_ref_test.cc"],
    deps = [
        ":class",
        ":jni_dep",
        ":jni_test",
        ":method",
        ":method_ref",
        ":method_selection",
        ":mock_jni_env",
        ":object_ref",
        ":params",
        ":return",
        ":string_ref",
        "//jni_helper",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "method_selection",
    hdrs = ["method_selection.h"],
    deps = [
        ":default_class_loader",
        ":proxy",
        "//metaprogramming:concatenate",
        "//metaprogramming:invoke",
        "//metaprogramming:n_bit_sequence",
        "//metaprogramming:type_index_mask",
        "//metaprogramming:type_of_nth_element",
    ],
)

cc_test(
    name = "method_selection_test",
    srcs = ["method_selection_test.cc"],
    deps = [
        ":class",
        ":class_loader",
        ":default_class_loader",
        ":method",
        ":method_selection",
        ":object_ref",
        ":params",
        ":ref_base",
        ":return",
        ":supported_class_set",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "mock_jni_env",
    testonly = 1,
    hdrs = ["mock_jni_env.h"],
    tags = ["nozapfhahn"],
    visibility = ["//visibility:public"],
    deps = [
        ":jni_dep",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "mock_jvm",
    testonly = 1,
    hdrs = ["mock_jvm.h"],
    tags = ["nozapfhahn"],
    visibility = ["//visibility:public"],
    deps = [
        ":jni_dep",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "multi_type_test",
    srcs = ["multi_type_test.cc"],
    deps = [
        ":class",
        ":class_loader",
        ":field",
        ":field_ref",
        ":jni_dep",
        ":jni_test",
        ":jvm",
        ":method",
        ":method_ref",
        ":mock_jni_env",
        ":mock_jvm",
        ":object_ref",
        ":params",
        ":return",
        ":supported_class_set",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "object",
    hdrs = ["object.h"],
)

cc_library(
    name = "object_ref",
    hdrs = [
        "global_object.h",
        "local_object.h",
        "object_ref.h",
    ],
    deps = [
        ":class",
        ":class_loader",
        ":class_ref",
        ":constructor",
        ":field_ref",
        ":jni_dep",
        ":jni_type_proxy",
        ":jvm",
        ":jvm_ref",
        ":method_ref",
        ":method_selection",
        ":ref_base",
        "//jni_helper",
        "//jni_helper:jni_env",
        "//metaprogramming:invocable_map",
        "//metaprogramming:optional_wrap",
        "//metaprogramming:queryable_map",
        "//metaprogramming:tuple_manipulation",
    ],
)

cc_library(
    name = "params",
    hdrs = ["params.h"],
    deps = [
        ":object",
        ":signature",
        "//jni_helper:jni_typename_to_string",
    ],
)

cc_test(
    name = "params_test",
    srcs = ["params_test.cc"],
    deps = [
        ":class",
        ":jni_type_proxy",
        ":mock_jni_env",
        ":object_ref",
        ":params",
        ":string",
        ":string_ref",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "proxy",
    hdrs = [
        "proxy.h",
        "proxy_definitions.h",
    ],
    deps = [
        ":class",
        ":class_loader",
        ":jni_dep",
        ":jvm",
        ":object",
        ":ref_base",
        ":string_ref",
        "//metaprogramming:cartesian_product",
        "//metaprogramming:combine",
        "//metaprogramming:concatenate",
        "//metaprogramming:contains",
        "//metaprogramming:flatten",
        "//metaprogramming:invoke",
        "//metaprogramming:per_element",
        "//metaprogramming:reduce",
        "//metaprogramming:tuple_manipulation",
        "//metaprogramming:type_of_nth_element",
        "//metaprogramming:type_to_type_map",
        "//metaprogramming:unique_set",
        "//metaprogramming:unwrap",
    ],
)

cc_test(
    name = "proxy_test",
    srcs = ["proxy_test.cc"],
    deps = [
        ":class",
        ":class_loader",
        ":jni_dep",
        ":jni_test",
        ":jvm",
        ":method",
        ":method_selection",
        ":object_ref",
        ":params",
        ":proxy",
        ":string_ref",
        ":supported_class_set",
        "//metaprogramming:concatenate",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "ref_base",
    hdrs = ["ref_base.h"],
    deps = [
        ":class",
        ":class_loader",
        ":default_class_loader",
        "//jni_helper:jni_env",
    ],
)

cc_library(
    name = "return",
    hdrs = ["return.h"],
    deps = [
        ":signature",
    ],
)

cc_library(
    name = "signature",
    hdrs = ["signature.h"],
    deps = [
        ":object",
        "//jni_helper:jni_typename_to_string",
    ],
)

cc_test(
    name = "signature_test",
    srcs = ["signature_test.cc"],
    deps = [
        ":class",
        ":mock_jni_env",
        ":object_ref",
        ":params",
        ":signature",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "string",
    hdrs = ["string.h"],
    deps = [
        ":class",
        ":jni_dep",
        ":ref_base",
        "//jni_helper",
        "//jni_helper:jni_env",
    ],
)

cc_library(
    name = "string_ref",
    hdrs = [
        "global_string.h",
        "local_string.h",
    ],
    deps = [
        ":jni_dep",
        ":jni_type_proxy",
        ":ref_base",
        ":string",
        "//jni_helper",
    ],
)

cc_test(
    name = "string_ref_test",
    srcs = ["string_ref_test.cc"],
    deps = [
        ":class",
        ":jni_dep",
        ":jni_test",
        ":method",
        ":mock_jni_env",
        ":object_ref",
        ":params",
        ":string_ref",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "supported_class_set",
    hdrs = ["supported_class_set.h"],
    deps = [
        ":class",
        "//metaprogramming:all_unique",
    ],
)
