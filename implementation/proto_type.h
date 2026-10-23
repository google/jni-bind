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

#ifndef JNI_BIND_IMPLEMENTATION_PROTO_TYPE_H_
#define JNI_BIND_IMPLEMENTATION_PROTO_TYPE_H_

// IWYU pragma: private, include "third_party/jni_wrapper/proto_bind.h"

#include <cstddef>
#include <string_view>
#include <tuple>
#include <type_traits>

#include "implementation/proto_template.h"
#include "proto_dep.h"

namespace proto {

// Compile-time type descriptor bundle for a `proto::Template` instance.
// Mirrors `jni::JniT` in `implementation/jni_type.h`.
template <const auto& template_v_, std::size_t kRank_ = 0>
struct ProtoT {
  static constexpr std::size_t kRank = kRank_;

  static constexpr auto GetTemplate() { return template_v_; }

  static constexpr const auto& template_v = template_v_;
  using TemplateT = std::decay_t<decltype(template_v_)>;
  static constexpr TemplateT stripped_template_v{template_v_};

  static constexpr std::string_view kName{template_v_.name_};
  static constexpr bool kIsAnonymous =
      (kName == std::string_view{kAnonymousTemplateName});

  static constexpr std::size_t kNumFields =
      std::tuple_size_v<decltype(template_v_.fields_)>;

  using SpanType = google::protobuf::Message*;
  using StorageType = google::protobuf::Message*;
};

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_PROTO_TYPE_H_
