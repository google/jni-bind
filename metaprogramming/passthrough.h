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
bool operator==(const T& lhs, const Passthrough<U, CustomDeleter>& rhs) {
  return lhs == static_cast<T>(rhs.t_);
}

template <typename T, typename U, typename CustomDeleter>
bool operator==(const Passthrough<U, CustomDeleter>& lhs, const T& rhs) {
  return static_cast<T>(lhs.t_) == rhs;
}

template <typename T, typename U, typename CustomDeleter>
bool operator!=(const T& lhs, const Passthrough<U, CustomDeleter>& rhs) {
  return !(lhs == rhs);
}

template <typename T, typename U, typename CustomDeleter>
bool operator!=(const Passthrough<U, CustomDeleter>& lhs, const T& rhs) {
  return !(lhs == rhs);
}

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_PASSTHROUGH_H_
