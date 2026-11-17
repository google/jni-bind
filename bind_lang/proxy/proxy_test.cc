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

#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>

#include <gtest/gtest.h>
#include "bind_lang/proxy/generic_proxy_helper.h"
#include "bind_lang/proxy/is_convertible_key.h"
#include "bind_lang/proxy/proxy_base.h"
#include "bind_lang/ref_base.h"

namespace {

using ::bind_lang::ConvertibleIntegralProxyBase;
using ::bind_lang::GenericProxyHelper;
using ::bind_lang::IsConvertibleKey_v;
using ::bind_lang::ObjectProxyBase;
using ::bind_lang::ProxyBase;

// A toy binding language, entirely independent of JNI, used to demonstrate
// that the proxy machinery is language-agnostic.
struct ToyHandleImpl {};
using ToyHandle = ToyHandleImpl*;

struct ToyObjectDecl {};
struct ToyExtraArg {};

// A scoped handle that can be passed as an lvalue (borrowed) or an rvalue
// (released).
class ToyScopedHandle : public bind_lang::RefBase<ToyHandle> {
 public:
  using bind_lang::RefBase<ToyHandle>::RefBase;
};

template <typename CDecl, typename Enable = void>
struct ToyProxy : public ProxyBase<CDecl> {};

template <typename CDecl>
struct ToyProxy<CDecl, std::enable_if_t<std::is_same_v<CDecl, int64_t>>>
    : public ConvertibleIntegralProxyBase<int64_t, int64_t, int32_t> {};

template <typename CDecl>
struct ToyProxy<CDecl, std::enable_if_t<std::is_same_v<CDecl, ToyHandle>>>
    : public ObjectProxyBase<ToyHandle, ToyObjectDecl, ToyExtraArg> {};

using ToyKeys = std::tuple<void, bool, int64_t, ToyHandle>;

template <typename T>
using ToyProxyHelper = GenericProxyHelper<ToyProxy, ToyKeys, T>;

////////////////////////////////////////////////////////////////////////////////
// `IsConvertibleKey`.
////////////////////////////////////////////////////////////////////////////////
struct Base {};
struct Derived : Base {};

static_assert(IsConvertibleKey_v<int, int>);
static_assert(IsConvertibleKey_v<int, const int&>);
static_assert(IsConvertibleKey_v<const char*, const char[4]>);
static_assert(IsConvertibleKey_v<Base, Derived>);
static_assert(IsConvertibleKey_v<Derived, Base>);
static_assert(!IsConvertibleKey_v<int, float>);

////////////////////////////////////////////////////////////////////////////////
// `GenericProxyHelper` selection.
////////////////////////////////////////////////////////////////////////////////
static_assert(std::is_same_v<ToyProxyHelper<bool>::CDecl, bool>);
static_assert(std::is_same_v<ToyProxyHelper<const bool&>::CDecl, bool>);
static_assert(std::is_same_v<ToyProxyHelper<int64_t>::CDecl, int64_t>);
static_assert(std::is_same_v<ToyProxyHelper<int32_t>::CDecl, int64_t>);
static_assert(std::is_same_v<ToyProxyHelper<ToyHandle>::CDecl, ToyHandle>);
static_assert(std::is_same_v<ToyProxyHelper<ToyObjectDecl>::CDecl, ToyHandle>);
static_assert(std::is_same_v<ToyProxyHelper<ToyExtraArg>::CDecl, ToyHandle>);
static_assert(
    std::is_same_v<ToyProxyHelper<ToyScopedHandle>::CDecl, ToyHandle>);
static_assert(
    std::is_same_v<ToyProxyHelper<ToyHandle>::Proxy_t, ToyProxy<ToyHandle>>);
static_assert(std::is_same_v<ToyProxyHelper<void>::AsReturn_t<int>, void>);

////////////////////////////////////////////////////////////////////////////////
// `ProxyBase`.
////////////////////////////////////////////////////////////////////////////////
static_assert(ProxyBase<bool>::kViable<void, bool>);
static_assert(!ProxyBase<bool>::kViable<void, int64_t>);
static_assert(std::is_same_v<ProxyBase<bool>::AsArg, std::tuple<bool>>);

TEST(ProxyBaseTest, ProxyAsArgIsPassthrough) {
  EXPECT_TRUE(ProxyBase<bool>::ProxyAsArg(true));
  EXPECT_FALSE(ProxyBase<bool>::ProxyAsArg(false));
}

////////////////////////////////////////////////////////////////////////////////
// `ConvertibleIntegralProxyBase`.
////////////////////////////////////////////////////////////////////////////////
using ToyIntegralProxy = ToyProxy<int64_t>;

static_assert(ToyIntegralProxy::kViable<void, int64_t>);
static_assert(ToyIntegralProxy::kViable<void, int32_t>);
static_assert(!ToyIntegralProxy::kViable<void, bool>);
static_assert(
    std::is_same_v<ToyIntegralProxy::AsArg, std::tuple<int64_t, int32_t>>);
static_assert(
    std::is_same_v<ToyIntegralProxy::AsDecl, std::tuple<int64_t, int32_t>>);

TEST(ConvertibleIntegralProxyBaseTest, LowersToCanonicalType) {
  static_assert(std::is_same_v<decltype(ToyIntegralProxy::ProxyAsArg(
                                   std::declval<int32_t>())),
                               int64_t>);
  static_assert(std::is_same_v<decltype(ToyIntegralProxy::ProxyAsArg(
                                   std::declval<int64_t>())),
                               int64_t>);

  EXPECT_EQ(ToyIntegralProxy::ProxyAsArg(int32_t{-5}), int64_t{-5});
  EXPECT_EQ(ToyIntegralProxy::ProxyAsArg(int64_t{1} << 40), int64_t{1} << 40);
}

////////////////////////////////////////////////////////////////////////////////
// `ObjectProxyBase`.
////////////////////////////////////////////////////////////////////////////////
using ToyObjectProxy = ToyProxy<ToyHandle>;

static_assert(
    std::is_same_v<ToyObjectProxy::AsDecl, std::tuple<ToyObjectDecl>>);
static_assert(
    std::is_same_v<
        ToyObjectProxy::AsArg,
        std::tuple<ToyHandle, bind_lang::RefBase<ToyHandle>, ToyExtraArg>>);

TEST(ObjectProxyBaseTest, RawHandleIsPassthrough) {
  ToyHandleImpl impl;
  EXPECT_EQ(ToyObjectProxy::ProxyAsArg(&impl), &impl);
}

TEST(ObjectProxyBaseTest, LvalueScopedHandleIsBorrowed) {
  ToyHandleImpl impl;
  ToyScopedHandle scoped{&impl};

  EXPECT_EQ(ToyObjectProxy::ProxyAsArg(scoped), &impl);
  EXPECT_EQ(static_cast<ToyHandle>(scoped), &impl);
}

TEST(ObjectProxyBaseTest, RvalueScopedHandleIsReleased) {
  ToyHandleImpl impl;
  ToyScopedHandle scoped{&impl};

  EXPECT_EQ(ToyObjectProxy::ProxyAsArg(std::move(scoped)), &impl);
  // NOLINTNEXTLINE(bugprone-use-after-move)
  EXPECT_EQ(static_cast<ToyHandle>(scoped), nullptr);
}

}  // namespace
