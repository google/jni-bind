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

#include "metaprogramming/vals_equal.h"

#include "metaprogramming/vals.h"

namespace {

using ::jni::metaprogramming::Vals;
using ::jni::metaprogramming::ValsConst;
using ::jni::metaprogramming::ValsConstRef;
using ::jni::metaprogramming::ValsEqual_v;
using ::jni::metaprogramming::ValsRef;

template <const auto... Vs>
struct A {};

static constexpr int kOne = 1;
static constexpr int kTwo = 2;

////////////////////////////////////////////////////////////////////////////////
// Types evaluate equal.
////////////////////////////////////////////////////////////////////////////////
static_assert(ValsEqual_v<int, int>);
static_assert(!ValsEqual_v<int, float>);

////////////////////////////////////////////////////////////////////////////////
// Vals compare equally (xref, auto, l-value, const lvalue).
////////////////////////////////////////////////////////////////////////////////
static_assert(ValsEqual_v<Vals<1>, Vals<1>>);
static_assert(ValsEqual_v<Vals<kOne>, Vals<1>>);
static_assert(ValsEqual_v<ValsRef<kOne>, Vals<1>>);
static_assert(ValsEqual_v<ValsConst<kOne>, Vals<1>>);
static_assert(ValsEqual_v<ValsConstRef<kOne>, Vals<1>>);

static_assert(ValsEqual_v<Vals<1>, Vals<kOne>>);
static_assert(ValsEqual_v<Vals<kOne>, Vals<kOne>>);
static_assert(ValsEqual_v<ValsRef<kOne>, Vals<kOne>>);
static_assert(ValsEqual_v<ValsConst<kOne>, Vals<kOne>>);
static_assert(ValsEqual_v<ValsConstRef<kOne>, Vals<kOne>>);

static_assert(ValsEqual_v<Vals<1>, ValsConst<kOne>>);
static_assert(ValsEqual_v<Vals<kOne>, ValsConst<kOne>>);
static_assert(ValsEqual_v<ValsRef<kOne>, ValsConst<kOne>>);
static_assert(ValsEqual_v<ValsConst<kOne>, ValsConst<kOne>>);
static_assert(ValsEqual_v<ValsConstRef<kOne>, ValsConst<kOne>>);

static_assert(ValsEqual_v<Vals<1>, ValsConstRef<kOne>>);
static_assert(ValsEqual_v<Vals<kOne>, ValsConstRef<kOne>>);
static_assert(ValsEqual_v<ValsRef<kOne>, ValsConstRef<kOne>>);
static_assert(ValsEqual_v<ValsConst<kOne>, ValsConstRef<kOne>>);
static_assert(ValsEqual_v<ValsConstRef<kOne>, ValsConstRef<kOne>>);

////////////////////////////////////////////////////////////////////////////////
// Other containers compare equally (xref, auto, l-value, const lvalue).
// Note, to fully test this, A should fully replicate Vals.
// Those paths are largely exercised above, so I've left this out.
////////////////////////////////////////////////////////////////////////////////
static_assert(ValsEqual_v<Vals<1>, A<1>>);
static_assert(ValsEqual_v<Vals<kOne>, A<1>>);
static_assert(ValsEqual_v<ValsRef<kOne>, A<1>>);
static_assert(ValsEqual_v<ValsConst<kOne>, A<1>>);
static_assert(ValsEqual_v<ValsConstRef<kOne>, A<1>>);

static_assert(ValsEqual_v<Vals<1>, A<kOne>>);
static_assert(ValsEqual_v<Vals<kOne>, A<kOne>>);
static_assert(ValsEqual_v<ValsRef<kOne>, A<kOne>>);
static_assert(ValsEqual_v<ValsConst<kOne>, A<kOne>>);
static_assert(ValsEqual_v<ValsConstRef<kOne>, A<kOne>>);

////////////////////////////////////////////////////////////////////////////////
// Unequal values compare unequally.
////////////////////////////////////////////////////////////////////////////////
static_assert(!ValsEqual_v<Vals<1>, Vals<2>>);
static_assert(!ValsEqual_v<Vals<1>, Vals<2>>);
static_assert(!ValsEqual_v<Vals<kOne>, Vals<2>>);
static_assert(!ValsEqual_v<ValsRef<kOne>, Vals<2>>);
static_assert(!ValsEqual_v<ValsConst<kOne>, Vals<2>>);
static_assert(!ValsEqual_v<ValsConstRef<kOne>, Vals<2>>);

static_assert(!ValsEqual_v<Vals<kOne>, Vals<kTwo>>);
static_assert(!ValsEqual_v<ValsRef<kOne>, Vals<kTwo>>);
static_assert(!ValsEqual_v<ValsConst<kOne>, Vals<kTwo>>);
static_assert(!ValsEqual_v<ValsConstRef<kOne>, Vals<kTwo>>);

static_assert(!ValsEqual_v<Vals<1>, ValsConst<kTwo>>);
static_assert(!ValsEqual_v<Vals<kOne>, ValsConst<kTwo>>);
static_assert(!ValsEqual_v<ValsRef<kOne>, ValsConst<kTwo>>);
static_assert(!ValsEqual_v<ValsConst<kOne>, ValsConst<kTwo>>);
static_assert(!ValsEqual_v<ValsConstRef<kOne>, ValsConst<kTwo>>);

static_assert(!ValsEqual_v<Vals<1>, ValsConstRef<kTwo>>);
static_assert(!ValsEqual_v<Vals<kOne>, ValsConstRef<kTwo>>);
static_assert(!ValsEqual_v<ValsRef<kOne>, ValsConstRef<kTwo>>);
static_assert(!ValsEqual_v<ValsConst<kOne>, ValsConstRef<kTwo>>);
static_assert(!ValsEqual_v<ValsConstRef<kOne>, ValsConstRef<kTwo>>);

}  // namespace
