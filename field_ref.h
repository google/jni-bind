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

#ifndef JNI_BIND_FIELD_REF_H_
#define JNI_BIND_FIELD_REF_H_

#include <mutex>
#include <tuple>
#include <utility>

#include "class_ref.h"
#include "field.h"
#include "jni_dep.h"
#include "jni_helper/field_value.h"
#include "jni_helper/jni_helper.h"
#include "metaprogramming/double_locked_value.h"
#include "metaprogramming/optional_wrap.h"
#include "metaprogramming/queryable_map.h"

namespace jni {

// See JvmRef::~JvmRef.
static inline auto& GetDefaultLoadedFieldList() {
  static auto* ret_val =
      new std::vector<metaprogramming::DoubleLockedValue<jfieldID>*>{};
  return *ret_val;
}

// Represents a live instance of Field I's definition.
//
// Note, this class performs no cleanup on destruction.  jFieldIDs are static
// throughout the duration of a JVM's life, see JvmRef for teardown.
template <const auto& class_loader_v_, const auto& class_v_, size_t I>
class FieldRef {
 public:
  using ValueRaw =
      ValueRaw_t<std::decay_t<decltype(std::get<I>(class_v_.fields_))>>;

  explicit FieldRef(jclass class_ref, jobject object_ref)
      : class_ref_(class_ref), object_ref_(object_ref) {}

  FieldRef(const FieldRef&) = delete;
  FieldRef(const FieldRef&&) = delete;
  void operator=(const FieldRef&) = delete;

  static auto& GetField() { return std::get<I>(class_v_.fields_); }

  static const char* GetFieldSignature() {
    static std::string signature{GetField().Signature()};
    return signature.c_str();
  }

  // This method is thread safe.
  static jfieldID GetFieldID(jclass clazz) {
    static jni::metaprogramming::DoubleLockedValue<jfieldID> return_value;

    return return_value.LoadAndMaybeInit([=]() {
      if constexpr (class_loader_v_ == kDefaultClassLoader) {
        GetDefaultLoadedFieldList().push_back(&return_value);
      }

      return jni::JniHelper::GetFieldID(clazz, GetField().name_,
                                        GetFieldSignature());
    });
  }

  ValueRaw Get() {
    return FieldHelper<ValueRaw>::GetValue(object_ref_, GetFieldID(class_ref_));
  }

  void Set(ValueRaw&& value) {
    FieldHelper<ValueRaw>::SetValue(object_ref_, GetFieldID(class_ref_),
                                    std::forward<ValueRaw>(value));
  }

 private:
  const jclass class_ref_;
  const jobject object_ref_;
};

}  // namespace jni

#endif  // JNI_BIND_FIELD_REF_H_
