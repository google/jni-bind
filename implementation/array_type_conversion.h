/*
 * Copyright 2022 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef JNI_BIND_IMPLEMENTATION_ARRAY_TYPE_CONVERSION_H_
#define JNI_BIND_IMPLEMENTATION_ARRAY_TYPE_CONVERSION_H_

#include "jni_dep.h"
#include "metaprogramming/type_to_type_map.h"

namespace jni {

template <typename T>
struct ArrayTag {};

template <typename T>
static constexpr bool kIsArrayType =
    std::is_base_of_v<ArrayTag<jbyteArray>, T> ||
    std::is_base_of_v<ArrayTag<jcharArray>, T> ||
    std::is_base_of_v<ArrayTag<jshortArray>, T> ||
    std::is_base_of_v<ArrayTag<jintArray>, T> ||
    std::is_base_of_v<ArrayTag<jfloatArray>, T> ||
    std::is_base_of_v<ArrayTag<jdoubleArray>, T> ||
    std::is_base_of_v<ArrayTag<jlongArray>, T> ||
    std::is_base_of_v<ArrayTag<jbooleanArray>, T> ||
    std::is_base_of_v<ArrayTag<jobjectArray>, T> ||
    std::is_base_of_v<ArrayTag<jarray>, T>;

// Primitive Keys.
using PrimitiveKeys =
    std::tuple<jbyteArray, jcharArray, jshortArray, jintArray, jlongArray,
               jfloatArray, jdoubleArray, jbooleanArray>;

// Simple type for proxying types used in the API (e.g. jint) to their
// corresponding array type (e.g. jintarray). Only use the former type when
// using JNI Bind (e.g. LocalArray<jint>, not LocalArray<jintArray>).
using RegularToArrayTypeMap = metaprogramming::TypeToTypeMap<
    std::tuple<jbyte, jchar, jshort, jint, jlong, jfloat, jdouble, jboolean,
               jobject, jstring, jarray>,
    std::tuple<jbyteArray, jcharArray, jshortArray, jintArray, jlongArray,
               jfloatArray, jdoubleArray, jbooleanArray, jobjectArray,
               jobjectArray, jarray>>;

// Given a type, returns the corresponding array type (e.g. jint => jintArray).
template <typename T>
using RegularToArrayTypeMap_t =
    metaprogramming::TypeToTypeMapQuery_t<RegularToArrayTypeMap, T>;

// Array to CDecl type used for invocation.
// Defined separately since this map is not invertible (jobject, jstring =>
// jobject).
using ArrayToRegularTypeMap = metaprogramming::TypeToTypeMap<
    std::tuple<jbyteArray, jcharArray, jshortArray, jintArray, jlongArray,
               jfloatArray, jdoubleArray, jbooleanArray, jobjectArray, jarray>,
    std::tuple<jbyte, jchar, jshort, jint, jlong, jfloat, jdouble, jboolean,
               jobject, jarray>>;

template <typename T>
using ArrayToRegularTypeMap_t =
    metaprogramming::TypeToTypeMapQuery_t<ArrayToRegularTypeMap, T>;

////////////////////////////////////////////////////////////////////////////////
// Storage Helper Metafunction.
////////////////////////////////////////////////////////////////////////////////

// Figures out the underlying physical opaque handle used to store a type.
// e.g. A rank two int is a jobjectarray.
template <typename T, std::size_t kRank>
struct StorageHelper {
  using type = jobjectArray;
};

// HACK: jstring has its own type despite just being a jobject. To make the
// lookup tables above function, this is handled separately. This will hopefully
// be removed in the future.
template <>
struct StorageHelper<jstring, 1> {
  using type = jobjectArray;
};

template <typename T>
struct StorageHelper<T, 1> {
  using type = RegularToArrayTypeMap_t<T>;
};

template <typename T>
struct StorageHelper<T, 0> {
  using type = T;
};

template <typename T, std::size_t kRank>
using StorageHelper_t = typename StorageHelper<T, kRank>::type;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_ARRAY_TYPE_CONVERSION_H_
