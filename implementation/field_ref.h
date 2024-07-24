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

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"

#include <type_traits>
#include <utility>
#include <vector>

#include "implementation/field_selection.h"
#include "implementation/id.h"
#include "implementation/id_type.h"
#include "implementation/jni_helper/field_value.h"
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/no_idx.h"
#include "implementation/promotion_mechanics_tags.h"
#include "implementation/proxy_convenience_aliases.h"
#include "implementation/ref_base.h"
#include "implementation/signature.h"
#include "jni_dep.h"
#include "metaprogramming/double_locked_value.h"

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
template <typename JniT, IdType field_type, std::size_t I>
class FieldRef {
 public:
  using IdT = Id<JniT, field_type, I, kNoIdx, kNoIdx, 0>;
  using FieldSelectionT = FieldSelection<JniT, I>;

  using SelfIdT = typename IdT::template ChangeIdType<IdType::CLASS>;

  explicit FieldRef(jclass class_ref, jobject object_ref)
      : class_ref_(class_ref), object_ref_(object_ref) {}

  FieldRef(const FieldRef&) = delete;
  FieldRef(const FieldRef&&) = delete;
  void operator=(const FieldRef&) = delete;

  // This method is thread safe.
  static jfieldID GetFieldID(jclass clazz) {
    static jni::metaprogramming::DoubleLockedValue<jfieldID> return_value;

    return return_value.LoadAndMaybeInit([=]() {
      if constexpr (JniT::class_loader_v == kDefaultClassLoader) {
        GetDefaultLoadedFieldList().push_back(&return_value);
      }

      if constexpr (IdT::kIsStatic) {
        return jni::JniHelper::GetStaticFieldID(clazz, IdT::Name(),
                                                Signature_v<IdT>.data());
      } else {
        return jni::JniHelper::GetFieldID(clazz, IdT::Name(),
                                          Signature_v<IdT>.data());
      }
    });
  }

  using ReturnProxied =
      std::conditional_t<IdT::kIsSelf,
                         Return_t<typename SelfIdT::MaterializeCDeclT, SelfIdT>,
                         Return_t<typename IdT::MaterializeCDeclT, IdT> >;

  const auto& SelfVal() {
    if constexpr (IdT::kIsStatic) {
      return class_ref_;
    } else {
      return object_ref_;
    }
  }

  ReturnProxied Get() {
    if constexpr (std::is_base_of_v<RefBaseBase, ReturnProxied>) {
      return {AdoptLocal{},
              FieldHelper<CDecl_t<typename IdT::RawValT>, IdT::kRank,
                          IdT::kIsStatic>::GetValue(SelfVal(),
                                                    GetFieldID(class_ref_))};
    } else {
      return {FieldHelper<CDecl_t<typename IdT::RawValT>, IdT::kRank,
                          IdT::kIsStatic>::GetValue(SelfVal(),
                                                    GetFieldID(class_ref_))};
    }
  }

  template <typename T>
  void Set(T&& value) {
    FieldHelper<CDecl_t<typename IdT::RawValT>, IdT::kRank,
                IdT::kIsStatic>::SetValue(SelfVal(), GetFieldID(class_ref_),
                                          Proxy_t<T>::ProxyAsArg(
                                              std::forward<T>(value)));
  }

 private:
  const jclass class_ref_;
  const jobject object_ref_;
};

}  // namespace jni

#endif  // JNI_BIND_FIELD_REF_H_
