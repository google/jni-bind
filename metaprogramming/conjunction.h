#ifndef JNI_BIND_METAPROGRAMMING_CONJUNCTION_H_
#define JNI_BIND_METAPROGRAMMING_CONJUNCTION_H_

#include <type_traits>

#include "metaprogramming/invoke.h"
#include "metaprogramming/next.h"

namespace jni::metaprogramming {

template <typename Lambda>
struct Conjunction;

template <typename Lambda, bool cur_val, typename Iter1, typename Iter2>
struct ConjunctionHelper {
  static constexpr bool value =
      cur_val && Invoke_t<Lambda, Iter1>::value &&
      Conjunction<Lambda>::template value<Next_t<Iter1>, Iter2>;
  using type = Invoke_t<Lambda, Iter1>;
};

template <typename Lambda, bool cur_val, typename IterEnd>
struct ConjunctionHelper<Lambda, cur_val, IterEnd, IterEnd> {
  static constexpr bool value = cur_val && Invoke_t<Lambda, IterEnd>::value;
};

template <typename Lambda>
struct Conjunction {
  template <typename Iter1, typename Iter2>
  static constexpr bool value =
      ConjunctionHelper<Lambda, true, Iter1, Iter2>::value;

  template <typename Iter1, typename Iter2>
  using type = std::bool_constant<value<Iter1, Iter2>>;
};

// Applies lambda from |Iter1| to |Iter2| inclusive.
template <typename Lambda, typename Iter1, typename Iter2>
static constexpr bool Conjunction_v =
    Conjunction<Lambda>::template value<Iter1, Iter2>;

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_CONJUNCTION_H_
