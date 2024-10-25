/*
 * Copyright 2024 Google LLC
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

#include "string_literal.h"

#include <cstddef>

using jni::metaprogramming::StringLiteral;

namespace {

template <size_t N>
constexpr StringLiteral<N> Foo(const char (&str)[N]) {
  StringLiteral b{str};
  return b;
}

// String literals can now peer through callsites. This was previously not
// possible in C++17 and should hopefully simplify method selection.
static_assert(Foo("Testing") == StringLiteral{"Testing"});
static_assert(Foo("abcdefg") != StringLiteral{"Testing"});
static_assert(Foo("Testing") != StringLiteral{"abcdefg"});
static_assert(Foo("Bar") != StringLiteral{"Testing"});
static_assert(Foo("Testing") != StringLiteral{"Bar"});

// This represents a container class. Any classes currently constructed with
// const char* will need to migrate to this new form which explicitly captures
// length.
template <std::size_t N>
struct SampleContainer {
  constexpr SampleContainer(const char (&val_)[N]) : val_(val_) {}

  StringLiteral<N> val_;
};

template <std::size_t N>
SampleContainer(const char (&val_)[N]) -> SampleContainer<N>;

static_assert(SampleContainer("Testing").val_ == StringLiteral{"Testing"});
static_assert(SampleContainer("abcdefg").val_ != StringLiteral{"Testing"});
static_assert(SampleContainer("Foo").val_ != StringLiteral{"Testing"});

}  // namespace
