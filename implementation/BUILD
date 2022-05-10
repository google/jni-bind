package(
    default_visibility = ["//visibility:public"],
    licenses = ["notice"],
)

filegroup(
    name = "headers_for_export",
    srcs = glob(["**/*.h"]),
)

cc_library(
    name = "array",
    hdrs = ["array.h"],
    deps = [
        ":object",
        ":return",
        "//:jni_dep",
        "//metaprogramming:contains",
        "//metaprogramming:type_to_type_map",
    ],
)

cc_library(
    name = "array_ref",
    hdrs = ["array_ref.h"],
    deps = [
        ":array",
        ":array_view",
        ":class",
        ":default_class_loader",
        ":ref_base",
        "//:jni_dep",
        "//implementation/jni_helper:jni_array_helper",
    ],
)

cc_library(
    name = "array_view",
    hdrs = ["array_view.h"],
    deps = [
        "//:jni_dep",
        "//implementation/jni_helper:jni_array_helper",
    ],
)

cc_library(
    name = "local_array",
    hdrs = ["local_array.h"],
    deps = [
        ":array",
        ":array_ref",
        ":class",
        ":class_ref",
        ":default_class_loader",
        ":jvm",
        ":object",
        "//:jni_dep",
        "//implementation/jni_helper",
        "//implementation/jni_helper:jni_array_helper",
    ],
)

