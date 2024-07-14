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

#ifndef JNI_BIND_FIELD_SELECTION_H_
#define JNI_BIND_FIELD_SELECTION_H_

#include "implementation/id.h"
#include "implementation/id_type.h"
#include "implementation/jni_helper/jni_typename_to_string.h"
#include "implementation/name_constants.h"
#include "implementation/no_idx.h"
#include "implementation/object.h"
#include "implementation/selector_static_info.h"
#include "metaprogramming/string_concatenate.h"

namespace jni {

template <typename JniT, size_t field_idx_>
struct FieldSelection {
  using IdT = Id<JniT, IdType::FIELD, field_idx_, kNoIdx, kNoIdx, 0>;
  static constexpr IdType kRetTypeId = IdType::FIELD;
};

}  // namespace jni

#endif  // JNI_BIND_FIELD_SELECTION_H_
