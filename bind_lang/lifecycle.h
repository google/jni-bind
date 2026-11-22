/*
 * Copyright 2026 Google LLC
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

#ifndef JNI_BIND_BIND_LANG_LIFECYCLE_H_
#define JNI_BIND_BIND_LANG_LIFECYCLE_H_

namespace bind_lang {

// Ownership model of a runtime handle (e.g. a JNI `jobject` or a
// `google::protobuf::Message*`).
enum class LifecycleType {
  LOCAL,
  GLOBAL,
  // WEAK, // not implemented yet.
};

// Customization point for creating, duplicating, promoting, and releasing
// handles of type `Span` under a given `LifecycleType`.
//
// Each binding language specializes this for its own handle types, providing
// whichever of the following static members it supports:
//   `Construct(...)`, `NewReference(Span)`, `Promote(Span)`, `Delete(Span)`.
//
// Specializations must be declared in `namespace bind_lang`.
template <typename Span, LifecycleType lifecycle_type>
struct LifecycleHelper;

}  // namespace bind_lang

#endif  // JNI_BIND_BIND_LANG_LIFECYCLE_H_
