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

#ifndef JNI_BIND_IMPLEMENTATION_SELF_H_
#define JNI_BIND_IMPLEMENTATION_SELF_H_

namespace jni {

// Tag to indicate you are referring to the enclosing class.
// Useful for builder patterns where the decorated object returned is identical.
struct Self {};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_SELF_H_
