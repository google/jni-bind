/*
 * Copyright 2023 Google LLC
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

#ifndef JNI_BIND_METAPROGRAMMING_PASSTHROUGH_H_
#define JNI_BIND_METAPROGRAMMING_PASSTHROUGH_H_

#include <utility>

namespace jni::metaprogramming {

template <typename T>
struct DefaultDeleter {
  static void Do(T&) {}
};

template <typename T, typename CustomDeleter = DefaultDeleter<T>>
struct Passthrough {
  template <typename... Ts>
  Passthrough(Ts... ts) : t_(std::forward<Ts>(ts)...) {}

  ~Passthrough() { CustomDeleter::Do(t_); }

  T& operator*() { return t_; }
  T* operator->() { return &t_; }

  template <typename... Us>
  auto operator()(Us&&... us) {
    return t_(us...);
  }

  template <typename U>
  explicit operator U() const {
    return static_cast<U>(t_);
  }

  T t_;
};

template <typename T, typename U, typename CustomDeleter>
constexpr bool operator==(const T& lhs,
                          const Passthrough<U, CustomDeleter>& rhs) {
  return lhs == static_cast<T>(rhs.t_);
}

template <typename T, typename U, typename CustomDeleter>
bool constexpr operator==(const Passthrough<U, CustomDeleter>& lhs,
                          const T& rhs) {
  return static_cast<T>(lhs.t_) == rhs;
}

template <typename T, typename U, typename CustomDeleter>
constexpr bool operator!=(const T& lhs,
                          const Passthrough<U, CustomDeleter>& rhs) {
  return !(lhs == rhs);
}

template <typename T, typename U, typename CustomDeleter>
constexpr bool operator!=(const Passthrough<U, CustomDeleter>& lhs,
                          const T& rhs) {
  return !(lhs == rhs);
}

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_PASSTHROUGH_H_
