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

#include <tuple>
#include <type_traits>

#include "corpus_tag.h"

struct CorpusTag {};

struct A {};
struct B {};
struct C {};

// Note: This ordering is intentional (see
// `::bind_lang::metaprogramming::Corpus`).
namespace bind_lang::metaprogramming {

template <>
struct UserDefined<CorpusTag> {
  using type = std::tuple<A, B, C>;
};

}  // namespace bind_lang::metaprogramming

#include "bind_lang/metaprogramming/corpus.h"
#include "bind_lang/metaprogramming/detect.h"

using ::bind_lang::metaprogramming::Corpus_t;
using ::bind_lang::metaprogramming::Detect_t;
using ::bind_lang::metaprogramming::UserDefined;

namespace {

static_assert(
    std::is_same_v<Detect_t<UserDefined, CorpusTag>, std::tuple<A, B, C>>);

static_assert(std::is_same_v<Corpus_t<CorpusTag, int, float>,
                             std::tuple<A, B, C, int, float>>);

}  // namespace