cc_test(
    name = "array_test",
    srcs = ["array_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "class",
    hdrs = ["class.h"],
    deps = [
        ":constructor",
        ":field",
        ":method",
        ":object",
        "//:jni_dep",
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
        ":jvm",
        ":method",
        "//:jni_dep",
        "//class_defs:java_lang_classes",
        "//implementation/jni_helper",
        "//implementation/jni_helper:jni_method_invoke",
        "//metaprogramming:double_locked_value",
    ],
)

cc_test(
    name = "class_test",
    srcs = ["class_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "//:mock_jni_env",
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
        "//metaprogramming:modified_max",
    ],
)

cc_test(
    name = "class_loader_test",
    srcs = ["class_loader_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "class_loader_ref",
    hdrs = [
        "class_loader_ref.h",
    ],
    deps = [
        ":class",
        ":class_loader",
        ":global_object",
        ":jvm_ref",
        ":local_object",
        ":method",
        ":method_ref",
        ":object_ref",
        ":params",
        ":return",
        ":string_ref",
        "//:jni_dep",
        "//class_defs:java_lang_classes",
        "//implementation/jni_helper:jni_env",
        "//metaprogramming:contains",
        "//metaprogramming:queryable_map",
    ],
)

cc_test(
    name = "class_loader_ref_test",
    srcs = ["class_loader_ref_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "//:mock_jni_env",
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
        "//implementation/jni_helper",
    ],
)

cc_test(
    name = "constructor_test",
    srcs = ["constructor_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "//:mock_jni_env",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "default_class_loader",
    hdrs = ["default_class_loader.h"],
    deps = ["//metaprogramming:modified_max"],
)

cc_library(
    name = "field",
    hdrs = ["field.h"],
    deps = [":params"],
)

cc_library(
    name = "field_ref",
    hdrs = ["field_ref.h"],
    deps = [
        ":class_ref",
        ":field_selection",
        ":proxy",
        "//:jni_dep",
        "//implementation/jni_helper",
        "//implementation/jni_helper:field_value_getter",
        "//metaprogramming:double_locked_value",
        "//metaprogramming:optional_wrap",
        "//metaprogramming:queryable_map",
    ],
)

cc_test(
    name = "field_ref_test",
    srcs = ["field_ref_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "//:mock_jni_env",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "field_selection",
    hdrs = ["field_selection.h"],
    deps = [
        ":name_constants",
        ":object",
        ":selector_static_info",
        "//implementation/jni_helper:jni_typename_to_string",
        "//metaprogramming:string_concatenate",
    ],
)

cc_library(
    name = "global_class_loader",
    hdrs = ["global_class_loader.h"],
    deps = [
        ":class_loader",
        ":class_loader_ref",
        ":jvm",
        "//:jni_dep",
        "//implementation/jni_helper",
    ],
)

cc_library(
    name = "global_object",
    hdrs = ["global_object.h"],
    deps = [
        ":class",
        ":local_object",
        ":object_ref",
        "//:jni_dep",
        "//implementation/jni_helper",
    ],
)

cc_test(
    name = "global_object_test",
    srcs = ["global_object_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "jvm",
    hdrs = ["jvm.h"],
    deps = [
        ":class",
        ":class_loader",
        "//:jni_dep",
        "//implementation/jni_helper:jni_env",
    ],
)

cc_library(
    name = "jvm_ref",
    hdrs = ["jvm_ref.h"],
    deps = [
        ":class_loader",
        ":class_ref",
        ":field_ref",
        ":jvm",
        ":method_ref",
        "//:jni_dep",
        "//metaprogramming:double_locked_value",
        "//metaprogramming:function_traits",
    ],
)

cc_test(
    name = "jvm_test",
    srcs = ["jvm_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "//implementation/jni_helper",
        "//implementation/jni_helper:jni_env",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "local_array_test",
    srcs = ["local_array_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "//:mock_jni_env",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "local_class_loader",
    hdrs = ["local_class_loader.h"],
    deps = [
        ":class_loader",
        ":class_loader_ref",
        ":jvm",
        ":jvm_ref",
        "//:jni_dep",
    ],
)

cc_library(
    name = "local_object",
    hdrs = ["local_object.h"],
    deps = [
        ":class",
        ":class_loader",
        ":jvm",
        ":jvm_ref",
        ":object_ref",
        "//:jni_dep",
        "//implementation/jni_helper",
    ],
)

cc_test(
    name = "local_object_test",
    srcs = ["local_object_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "//:mock_jni_env",
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
        ":method",
        ":params",
        ":proxy",
        ":ref_base",
        ":return",
        ":string",
        "//:jni_dep",
        "//implementation/jni_helper",
        "//implementation/jni_helper:jni_env",
        "//implementation/jni_helper:jni_method_invoke",
        "//metaprogramming:double_locked_value",
        "//metaprogramming:invocable_map",
        "//metaprogramming:optional_wrap",
        "//metaprogramming:string_concatenate",
    ],
)

cc_test(
    name = "method_ref_test",
    srcs = ["method_ref_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "//:mock_jni_env",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "method_selection",
    hdrs = ["method_selection.h"],
    deps = [
        ":array",
        ":array_ref",
        ":default_class_loader",
        ":method_ref",
        ":proxy",
        ":selector_static_info",
        "//implementation/jni_helper:jni_typename_to_string",
        "//metaprogramming:concatenate",
        "//metaprogramming:invoke",
        "//metaprogramming:n_bit_sequence",
        "//metaprogramming:tuple_manipulation",
        "//metaprogramming:type_index_mask",
        "//metaprogramming:type_of_nth_element",
    ],
)

cc_test(
    name = "method_selection_test",
    srcs = ["method_selection_test.cc"],
    deps = [
        "//:jni_bind",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "multi_type_test",
    srcs = ["multi_type_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "//:mock_jni_env",
        "//:mock_jvm",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "name_constants",
    hdrs = ["name_constants.h"],
)

cc_library(
    name = "object",
    hdrs = ["object.h"],
)

cc_library(
    name = "object_ref",
    hdrs = ["object_ref.h"],
    deps = [
        ":class",
        ":class_loader",
        ":class_ref",
        ":constructor",
        ":default_class_loader",
        ":field_ref",
        ":jvm_ref",
        ":method_ref",
        ":method_selection",
        ":proxy",
        ":ref_base",
        "//:jni_dep",
        "//implementation/jni_helper:jni_env",
        "//metaprogramming:invocable_map",
        "//metaprogramming:optional_wrap",
        "//metaprogramming:queryable_map",
        "//metaprogramming:tuple_manipulation",
    ],
)

cc_test(
    name = "overload_ref_test",
    srcs = ["overload_ref_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
        "//implementation/jni_helper",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "params",
    hdrs = ["params.h"],
    deps = [":object"],
)

cc_library(
    name = "proxy",
    hdrs = [
        "proxy.h",
        "proxy_definitions.h",
    ],
    deps = [
        ":array",
        ":array_ref",
        ":class",
        ":class_loader",
        ":default_class_loader",
        ":jvm",
        ":local_array",
        ":object",
        ":ref_base",
        ":string_ref",
        "//:jni_dep",
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
        "//:jni_bind",
        "//:jni_test",
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
        "//implementation/jni_helper:jni_env",
    ],
)

cc_library(
    name = "return",
    hdrs = ["return.h"],
)

cc_library(
    name = "string",
    hdrs = ["string.h"],
    deps = [
        ":class",
        ":ref_base",
        "//:jni_dep",
        "//implementation/jni_helper",
        "//implementation/jni_helper:jni_env",
    ],
)

cc_library(
    name = "selector_static_info",
    hdrs = ["selector_static_info.h"],
    deps = [
        ":array",
        ":name_constants",
        ":object",
        ":return",
        "//implementation/jni_helper:jni_typename_to_string",
        "//metaprogramming:string_concatenate",
    ],
)

cc_test(
    name = "selector_static_info_test",
    srcs = ["selector_static_info_test.cc"],
    deps = [
        "//:jni_bind",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "string_ref",
    hdrs = [
        "global_string.h",
        "local_string.h",
    ],
    deps = [
        ":ref_base",
        ":string",
        "//:jni_dep",
        "//implementation/jni_helper",
    ],
)

cc_test(
    name = "string_ref_test",
    srcs = ["string_ref_test.cc"],
    deps = [
        "//:jni_bind",
        "//:jni_test",
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