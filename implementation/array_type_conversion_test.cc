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

/*
#include <type_traits>

#include "jni_bind.h"

namespace {

using ::jni::StorageHelper_t;

static_assert(std::is_same_v<StorageHelper_t<jint, 0>, jint>);
static_assert(std::is_same_v<StorageHelper_t<jint, 1>, jintArray>);
static_assert(std::is_same_v<StorageHelper_t<jint, 2>, jobjectArray>);

static_assert(std::is_same_v<StorageHelper_t<jstring, 0>, jstring>);
static_assert(std::is_same_v<StorageHelper_t<jstring, 1>, jobjectArray>);
static_assert(std::is_same_v<StorageHelper_t<jstring, 2>, jobjectArray>);

static_assert(std::is_same_v<StorageHelper_t<jobject, 0>, jobject>);
static_assert(std::is_same_v<StorageHelper_t<jobject, 1>, jobjectArray>);
static_assert(std::is_same_v<StorageHelper_t<jobject, 2>, jobjectArray>);

}  // namespace

*/