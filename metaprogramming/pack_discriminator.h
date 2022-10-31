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
#ifndef JNI_BIND_METAPROGRAMMING_PACK_DISCRIMINATOR_H_
#define JNI_BIND_METAPROGRAMMING_PACK_DISCRIMINATOR_H_

namespace jni::metaprogramming {

enum class PackType {
  TYPES,
  AUTO,
  AUTO_REF,
  CONST_AUTO_REF,
};

// Metafunction to discrimate the underlying pack type of a Container.
// Note: This interface is subject to change as the auto partial specialisations
// cannot discriminate on void.
struct PackDiscrimator {
  template <typename T>
  struct Helper;

  template <template <typename...> class Container, typename... Ts>
  struct Helper<Container<Ts...>> {
    static constexpr PackType val = PackType::TYPES;
  };

  template <template <auto...> class Container, auto... Vs>
  struct Helper<Container<Vs...>> {
    static constexpr PackType val = PackType::AUTO;
  };

  template <template <auto&...> class Container, auto&... Vs>
  struct Helper<Container<Vs...>> {
    static constexpr PackType val = PackType::AUTO_REF;
  };

  template <template <const auto&...> class Container, const auto&... Vs>
  struct Helper<Container<Vs...>> {
    static constexpr PackType val = PackType::CONST_AUTO_REF;
  };

  template <typename T>
  static constexpr PackType val = Helper<T>::val;
};

template <typename T>
static constexpr PackType PackDiscriminator_e =
    PackDiscrimator::template val<T>;

// Metafunction to forward a containerize pack to a compatible container.
template <template <template <typename...> class> class TypesContainer,
          template <template <auto...> class> class AutoContainer,
          template <template <const auto&...> class>
          class ConstAutoRefContainer>
struct PackDiscriminatedForward {
  template <typename T>
  struct Helper;

  template <template <typename...> class Container, typename... Ts>
  struct Helper<Container<Ts...>> {
    using type =
        typename TypesContainer<Container>::template type<Container<Ts...>>;
  };

  template <template <auto...> class Container, auto... vs>
  struct Helper<Container<vs...>> {
    using type =
        typename AutoContainer<Container>::template type<Container<vs...>>;
  };

  template <template <const auto&...> class Container, const auto&... vs>
  struct Helper<Container<vs...>> {
    using type = typename ConstAutoRefContainer<Container>::template type<
        Container<vs...>>;
  };

  template <typename T>
  using type = typename Helper<T>::type;
};

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_PACK_DISCRIMINATOR_H_
