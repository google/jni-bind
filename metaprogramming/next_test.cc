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

#include "metaprogramming/next.h"

#include <cstddef>
#include <type_traits>

#ifdef NO

using ::jni::metaprogramming::EndConstRefVal;
using ::jni::metaprogramming::EndType;
// using ::jni::metaprogramming::EndVal;

using ::jni::metaprogramming::Next_t;
using ::jni::metaprogramming::NextConstRefVal;  // NOLINT
using ::jni::metaprogramming::NextType;         // NOLINT
// using ::jni::metaprogramming::NextVal;          // NOLINT

namespace jni::metaprogramming {

////////////////////////////////////////////////////////////////////////////////
// Type tests.
////////////////////////////////////////////////////////////////////////////////
template <typename I, typename Max>
struct TypeCounter {};

template <std::size_t I, std::size_t kMaxVal>
using TypeCounter_t = TypeCounter<std::integral_constant<std::size_t, I>,
                                  std::integral_constant<std::size_t, kMaxVal>>;

template <>
struct NextType<TypeCounter> {
  template <typename TypeCounter>
  struct Helper;

  template <std::size_t I, std::size_t kMaxVal>
  struct Helper<TypeCounter_t<I, kMaxVal>> {
    using type = TypeCounter_t<I + 1, kMaxVal>;
  };

  template <std::size_t kMaxVal>
  struct Helper<TypeCounter_t<kMaxVal, kMaxVal>> {
    using type = EndType<TypeCounter>;
  };

  template <typename T>
  using type = typename Helper<T>::type;
};

static_assert(std::is_same_v<Next_t<TypeCounter_t<0, 3>>, TypeCounter_t<1, 3>>);

static_assert(
    std::is_same_v<Next_t<Next_t<TypeCounter_t<0, 3>>>, TypeCounter_t<2, 3>>);
static_assert(std::is_same_v<Next_t<Next_t<Next_t<TypeCounter_t<0, 3>>>>,
                             TypeCounter_t<3, 3>>);
static_assert(
    std::is_same_v<Next_t<Next_t<Next_t<Next_t<TypeCounter_t<0, 3>>>>>,
                   EndType<TypeCounter>>);

/*
////////////////////////////////////////////////////////////////////////////////
// Auto tests.
////////////////////////////////////////////////////////////////////////////////
template <std::size_t I, std::size_t kMaxVal>
struct Counter {};

template <>
struct NextVal<Counter> {
  template <typename T>
  struct Helper;

  template <std::size_t I, std::size_t kMaxVal>
  struct Helper<Counter<I, kMaxVal>> {
    using type = Counter<I + 1, kMaxVal>;
  };

  template <std::size_t kMaxVal>
  struct Helper<Counter<kMaxVal, kMaxVal>> {
    using type = EndVal<Counter>;
  };

  template <typename T>
  using type = typename Helper<T>::type;
};

static_assert(std::is_same_v<Next_t<Counter<0, 3>>, Counter<1, 3>>);
static_assert(std::is_same_v<Next_t<Next_t<Counter<0, 3>>>, Counter<2, 3>>);
static_assert(
    std::is_same_v<Next_t<Next_t<Next_t<Counter<0, 3>>>>, Counter<3, 3>>);
static_assert(std::is_same_v<Next_t<Next_t<Next_t<Next_t<Counter<0, 3>>>>>,
                             EndVal<Counter>>);
                             */

////////////////////////////////////////////////////////////////////////////////
// Const Auto& tests.
////////////////////////////////////////////////////////////////////////////////
template <const std::size_t& I, const std::size_t& kMaxVal>
struct ConstRefCounter {
  static constexpr std::size_t val = I;
};

static constexpr std::size_t kTheValue0 = 0;
static constexpr std::size_t kMaxValSizeGlobal = 3;

template <>
struct NextConstRefVal<ConstRefCounter> {
  template <typename T>
  struct Helper;

  template <const std::size_t& I, const std::size_t& kMaxVal>
  struct Helper<ConstRefCounter<I, kMaxVal>> {
    static constexpr std::size_t kNextVal = I + 1;
    using type = ConstRefCounter<kNextVal, kMaxVal>;
  };

  template <const std::size_t& kMaxVal>
  struct Helper<ConstRefCounter<kMaxVal, kMaxVal>> {
    using type = EndConstRefVal<ConstRefCounter>;
  };

  template <typename T>
  using type = typename Helper<T>::type;
};

// Note: You cannot simply check for type equivalence as the underlying const&
// that is formed may not refer to the same value, even if they are equivalent.
static_assert(Next_t<ConstRefCounter<kTheValue0, kMaxValSizeGlobal>>::val == 1);
static_assert(
    Next_t<Next_t<ConstRefCounter<kTheValue0, kMaxValSizeGlobal>>>::val == 2);
static_assert(
    Next_t<
        Next_t<Next_t<ConstRefCounter<kTheValue0, kMaxValSizeGlobal>>>>::val ==
    3);

}  // namespace jni::metaprogramming

#endif  // NO
