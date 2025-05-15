#ifndef JNI_BIND_IMPLEMENTATION_PROXY_TEMPORARY_H_
#define JNI_BIND_IMPLEMENTATION_PROXY_TEMPORARY_H_

namespace jni {

struct ProxyTemporaryBase {};

template <typename T, typename DtorLambda>
struct ProxyTemporary : ProxyTemporaryBase {
  ProxyTemporary(T t) : t_(t) { }

  ~ProxyTemporary() {
    fprintf(stderr, "ProxyTemporary::~ProxyTemporary()\n");
    DtorLambda::Call(t_);
  }

  const T& Get() const { return t_; }

  T t_;
};


namespace detail {

template <typename T>
const auto& ForwardWithRefStripImpl(T&& t, std::true_type /* is_ref_base */) {
  return t.Get();
}

template <typename T>
decltype(auto) ForwardWithRefStripImpl(
    T&& t, std::false_type /* not ref base */) {
  return std::forward<T>(t); // perfect forward
}

} // namespace detail

template <typename T>
decltype(auto) ForwardWithRefStrip(T&& t) {
  using U = std::remove_reference_t<T>;
  return detail::ForwardWithRefStripImpl(
      std::forward<T>(t),
      std::is_base_of<ProxyTemporaryBase, U>{});
}

} // namespace jni

#endif // JNI_BIND_IMPLEMENTATION_PROXY_TEMPORARY_H_
