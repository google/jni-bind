/*
 * Copyright 2023 Google LLC
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
#ifndef JNI_BIND_IMPLEMENTATION_PROMOTION_MECHANICS_TAGS_H_
#define JNI_BIND_IMPLEMENTATION_PROMOTION_MECHANICS_TAGS_H_

namespace jni {

// Adopts a local.
struct AdoptLocal {};

// Creates an additional reference to the underlying object.
// When used for local, presumes local, for global, presumes global.
struct CreateCopy {};

// This tag allows the constructor to promote underlying jobject for you.
struct PromoteToGlobal {};

// CAUTION: This tag assume the underlying jobject has been pinned as a global.
// This is atypical when solely using JNI Bind, use with caution.
struct AdoptGlobal {};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_PROMOTION_MECHANICS_TAGS_H_
