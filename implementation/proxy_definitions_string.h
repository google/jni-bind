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
#ifndef JNI_BIND_IMPLEMENTATION_PROXY_DEFINITIONS_STRING_H_
#define JNI_BIND_IMPLEMENTATION_PROXY_DEFINITIONS_STRING_H_

#include <iterator>
#include <string>
#include <type_traits>

#include "class_defs/java_lang_classes.h"
#include "implementation/default_class_loader.h"
#include "implementation/forward_declarations.h"
#include "implementation/jni_helper/lifecycle_string.h"
#include "implementation/jvm.h"
#include "implementation/proxy.h"
#include "implementation/ref_base.h"

namespace jni {


template <typename JString>
struct Proxy<JString,
             typename std::enable_if_t<std::is_same_v<JString, jstring>>>
    : public ProxyBase<JString> {
  template <typename Id, std::size_t kRank>
  struct Helper {
    using type = LocalArray<jstring, kRank, kJavaLangString,
                            kDefaultClassLoader, kDefaultJvm>;
  };

  template <typename Id>
  struct Helper<Id, 0> {
    using type = LocalString;
  };

  using AsArg = std::tuple<std::string, jstring, char*, const char*,
                           std::string_view, RefBaseTag<jstring>>;

  template <typename Id>
  using AsReturn = typename Helper<Id, Id::kRank>::type;

  template <typename OverloadSelection, typename T>
  static constexpr bool kViable =
      IsConvertibleKey<T>::template value<std::string> ||
      IsConvertibleKey<T>::template value<jstring> ||
      IsConvertibleKey<T>::template value<char*> ||
      IsConvertibleKey<T>::template value<const char*> ||
      IsConvertibleKey<T>::template value<std::string_view> ||
      std::is_same_v<T, LocalString> || std::is_same_v<T, GlobalString>;

  // These leak local instances of strings.  Usually, RAII mechanisms would
  // correctly release local instances, but here we are stripping that so it can
  // be used in a method.  This could be obviated by wrapping the calling scope
  // in a local stack frame.
  static jstring ProxyAsArg(jstring s) { return s; }

  template <typename T,
            typename = std::enable_if_t<std::is_same_v<T, const char*> ||
                                        std::is_same_v<T, std::string> ||
                                        std::is_same_v<T, std::string_view>>>
  static jstring ProxyAsArg(T s) {
    if constexpr (std::is_same_v<T, const char*>) {
      return LifecycleHelper<jstring, LifecycleType::LOCAL>::Construct(s);
    } else {
      return LifecycleHelper<jstring, LifecycleType::LOCAL>::Construct(
          s.data());
    }
  }

  template <typename T,
            typename = std::enable_if_t<std::is_same_v<T, GlobalString> ||
                                        std::is_same_v<T, LocalString>>>
  static jstring ProxyAsArg(T& t) {
    return jstring{t};
  }

  template <typename T,
            typename = std::enable_if_t<std::is_same_v<T, GlobalString> ||
                                        std::is_same_v<T, LocalString>>>
  static jstring ProxyAsArg(T&& t) {
    return t.Release();
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_PROXY_DEFINITIONS_STRING_H_
