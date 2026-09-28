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

#ifndef JNI_BIND_BIND_LANG_PROXY_PROXY_BASE_H_
#define JNI_BIND_BIND_LANG_PROXY_PROXY_BASE_H_

#include <tuple>
#include <type_traits>
#include <utility>

#include "bind_lang/proxy/is_convertible_key.h"
#include "bind_lang/ref_base.h"

namespace bind_lang {

// Default Proxy, all types and values are pure passthrough.
//
// A "Proxy" describes, for a single canonical declaration type (`CDecl`), which
// C++ types may be passed as arguments (`AsArg`), which types may be used to
// declare it (`AsDecl`), what is returned (`AsReturn`), and how an argument is
// lowered to the runtime representation (`ProxyAsArg`).
template <typename Key_>
struct ProxyBase {
  using Key = Key_;

  using CDecl = Key_;

  template <typename>
  using AsReturn = Key_;

  using AsArg = std::tuple<Key_>;
  using AsDecl = std::tuple<Key_>;

  // Returns by value (`auto`, not `decltype(auto)`) so that lvalue primitive
  // arguments produce prvalue temporaries that can bind to rvalue-reference
  // parameters in downstream helpers (e.g. `FieldHelper::SetValue(...,
  // Raw&&)`).
  //
  // Note: Because this returns by value, it is only intended for trivially
  // copyable types to avoid performance penalties on expensive custom types.
  template <typename T>
  static auto ProxyAsArg(T&& t) {
    static_assert(
        std::is_trivially_copyable_v<std::decay_t<T>>,
        "ProxyBase is only intended for trivially copyable types.");
    return std::forward<T>(t);
  }

  template <typename InputParamSelectionT, typename T>
  static constexpr bool kViable = IsConvertibleKey_v<Key_, T>;
};

// Shared base for primitive types that alias multiple C++ declarations (e.g.
// `jchar`/`char`, `jboolean`/`bool`, `jlong`/`long`).
//
// Any of `ViableTs` may be passed as an argument or used as a declaration.
// Arguments are lowered to `CanonicalT`.
template <typename CanonicalT, typename... ViableTs>
struct ConvertibleIntegralProxyBase : public ProxyBase<CanonicalT> {
  using AsArg = std::tuple<ViableTs...>;
  using AsDecl = std::tuple<ViableTs...>;

  template <typename InputParamSelectionT, typename T>
  static constexpr bool kViable = (IsConvertibleKey_v<ViableTs, T> || ...);

  static CanonicalT ProxyAsArg(CanonicalT val) { return val; }

  // Viable types that are not `CanonicalT` (e.g. `long` when `jlong` is a
  // distinct type, as on some ARM targets) are converted to `CanonicalT`.
  template <typename T,
            typename = std::enable_if_t<(std::is_same_v<T, ViableTs> || ...) &&
                                        !std::is_same_v<CanonicalT, T>>>
  static CanonicalT ProxyAsArg(T val) {
    return static_cast<CanonicalT>(val);
  }
};

// Shared base for handle-backed composite objects (e.g. `jobject` in JNI Bind,
// `google::protobuf::Message*` in Proto Bind).
//
// `RawHandleT` is the runtime handle, `DeclBaseT` is the declaration type
// (e.g. `jni::Object`), and `ExtraAsArgTs` are additional argument types the
// language accepts (e.g. `jni::LoaderTag`).
template <typename RawHandleT, typename DeclBaseT, typename... ExtraAsArgTs>
struct ObjectProxyBase : public ProxyBase<RawHandleT> {
  using AsDecl = std::tuple<DeclBaseT>;
  using AsArg = std::tuple<RawHandleT, RefBase<RawHandleT>, ExtraAsArgTs...>;

  static RawHandleT ProxyAsArg(RawHandleT obj) { return obj; }

  // Applies for lvalue scoped handles (e.g. local and global objects).
  template <typename T>
  static RawHandleT ProxyAsArg(T& t) {
    return RawHandleT{t};
  }

  // Applies for rvalue scoped handles (releases ownership across boundary).
  template <typename T>
  static RawHandleT ProxyAsArg(T&& t) {
    return t.Release();
  }
};

}  // namespace bind_lang

#endif  // JNI_BIND_BIND_LANG_PROXY_PROXY_BASE_H_
