#ifndef JNI_BIND_METAPROGRAMMING_CLAMP_H_
#define JNI_BIND_METAPROGRAMMING_CLAMP_H_

#include <cstddef>
#include <limits>

namespace jni {

// Increments `val` by `increment_count`, wrapping to {0} at `max`.
// This can be useful for generalising tests over different size types.
template <typename T>
T Clamp(std::size_t increment_count, T val = T{0},
        T max = std::numeric_limits<T>::max()) {
  T ret = val;

  for (int i = 0; i < increment_count; ++i) {
    ret += T{1};
    while (ret >= max) {
      ret -= max;
    }
  }

  return ret;
}

}  // namespace jni

#endif  // JNI_BIND_METAPROGRAMMING_CLAMP_H_
