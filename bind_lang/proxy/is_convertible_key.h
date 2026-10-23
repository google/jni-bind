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

#ifndef JNI_BIND_BIND_LANG_PROXY_IS_CONVERTIBLE_KEY_H_
#define JNI_BIND_BIND_LANG_PROXY_IS_CONVERTIBLE_KEY_H_

#include <type_traits>

namespace bind_lang {

// Checks if a type is related by being the same decayed type, or having a
// base/derived relationship. This allows for looser matching than exact type
// equality, e.g., handling array-to-pointer decay or base/derived proxies.
template <typename Query>
struct IsConvertibleKey {
  template <typename T>
  static constexpr bool value =
      std::is_same_v<std::decay_t<Query>, std::decay_t<T>> ||
      std::is_base_of_v<std::decay_t<T>, std::decay_t<Query>> ||
      std::is_base_of_v<std::decay_t<Query>, std::decay_t<T>>;
};

template <typename Query, typename T>
inline constexpr bool IsConvertibleKey_v =
    IsConvertibleKey<Query>::template value<T>;

}  // namespace bind_lang

#endif  // JNI_BIND_BIND_LANG_PROXY_IS_CONVERTIBLE_KEY_H_
