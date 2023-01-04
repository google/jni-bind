/*
 * Copyright 2021 Google LLC
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

#ifndef JNI_BIND_FIELD_H_
#define JNI_BIND_FIELD_H_

#include <string>

#include "params.h"

namespace jni {

struct FieldBase {};

template <typename Raw_>
struct Field : public FieldBase {
 public:
  using Raw = Raw_;

  const char* name_;

  const Raw_ raw_ = {};

  constexpr Field(const char* name) : name_(name) {}
  constexpr Field(const char* name, Raw_ value_raw)
      : name_(name), raw_(value_raw) {}
};

template <typename Raw_>
Field(const char*, Raw_) -> Field<Raw_>;

template <typename T>
using Raw_t = typename T::Raw;

}  // namespace jni

#endif  // JNI_BIND_FIELD_H_
