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

#include "implementation/corpus_tag.h"

struct A {};
struct B {};
struct C {};

// Note: This ordering is intentional (see `::jni::Corpus`).
namespace jni {

template <>
struct UserDefined<JniUserDefinedCorpusTag> {
  using type = std::tuple<A, B, C>;
};

}  // namespace jni

#include "implementation/corpus.h"
#include "metaprogramming/detect.h"

using ::jni::Corpus_t;
using ::jni::JniUserDefinedCorpusTag;
using ::jni::UserDefined;
using ::jni::metaprogramming::Detect_t;

namespace {

static_assert(
    std::is_same_v<Detect_t<jni::UserDefined, JniUserDefinedCorpusTag>,
                   std::tuple<A, B, C>>);

static_assert(std::is_same_v<Corpus_t<JniUserDefinedCorpusTag, int, float>,
                             std::tuple<A, B, C, int, float>>);

}  // namespace
