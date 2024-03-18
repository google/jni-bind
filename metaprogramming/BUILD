package(
    default_visibility = ["//visibility:public"],
)

licenses(["notice"])

exports_files(["LICENSE"])

################################################################################
# All.
################################################################################
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
        "@com_google_googletest//:gtest_main",
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Any.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Apply.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Base.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Call.
################################################################################
cc_library(
    name = "call",
    hdrs = ["call.h"],
)

cc_test(
    name = "call_test",
    srcs = ["call_test.cc"],
    deps = [
        ":call",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Cartesian Product.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Chain.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# color.
################################################################################
cc_library(
    name = "color",
    hdrs = ["color.h"],
)

cc_test(
    name = "color_test",
    srcs = ["color_test.cc"],
    deps = [
        ":color",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Concatenate.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Conditional.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Conjunction.
################################################################################
cc_library(
    name = "conjunction",
    hdrs = ["conjunction.h"],
    deps = [
        ":invoke",
        ":next",
    ],
)

cc_test(
    name = "conjunction_test",
    srcs = ["conjunction_test.cc"],
    deps = [
        ":conjunction",
        ":invoke",
        ":next",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Contains.
################################################################################
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
        "@com_google_googletest//:gtest_main",
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Combine.
################################################################################
cc_library(
    name = "combine",
    hdrs = ["combine.h"],
)

cc_test(
    name = "combine_test",
    srcs = ["combine_test.cc"],
    deps = [
        ":combine",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Corpus.
################################################################################
cc_library(
    name = "corpus",
    hdrs = ["corpus.h"],
    deps = [
        ":concatenate",
        ":corpus_tag",
        ":detect",
    ],
)

cc_library(
    name = "corpus_tag",
    hdrs = ["corpus_tag.h"],
)

cc_test(
    name = "corpus_test",
    srcs = ["corpus_test.cc"],
    deps = [
        ":corpus",
        ":corpus_tag",
        ":detect",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Deep Equal.
################################################################################
cc_library(
    name = "deep_equal",
    hdrs = ["deep_equal.h"],
    deps = [
        ":pack_discriminator",
        ":vals_equal",
    ],
)

cc_test(
    name = "deep_equal_test",
    srcs = ["deep_equal_test.cc"],
    deps = [
        ":deep_equal",
        ":pack_discriminator",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "deep_equal_diminished",
    hdrs = ["deep_equal_diminished.h"],
    deps = [
        ":pack_discriminator",
        ":vals_equal_diminished",
    ],
)

cc_test(
    name = "deep_equal_diminished_test",
    srcs = ["deep_equal_diminished_test.cc"],
    deps = [
        ":deep_equal_diminished",
        ":pack_discriminator",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Detect.
################################################################################
cc_library(
    name = "detect",
    hdrs = ["detect.h"],
)

cc_test(
    name = "detect_test",
    srcs = ["detect_test.cc"],
    deps = [
        ":detect",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Double Locked Value.
################################################################################
cc_library(
    name = "double_locked_value",
    hdrs = ["double_locked_value.h"],
)

cc_test(
    name = "double_locked_value_test",
    srcs = ["double_locked_value_test.cc"],
    deps = [
        ":double_locked_value",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Even Odd.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Filter.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Find Idx.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Flatten.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Function Traits.
################################################################################
cc_library(
    name = "function_traits",
    hdrs = ["function_traits.h"],
)

cc_test(
    name = "function_traits_test",
    srcs = ["function_traits_test.cc"],
    deps = [
        ":function_traits",
        "@com_google_googletest//:gtest_main",
    ],
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
# Increment.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Index.
################################################################################
cc_library(
    name = "index",
    hdrs = ["index.h"],
    deps = [":tuple_manipulation"],
)

cc_test(
    name = "index_test",
    srcs = ["index_test.cc"],
    deps = [
        ":index",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Interleave.
################################################################################
cc_library(
    name = "interleave",
    hdrs = ["interleave.h"],
)

cc_test(
    name = "interleave_test",
    srcs = ["interleave_test.cc"],
    deps = [
        ":interleave",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Invocable Map.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Invoke.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Lambda Compatible.
################################################################################
cc_library(
    name = "lambda_compatible",
    hdrs = ["lambda_compatible.h"],
    deps = [
        ":deep_equal",
        ":tuple_manipulation",
        ":type_of_nth_element",
    ],
)

cc_test(
    name = "lambda_compatible_test",
    srcs = ["lambda_compatible_test.cc"],
    deps = [
        ":deep_equal",
        ":lambda_compatible",
        ":vals",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Lambda String.
################################################################################
cc_library(
    name = "lambda_string",
    hdrs = ["lambda_string.h"],
)

cc_test(
    name = "lambda_string_test",
    srcs = ["lambda_string_test.cc"],
    deps = [
        ":lambda_string",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Optional Wrap.
################################################################################
cc_library(
    name = "optional_wrap",
    hdrs = ["optional_wrap.h"],
)

cc_test(
    name = "optional_wrap_test",
    srcs = ["optional_wrap_test.cc"],
    deps = [
        ":optional_wrap",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Tuple Manipulation.
################################################################################
cc_library(
    name = "tuple_manipulation",
    hdrs = ["tuple_manipulation.h"],
)

cc_test(
    name = "tuple_manipulation_test",
    srcs = ["tuple_manipulation_test.cc"],
    deps = [
        ":tuple_manipulation",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Tuple from Size.
################################################################################
cc_library(
    name = "tuple_from_size",
    hdrs = ["tuple_from_size.h"],
)

cc_test(
    name = "tuple_from_size_test",
    srcs = ["tuple_from_size_test.cc"],
    deps = [
        ":tuple_from_size",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Type Map.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Type of nth Element.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
#  Min Max.
################################################################################
cc_library(
    name = "min_max",
    hdrs = ["min_max.h"],
    deps = [":vals"],
)

cc_test(
    name = "min_max_test",
    srcs = ["min_max_test.cc"],
    deps = [
        ":min_max",
        ":reduce",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Modified Max.
################################################################################
cc_library(
    name = "modified_max",
    hdrs = ["modified_max.h"],
)

cc_test(
    name = "modified_max_test",
    srcs = ["modified_max_test.cc"],
    deps = [
        ":modified_max",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Name Constants.
################################################################################
cc_library(
    name = "name_constants",
    hdrs = ["name_constants.h"],
)

################################################################################
# N bit.
################################################################################
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
        "@com_google_googletest//:gtest_main",
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Next.
################################################################################
cc_library(
    name = "next",
    hdrs = ["next.h"],
    deps = [
        ":pack_discriminator",
    ],
)

cc_test(
    name = "next_test",
    srcs = ["next_test.cc"],
    deps = [
        ":next",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Pack Discriminator.
################################################################################
cc_library(
    name = "pack_discriminator",
    hdrs = ["pack_discriminator.h"],
)

cc_test(
    name = "pack_discriminator_test",
    srcs = ["pack_discriminator_test.cc"],
    deps = [
        ":pack_discriminator",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Passthrough.
################################################################################
cc_library(
    name = "passthrough",
    hdrs = ["passthrough.h"],
)

cc_test(
    name = "passthrough_test",
    srcs = ["passthrough_test.cc"],
    deps = [
        ":passthrough",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Per Element.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "per_element_val",
    hdrs = ["per_element_val.h"],
)

cc_test(
    name = "per_element_val_test",
    srcs = ["per_element_val_test.cc"],
    deps = [
        ":per_element_val",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Queryable Map.
################################################################################
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

cc_test(
    name = "queryable_map_test",
    srcs = ["queryable_map_test.cc"],
    deps = [
        ":lambda_string",
        ":queryable_map",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Reduce.
################################################################################
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
        ":vals",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Repeat String.
################################################################################
cc_library(
    name = "repeat_string",
    hdrs = ["repeat_string.h"],
    deps = [":string_concatenate"],
)

cc_test(
    name = "repeat_string_test",
    srcs = ["repeat_string_test.cc"],
    deps = [
        ":repeat_string",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "replace_string",
    hdrs = ["replace_string.h"],
    deps = [":lambda_string"],
)

cc_test(
    name = "replace_string_test",
    srcs = ["replace_string_test.cc"],
    deps = [
        ":replace_string",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Reverse.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Same.
################################################################################
cc_library(
    name = "same",
    hdrs = ["same.h"],
)

cc_test(
    name = "same_test",
    srcs = ["same_test.cc"],
    deps = [
        ":same",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# String Concenate.
################################################################################
cc_library(
    name = "string_concatenate",
    hdrs = ["string_concatenate.h"],
)

cc_test(
    name = "string_concatenate_test",
    srcs = ["string_concatenate_test.cc"],
    deps = [
        ":string_concatenate",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# String Contains.
################################################################################
cc_library(
    name = "string_contains",
    hdrs = ["string_contains.h"],
    deps = [":lambda_string"],
)

cc_test(
    name = "string_contains_test",
    srcs = ["string_contains_test.cc"],
    deps = [
        ":string_contains",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Type Index Mask.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Type to Type Map.
################################################################################
cc_library(
    name = "type_to_type_map",
    hdrs = ["type_to_type_map.h"],
    deps = [
        ":concatenate",
        ":contains",
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Unfurl.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Unique Set.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Unwrap.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Vals.
################################################################################
cc_library(
    name = "vals",
    hdrs = ["vals.h"],
)

cc_library(
    name = "vals_equal",
    hdrs = ["vals_equal.h"],

    # This is incompatibel with MSVC, do not use.
    visibility = ["//visibility:private"],
    deps = [":vals"],
)

cc_test(
    name = "vals_equal_test",
    srcs = ["vals_equal_test.cc"],
    deps = [
        ":vals",
        ":vals_equal",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_library(
    name = "vals_equal_diminished",
    hdrs = ["vals_equal_diminished.h"],
    deps = [":vals"],
)

cc_test(
    name = "vals_equal_diminished_test",
    srcs = ["vals_equal_diminished_test.cc"],
    deps = [
        ":vals_equal_diminished",
        "@com_google_googletest//:gtest_main",
    ],
)

################################################################################
# Zip Invoke.
################################################################################
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
        "@com_google_googletest//:gtest_main",
    ],
)
