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

#ifndef JNI_BIND_IMPLEMENTATION_PROXY_DEFINITIONS_PROTO_H_
#define JNI_BIND_IMPLEMENTATION_PROXY_DEFINITIONS_PROTO_H_

// IWYU pragma: private, include "third_party/jni_wrapper/proto_bind.h"

#include <cstdint>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

#include "bind_lang/metaprogramming/corpus.h"
#include "bind_lang/proxy/generic_proxy_helper.h"
#include "bind_lang/proxy/is_convertible_key.h"
#include "bind_lang/proxy/proxy_base.h"
#include "bind_lang/ref_base.h"
#include "implementation/proto_template.h"
#include "proto_dep.h"

namespace proto {

struct ProtoUserDefinedCorpusTag {};

template <const auto& template_v_>
class Message;

// `ProtoProxy` reuses the language-agnostic `ProxyBase`,
// `ConvertibleIntegralProxyBase`, `ObjectProxyBase`, `GenericProxyHelper`, and
// `IsConvertibleKey` from `bind_lang/proxy/` (shared with `jni::Proxy`).
template <typename CDecl, typename Enable = void>
struct ProtoProxy : public bind_lang::ProxyBase<CDecl> {};

using AllProtoKeys = bind_lang::metaprogramming::Corpus_t<
    ProtoUserDefinedCorpusTag, void, bool, int32_t, uint32_t, int64_t, uint64_t,
    float, double, std::string, google::protobuf::Message*>;

template <typename TUndecayed>
struct ProtoProxyHelper
    : public bind_lang::GenericProxyHelper<ProtoProxy, AllProtoKeys,
                                           TUndecayed> {};

template <typename T>
using ProtoProxy_t = typename ProtoProxyHelper<T>::Proxy_t;

template <typename T>
using ProtoIndex_t = typename ProtoProxyHelper<T>::Index;

template <typename T>
using ProtoCDecl_t = typename ProtoProxyHelper<T>::CDecl;

template <typename T, typename Id>
using ProtoReturn_t = typename ProtoProxyHelper<T>::template AsReturn_t<Id>;

template <typename T>
using ProtoAsDecl_t = typename ProtoProxyHelper<T>::AsDecl_t;

////////////////////////////////////////////////////////////////////////////////
// Primitive & Integral Proto Proxy Definitions (shared via `bind_lang/proxy/`)
////////////////////////////////////////////////////////////////////////////////
template <typename VoidType>
struct ProtoProxy<VoidType, std::enable_if_t<std::is_same_v<VoidType, void>>>
    : public bind_lang::ProxyBase<void> {};

template <typename Int32Type>
struct ProtoProxy<Int32Type,
                  std::enable_if_t<std::is_same_v<Int32Type, int32_t>>>
    : public bind_lang::ConvertibleIntegralProxyBase<int32_t, int32_t, int16_t,
                                                     int8_t> {};

template <typename UInt32Type>
struct ProtoProxy<UInt32Type,
                  std::enable_if_t<std::is_same_v<UInt32Type, uint32_t>>>
    : public bind_lang::ConvertibleIntegralProxyBase<uint32_t, uint32_t,
                                                     uint16_t, uint8_t> {
  template <typename OverloadSelection, typename T>
  static constexpr bool kViable =
      std::is_same_v<T, uint32_t> || std::is_same_v<T, uint16_t> ||
      std::is_same_v<T, uint8_t> || std::is_same_v<T, int32_t>;

  static uint32_t ProxyAsArg(uint32_t val) { return val; }

  template <typename T, typename = std::enable_if_t<
                            std::is_integral_v<std::decay_t<T>> &&
                            !std::is_same_v<std::decay_t<T>, bool> &&
                            !std::is_same_v<std::decay_t<T>, uint32_t>>>
  static uint32_t ProxyAsArg(T val) {
    return static_cast<uint32_t>(val);
  }
};

template <typename Int64Type>
struct ProtoProxy<Int64Type,
                  std::enable_if_t<std::is_same_v<Int64Type, int64_t>>>
    : public bind_lang::ConvertibleIntegralProxyBase<
          int64_t, int64_t, long, long long> {  // NOLINT(runtime/int)
  template <typename OverloadSelection, typename T>
  static constexpr bool kViable =
      std::is_same_v<T, int64_t> ||
      std::is_same_v<T, long> ||       // NOLINT(runtime/int)
      std::is_same_v<T, long long> ||  // NOLINT(runtime/int)
      std::is_same_v<T, int32_t> || std::is_same_v<T, int16_t> ||
      std::is_same_v<T, int8_t>;

  static int64_t ProxyAsArg(int64_t val) { return val; }

  template <typename T, typename = std::enable_if_t<
                            std::is_integral_v<std::decay_t<T>> &&
                            !std::is_same_v<std::decay_t<T>, bool> &&
                            !std::is_same_v<std::decay_t<T>, int64_t>>>
  static int64_t ProxyAsArg(T val) {
    return static_cast<int64_t>(val);
  }
};

template <typename UInt64Type>
struct ProtoProxy<UInt64Type,
                  std::enable_if_t<std::is_same_v<UInt64Type, uint64_t>>>
    : public bind_lang::ConvertibleIntegralProxyBase<
          uint64_t, uint64_t, unsigned long,  // NOLINT(runtime/int)
          unsigned long long> {               // NOLINT(runtime/int)
  template <typename OverloadSelection, typename T>
  static constexpr bool kViable =
      std::is_same_v<T, uint64_t> ||
      std::is_same_v<T, unsigned long> ||       // NOLINT(runtime/int)
      std::is_same_v<T, unsigned long long> ||  // NOLINT(runtime/int)
      std::is_same_v<T, uint32_t> || std::is_same_v<T, int32_t> ||
      std::is_same_v<T, uint16_t> || std::is_same_v<T, uint8_t>;

  static uint64_t ProxyAsArg(uint64_t val) { return val; }

  template <typename T, typename = std::enable_if_t<
                            std::is_integral_v<std::decay_t<T>> &&
                            !std::is_same_v<std::decay_t<T>, bool> &&
                            !std::is_same_v<std::decay_t<T>, uint64_t>>>
  static uint64_t ProxyAsArg(T val) {
    return static_cast<uint64_t>(val);
  }
};

template <typename DoubleType>
struct ProtoProxy<DoubleType,
                  std::enable_if_t<std::is_same_v<DoubleType, double>>>
    : public bind_lang::ProxyBase<double> {
  template <typename OverloadSelection, typename T>
  static constexpr bool kViable =
      std::is_same_v<T, double> || std::is_same_v<T, float>;

  static double ProxyAsArg(double val) { return val; }
  static double ProxyAsArg(float val) { return static_cast<double>(val); }
};

////////////////////////////////////////////////////////////////////////////////
// String Proto Proxy Definition.
////////////////////////////////////////////////////////////////////////////////
template <typename StdString>
struct ProtoProxy<StdString,
                  std::enable_if_t<std::is_same_v<StdString, std::string>>>
    : public bind_lang::ProxyBase<std::string> {
  using AsDecl = std::tuple<std::string, const char*, std::string_view>;
  using AsArg = std::tuple<std::string, char*, const char*, std::string_view>;

  template <typename Id>
  using AsReturn = std::string;

  template <typename SelectionT, typename T>
  static constexpr bool kViable =
      bind_lang::IsConvertibleKey<T>::template value<std::string> ||
      bind_lang::IsConvertibleKey<T>::template value<char*> ||
      bind_lang::IsConvertibleKey<T>::template value<const char*> ||
      bind_lang::IsConvertibleKey<T>::template value<std::string_view>;

  static std::string ProxyAsArg(std::string s) { return s; }
  static std::string ProxyAsArg(std::string_view s) { return std::string{s}; }
  static std::string ProxyAsArg(const char* s) {
    return std::string{s != nullptr ? s : ""};
  }
};

////////////////////////////////////////////////////////////////////////////////
// Nested Message Proxy Definition (reuses bind_lang::ObjectProxyBase).
////////////////////////////////////////////////////////////////////////////////
template <typename ProtoMsgPtr>
struct ProtoProxy<
    ProtoMsgPtr,
    std::enable_if_t<std::is_same_v<ProtoMsgPtr, google::protobuf::Message*>>>
    : public bind_lang::ObjectProxyBase<google::protobuf::Message*,
                                        TemplateBase> {
  template <typename IdT, typename T>
  struct ContextualViabilityHelper {
    using DecayT = std::decay_t<T>;
    using PointeeT = std::remove_cv_t<std::remove_pointer_t<DecayT>>;
    static constexpr bool kViable =
        (std::is_pointer_v<DecayT> &&
         std::is_base_of_v<google::protobuf::Message, PointeeT>) ||
        std::is_base_of_v<google::protobuf::Message, DecayT>;
  };

  template <typename IdT, const auto& sub_template_v>
  struct ContextualViabilityHelper<IdT, Message<sub_template_v>> {
    static constexpr bool kViable = (sub_template_v == IdT::Val());
  };

  template <typename IdT, typename T>
  static constexpr bool kViable = ContextualViabilityHelper<IdT, T>::kViable;

  template <typename Id, bool kIsSubMsg = IsSubMessage_v<typename Id::RawValT>>
  struct Helper {
    using type = Message<Id::RawValT::kTemplate>;
  };

  template <typename Id>
  struct Helper<Id, false> {
    static constexpr auto kSubTemplate{Id::Val()};
    using type = Message<kSubTemplate>;
  };

  template <typename Id>
  using AsReturn = typename Helper<Id>::type;

  static const google::protobuf::Message* ProxyAsArg(
      const google::protobuf::Message* msg) {
    return msg;
  }

  static const google::protobuf::Message* ProxyAsArg(
      const google::protobuf::Message& msg) {
    return &msg;
  }

  // Note: Unlike JNI's `jobject` (where rvalue `LocalObject`s release their
  // local reference across the JNI C boundary), `SetMessageField` and
  // `AddMessageField` copy the sub-message contents (`CopyFrom`). Therefore,
  // rvalue `proto::Message` temporaries must retain ownership so their
  // destructor deletes the heap-allocated `google::protobuf::Message*` at the
  // end of the full-expression.
  static const google::protobuf::Message* ProxyAsArg(
      const bind_lang::RefBase<google::protobuf::Message*>& t) {
    return static_cast<google::protobuf::Message*>(t);
  }
};

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_PROXY_DEFINITIONS_PROTO_H_
