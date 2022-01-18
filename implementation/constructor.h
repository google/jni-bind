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

#ifndef JNI_BIND_CONSTRUCTOR_H_
#define JNI_BIND_CONSTRUCTOR_H_

#include "method.h"
#include "params.h"
#include "return.h"
#include "implementation/jni_helper/jni_helper.h"

namespace jni {

struct ConstructorBase {};

template <typename... ParamsRaw_>
class Constructor : public ConstructorBase {
 public:
  const Params<ParamsRaw_...> params_;

  // Zero args constructor.
  template <std::size_t N = sizeof...(ParamsRaw_),
            typename std::enable_if<(N == 0), int>::type = 0>
  constexpr explicit Constructor() {}

  // CTAD constructor, fully defined args (e.g. Params{class_value}).
  template <
      std::size_t N = sizeof...(ParamsRaw_), typename... Args,
      typename std::enable_if<(N > 0 && sizeof...(Args) == N), int>::type = 0>
  constexpr explicit Constructor(const Args... params_raw)
      : params_(params_raw...) {}

  // Constructor for explicit templatizing, no args (e.g. Params<jint>{}).
  template <
      std::size_t N = sizeof...(ParamsRaw_), typename... Args,
      typename std::enable_if<(N > 0 && sizeof...(Args) == 0), int>::type = 0>
  constexpr explicit Constructor(Args...) {}
};

template <typename... ParamsRaw>
Constructor(ParamsRaw...) -> Constructor<ParamsRaw...>;

template <typename... LhsParams, typename... RhsParams>
constexpr bool operator==(const Constructor<LhsParams...>& lhs,
                          const Constructor<LhsParams...>& rhs) {
  return lhs.params_ == rhs.params_;
}

//==============================================================================
// Represents a constructor used at runtime and has index data about where it
// exists in the static class definition which is embedded on the caller's
// behalf.  This is particular to constructors which exist as an overload set.
template <const auto& class_v_, size_t I>
class ConstructorRef : public ConstructorBase {
 public:
  static const auto& GetConstructor() {
    return std::get<I>(class_v_.constructors_);
  }

  static std::string* GetConstructorSignature() {
    static std::string* constructor_signature = new std::string{
        GetConstructor().params_.GetSignature() + std::string{"V"}};
    return constructor_signature;
  }

  static jmethodID GetConstructorMethodID(jclass class_ref) {
    // TODO(b/174287131): Store and release ID statically.
    return JniHelper::GetMethodID(class_ref, "<init>",
                                  GetConstructorSignature()->c_str());
  }
};

}  // namespace jni

#endif  // JNI_BIND_CONSTRUCTOR_H_
