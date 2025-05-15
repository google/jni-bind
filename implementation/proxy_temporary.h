#ifndef JNI_BIND_IMPLEMENTATION_PROXY_TEMPORARY_H_
#define JNI_BIND_IMPLEMENTATION_PROXY_TEMPORARY_H_

namespace jni {

struct ProxyTemporaryBase {};

// Some values built in proxy_definitions are intended to be ephemeral.
// Because the full decorated LocalObject is not defined, we use this 
// class to proxy the value for the duration of `ProxyAsArg`, but allow it to
// immediately be destroyed after the call.
//
// See https://github.com/google/jni-bind/issues/414.
template <typename T, typename DtorLambda>
struct ProxyTemporary : ProxyTemporaryBase {
  ProxyTemporary(T t) : t_(t) { }

  ~ProxyTemporary() {
    DtorLambda::Call(t_);
  }

  const T& Get() const { return t_; }

  T t_;
};


namespace detail {

template <typename T>
const auto& ForwardWithProxyTemporaryStripImpl(T&& t, std::true_type /* is_ref_base */) {
  return t.Get();
}

template <typename T>
decltype(auto) ForwardWithProxyTemporaryStripImpl(
    T&& t, std::false_type /* not ref base */) {
  return std::forward<T>(t); // perfect forward
}

} // namespace detail

template <typename T>
decltype(auto) ForwardWithProxyTemporaryStrip(T&& t) {
  using U = std::remove_reference_t<T>;
  return detail::ForwardWithProxyTemporaryStripImpl(
      std::forward<T>(t),
      std::is_base_of<ProxyTemporaryBase, U>{});
}

} // namespace jni

#endif // JNI_BIND_IMPLEMENTATION_PROXY_TEMPORARY_H_
