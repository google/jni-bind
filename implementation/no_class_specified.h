/*
 * Copyright 2024 Google LLC
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
#ifndef JNI_BIND_IMPLEMENTATION_NO_CLASS_SPECIFIED_H_
#define JNI_BIND_IMPLEMENTATION_NO_CLASS_SPECIFIED_H_

#include <tuple>

#include "implementation/static.h"

namespace jni {

struct ExtendsBase {};

struct RootObject {
  constexpr RootObject() = default;
};

static constexpr RootObject kObject{};

static constexpr struct NoClass {
  constexpr NoClass() {}

  const char* name_ = "__JNI_BIND__NO_CLASS__";
  const RootObject parent_;
  const Static<std::tuple<>, std::tuple<>> static_{};
  const std::tuple<> methods_{};
  const std::tuple<> fields_{};

  constexpr bool operator==(const NoClass&) const { return true; }
  constexpr bool operator!=(const NoClass&) const { return true; }
} kNoClassSpecified;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_NO_CLASS_SPECIFIED_H_
