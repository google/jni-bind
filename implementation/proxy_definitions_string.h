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

#include <string>
#include <type_traits>

#include "class_defs/java_lang_classes.h"
#include "implementation/default_class_loader.h"
#include "implementation/jvm.h"
#include "implementation/proxy.h"

namespace jni {

class LocalString;

template <typename JString>
struct Proxy<JString,
             typename std::enable_if_t<std::is_same_v<JString, jstring>>>
    : public ProxyBase<JString> {
  using AsArg =
      std::tuple<std::string, jstring, char*, const char*, std::string_view>;

  template <typename>
  using AsReturn = LocalString;

  template <typename T>
  struct Helper {
    static constexpr bool val = true;
  };

  template <typename OverloadSelection, typename T>
  static constexpr bool kViable =
      IsConvertibleKey<T>::template value<std::string> ||
      IsConvertibleKey<T>::template value<jstring> ||
      IsConvertibleKey<T>::template value<char*> ||
      IsConvertibleKey<T>::template value<const char*> ||
      IsConvertibleKey<T>::template value<std::string_view>;

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
      return JniHelper::NewLocalString(s);
    } else {
      return JniHelper::NewLocalString(s.data());
    }
  }
};

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_PROXY_DEFINITIONS_STRING_H_
