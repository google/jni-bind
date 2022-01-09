package(default_visibility = ["//visibility:public"])

licenses(["notice"])

exports_files(["LICENSE"])

filegroup(
    name = "headers_for_export",
    srcs = glob(["**/*.h"]),
)

cc_library(
    name = "all",
    hdrs = ["all.h"],
    deps = [":tuple_manipulation"],
)

cc_test(
    name = "all_test",
    srcs = ["all_test.cc"],
    deps = [
        ":all",
        ":invoke",
        ":same",
        ":unwrap",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "all_unique",
    hdrs = ["all_unique.h"],
    deps = [":contains"],
)

cc_test(
    name = "all_unique_test",
    srcs = ["all_unique_test.cc"],
    deps = [
        ":all_unique",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "any",
    hdrs = ["any.h"],
    deps = [":tuple_manipulation"],
)

cc_test(
    name = "any_test",
    srcs = ["any_test.cc"],
    deps = [
        ":any",
        ":invoke",
        ":same",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "apply",
    hdrs = ["apply.h"],
    deps = [
        ":invoke",
        ":tuple_manipulation",
    ],
)

cc_test(
    name = "apply_test",
    srcs = ["apply_test.cc"],
    deps = [
        ":apply",
        ":concatenate",
        ":invoke",
        ":same",
        ":tuple_manipulation",
        ":type_of_nth_element",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "base",
    hdrs = ["base.h"],
)

cc_library(
    name = "base_filter",
    hdrs = ["base_filter.h"],
    deps = [":concatenate"],
)

cc_test(
    name = "base_filter_test",
    srcs = ["base_filter_test.cc"],
    deps = [
        ":base_filter",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "cartesian_product",
    hdrs = ["cartesian_product.h"],
    deps = [
        ":concatenate",
        ":n_bit",
        ":n_bit_sequence",
        ":type_index_mask",
        ":type_of_nth_element",
    ],
)

cc_test(
    name = "cartesian_product_test",
    srcs = ["cartesian_product_test.cc"],
    deps = [
        ":cartesian_product",
        ":invoke",
        ":n_bit",
        ":n_bit_sequence",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "chain",
    hdrs = ["chain.h"],
    deps = [
        ":invoke",
        ":tuple_manipulation",
    ],
)

cc_test(
    name = "chain_test",
    srcs = ["chain_test.cc"],
    deps = [
        ":chain",
        ":increment",
        ":invoke",
        ":same",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "concatenate",
    hdrs = ["concatenate.h"],
    deps = [
        ":invoke",
        ":per_element",
        ":reduce",
        ":tuple_manipulation",
    ],
)

cc_test(
    name = "concatenate_test",
    srcs = ["concatenate_test.cc"],
    deps = [
        ":concatenate",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "conditional",
    hdrs = ["conditional.h"],
)

cc_test(
    name = "conditional_test",
    srcs = ["conditional_test.cc"],
    deps = [
        ":conditional",
        ":same",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "contains",
    hdrs = ["contains.h"],
    deps = [
        ":any",
        ":invoke",
        ":same",
        ":tuple_manipulation",
    ],
)

cc_test(
    name = "contains_test",
    srcs = ["contains_test.cc"],
    deps = [
        ":contains",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "contains_base",
    hdrs = ["contains_base.h"],
    deps = [
        ":base",
        ":contains",
        ":invoke",
    ],
)

cc_test(
    name = "contains_base_test",
    srcs = ["contains_base_test.cc"],
    deps = [
        ":contains",
        ":contains_base",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "combine",
    hdrs = ["combine.h"],
)

cc_test(
    name = "combine_test",
    srcs = ["combine_test.cc"],
    deps = [
        ":combine",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "double_locked_value",
    hdrs = ["double_locked_value.h"],
)

cc_test(
    name = "double_locked_value_test",
    srcs = ["double_locked_value_test.cc"],
    deps = [
        ":double_locked_value",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "even_odd",
    hdrs = ["even_odd.h"],
    deps = [":concatenate"],
)

cc_test(
    name = "even_odd_test",
    srcs = ["even_odd_test.cc"],
    deps = [
        ":even_odd",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "increment",
    hdrs = ["increment.h"],
)

cc_test(
    name = "increment_test",
    srcs = ["increment_test.cc"],
    deps = [
        ":increment",
        ":invoke",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "interleave",
    hdrs = ["interleave.h"],
)

cc_library(
    name = "find_idx_of_val",
    hdrs = ["find_idx_of_val.h"],
    deps = [
        ":all_unique",
        ":same",
        ":tuple_manipulation",
    ],
)

cc_test(
    name = "find_idx_of_val_test",
    srcs = ["find_idx_of_val_test.cc"],
    deps = [
        ":find_idx_of_val",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "flatten",
    hdrs = ["flatten.h"],
    deps = [":concatenate"],
)

cc_test(
    name = "flatten_test",
    srcs = ["flatten_test.cc"],
    deps = [
        ":flatten",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "interleave_test",
    srcs = ["interleave_test.cc"],
    deps = [
        ":interleave",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "lambda_string",
    hdrs = ["lambda_string.h"],
)

cc_test(
    name = "lambda_string_test",
    srcs = ["lambda_string_test.cc"],
    deps = [
        ":lambda_string",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "invocable_map",
    hdrs = ["invocable_map.h"],
    deps = [
        ":interleave",
        ":tuple_from_size",
        ":tuple_manipulation",
        ":type_of_nth_element",
    ],
)

cc_test(
    name = "invocable_map_test",
    srcs = ["invocable_map_test.cc"],
    deps = [
        ":invocable_map",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "optional_wrap",
    hdrs = ["optional_wrap.h"],
)

cc_test(
    name = "optional_wrap_test",
    srcs = ["optional_wrap_test.cc"],
    deps = [
        ":optional_wrap",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "tuple_manipulation",
    hdrs = ["tuple_manipulation.h"],
)

cc_test(
    name = "tuple_manipulation_test",
    srcs = ["tuple_manipulation_test.cc"],
    deps = [
        ":tuple_manipulation",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "tuple_from_size",
    hdrs = ["tuple_from_size.h"],
)

cc_test(
    name = "tuple_from_size_test",
    srcs = ["tuple_from_size_test.cc"],
    deps = [
        ":tuple_from_size",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "type_map",
    hdrs = ["type_map.h"],
    deps = [
        ":interleave",
        ":lambda_string",
        ":tuple_from_size",
        ":tuple_manipulation",
        ":type_of_nth_element",
    ],
)

cc_test(
    name = "type_map_test",
    srcs = ["type_map_test.cc"],
    deps = [
        ":type_map",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "type_of_nth_element",
    hdrs = ["type_of_nth_element.h"],
    deps = [":unwrap"],
)

cc_test(
    name = "type_of_nth_element_test",
    srcs = ["type_of_nth_element_test.cc"],
    deps = [
        ":type_of_nth_element",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "queryable_map",
    hdrs = ["queryable_map.h"],
    deps = [
        ":interleave",
        ":tuple_from_size",
        ":tuple_manipulation",
        ":type_of_nth_element",
    ],
)

cc_library(
    name = "function_traits",
    hdrs = ["function_traits.h"],
)

cc_test(
    name = "function_traits_test",
    srcs = ["function_traits_test.cc"],
    deps = [
        ":function_traits",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "invoke",
    hdrs = ["invoke.h"],
    deps = [
        ":per_element",
        ":tuple_manipulation",
        ":type_of_nth_element",
    ],
)

cc_test(
    name = "invoke_test",
    srcs = ["invoke_test.cc"],
    deps = [
        ":increment",
        ":invoke",
        ":per_element",
        ":same",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "filter",
    hdrs = ["filter.h"],
    deps = [
        ":combine",
        ":concatenate",
        ":conditional",
        ":reduce",
        ":same",
    ],
)

cc_test(
    name = "filter_test",
    srcs = ["filter_test.cc"],
    deps = [
        ":filter",
        ":invoke",
        ":same",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "modified_max",
    hdrs = ["modified_max.h"],
)

cc_test(
    name = "modified_max_test",
    srcs = ["modified_max_test.cc"],
    deps = [
        ":modified_max",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "n_bit",
    hdrs = ["n_bit.h"],
)

cc_test(
    name = "n_bit_test",
    srcs = ["n_bit_test.cc"],
    deps = [
        ":increment",
        ":n_bit",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "n_bit_sequence",
    hdrs = ["n_bit_sequence.h"],
    deps = [
        ":increment",
        ":n_bit",
        ":type_index_mask",
        ":type_of_nth_element",
    ],
)

cc_test(
    name = "n_bit_sequence_test",
    srcs = ["n_bit_sequence_test.cc"],
    deps = [
        ":increment",
        ":n_bit",
        ":n_bit_sequence",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "per_element",
    hdrs = ["per_element.h"],
    deps = [
        ":tuple_manipulation",
        ":type_of_nth_element",
    ],
)

cc_test(
    name = "per_element_test",
    srcs = ["per_element_test.cc"],
    deps = [
        ":per_element",
        "@googletest//:gtest_main",
    ],
)

cc_test(
    name = "queryable_map_test",
    srcs = ["queryable_map_test.cc"],
    deps = [
        ":lambda_string",
        ":queryable_map",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "reduce",
    hdrs = ["reduce.h"],
    deps = [
        ":combine",
        ":tuple_manipulation",
    ],
)

cc_test(
    name = "reduce_test",
    srcs = ["reduce_test.cc"],
    deps = [
        ":combine",
        ":concatenate",
        ":conditional",
        ":invoke",
        ":reduce",
        ":same",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "reverse",
    hdrs = ["reverse.h"],
    deps = [
        ":concatenate",
        ":tuple_manipulation",
    ],
)

cc_test(
    name = "reverse_test",
    srcs = ["reverse_test.cc"],
    deps = [
        ":reverse",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "same",
    hdrs = ["same.h"],
)

cc_test(
    name = "same_test",
    srcs = ["same_test.cc"],
    deps = [
        ":same",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "string_concatenate",
    hdrs = ["string_concatenate.h"],
)

cc_test(
    name = "string_concatenate_test",
    srcs = ["string_concatenate_test.cc"],
    deps = [
        ":string_concatenate",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "type_index_mask",
    hdrs = ["type_index_mask.h"],
    deps = [":type_of_nth_element"],
)

cc_test(
    name = "type_index_mask_test",
    srcs = ["type_index_mask_test.cc"],
    deps = [
        ":type_index_mask",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "type_to_type_map",
    hdrs = ["type_to_type_map.h"],
    deps = [
        ":concatenate",
        ":even_odd",
        ":find_idx_of_val",
        ":same",
        ":tuple_manipulation",
        ":type_of_nth_element",
    ],
)

cc_test(
    name = "type_to_type_map_test",
    srcs = ["type_to_type_map_test.cc"],
    deps = [
        ":type_to_type_map",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "unfurl",
    hdrs = ["unfurl.h"],
    deps = [
    ],
)

cc_test(
    name = "unfurl_test",
    srcs = ["unfurl_test.cc"],
    deps = [
        ":contains",
        ":unfurl",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "unique_set",
    hdrs = ["unique_set.h"],
    deps = [
        ":contains",
        ":reverse",
    ],
)

cc_test(
    name = "unique_set_test",
    srcs = ["unique_set_test.cc"],
    deps = [
        ":unique_set",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "unwrap",
    hdrs = ["unwrap.h"],
    deps = [":tuple_manipulation"],
)

cc_test(
    name = "unwrap_test",
    srcs = ["unwrap_test.cc"],
    deps = [
        ":invoke",
        ":same",
        ":unwrap",
        "@googletest//:gtest_main",
    ],
)

cc_library(
    name = "zip_invoke",
    hdrs = ["zip_invoke.h"],
    deps = [
        ":apply",
        ":invoke",
        ":per_element",
        ":type_of_nth_element",
    ],
)

cc_test(
    name = "zip_invoke_test",
    srcs = ["zip_invoke_test.cc"],
    deps = [
        ":all",
        ":base",
        ":increment",
        ":per_element",
        ":same",
        ":unwrap",
        ":zip_invoke",
        "@googletest//:gtest_main",
    ],
)
