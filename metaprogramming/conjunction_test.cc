#include "metaprogramming/conjunction.h"

#include <cstddef>
#include <type_traits>

#include "metaprogramming/invoke.h"
#include "metaprogramming/next.h"

namespace {

using ::jni::metaprogramming::Conjunction_v;
using ::jni::metaprogramming::Invoke_t;
using ::jni::metaprogramming::NextVal;  // NOLINT

template <std::size_t I>
struct ValCounter {
  static constexpr std::size_t value = I;
};

}  // namespace

template <>
struct NextVal<ValCounter> {
  template <typename ValCounter>
  struct Helper;

  template <std::size_t I>
  struct Helper<ValCounter<I>> {
    using type = ValCounter<I + 1>;
  };

  template <typename T>
  using type = typename Helper<T>::type;
};

struct Not3 {
  template <typename T>
  static constexpr bool value = T::value != 3;

  template <typename T>
  using type = std::bool_constant<value<T>>;
};

static_assert(Invoke_t<Not3, std::integral_constant<std::size_t, 0>>::value);
static_assert(Invoke_t<Not3, std::integral_constant<std::size_t, 1>>::value);
static_assert(Invoke_t<Not3, std::integral_constant<std::size_t, 2>>::value);
static_assert(!Invoke_t<Not3, std::integral_constant<std::size_t, 3>>::value);
static_assert(Invoke_t<Not3, std::integral_constant<std::size_t, 4>>::value);
static_assert(Invoke_t<Not3, std::integral_constant<std::size_t, 5>>::value);

using Iter1 = ValCounter<1>;
using Iter2 = ValCounter<2>;
using Iter3 = ValCounter<3>;
using Iter4 = ValCounter<4>;
using Iter5 = ValCounter<5>;

// Iterators spanning 1 element.
static_assert(Conjunction_v<Not3, Iter1, Iter1>);
static_assert(Conjunction_v<Not3, Iter2, Iter2>);
static_assert(!Conjunction_v<Not3, Iter3, Iter3>);
static_assert(Conjunction_v<Not3, Iter4, Iter4>);
static_assert(Conjunction_v<Not3, Iter5, Iter5>);

// Iterators spanning multiple elements (starting at 1).
static_assert(Conjunction_v<Not3, Iter1, Iter2>);
static_assert(!Conjunction_v<Not3, Iter1, Iter3>);
static_assert(!Conjunction_v<Not3, Iter1, Iter4>);
static_assert(!Conjunction_v<Not3, Iter1, Iter5>);

// Iterators spanning multiple elements (starting at 2).
static_assert(!Conjunction_v<Not3, Iter2, Iter3>);
static_assert(!Conjunction_v<Not3, Iter2, Iter4>);
static_assert(!Conjunction_v<Not3, Iter2, Iter5>);

// Iterators spanning multiple elements (starting at 3).
static_assert(!Conjunction_v<Not3, Iter3, Iter4>);
static_assert(!Conjunction_v<Not3, Iter3, Iter5>);

// Iterators spanning multiple elements (starting at 4).
static_assert(Conjunction_v<Not3, Iter4, Iter5>);
