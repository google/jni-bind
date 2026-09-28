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

#ifndef JNI_BIND_IMPLEMENTATION_DESCRIPTOR_REF_H_
#define JNI_BIND_IMPLEMENTATION_DESCRIPTOR_REF_H_

// IWYU pragma: private, include "third_party/jni_wrapper/proto_bind.h"

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "third_party/absl/strings/str_cat.h"
#include "bind_lang/id_type.h"
#include "bind_lang/metaprogramming/double_locked_value.h"
#include "implementation/proto_field.h"
#include "implementation/proto_helper/proto_helper.h"
#include "implementation/proto_id.h"
#include "implementation/proto_template.h"
#include "implementation/proto_type.h"
#include "proto_dep.h"

namespace proto {

inline std::mutex& GetDescriptorCacheMutex() {  // NOLINT(build/c++11)
  static auto* mtx = new std::mutex{};          // NOLINT(build/c++11)
  return *mtx;
}

inline auto& GetDefaultLoadedDescriptorList() {
  static auto* ret_val =
      new std::vector<bind_lang::metaprogramming::DoubleLockedValue<
          const google::protobuf::Descriptor*>*>{};
  return *ret_val;
}

// Resolves (or dynamically synthesizes) a `google::protobuf::Descriptor` for a
// compile-time `ProtoT` schema. Mirrors `jni::ClassRef` in
// `implementation/class_ref.h`.
template <typename ProtoT_>
class DescriptorRef {
 public:
  static const google::protobuf::Descriptor* GetAndMaybeLoadDescriptor() {
    return descriptor_ref_.LoadAndMaybeInit([]() {
      {
        std::lock_guard<std::mutex> lock(  // NOLINT(build/c++11)
            GetDescriptorCacheMutex());
        GetDefaultLoadedDescriptorList().push_back(&descriptor_ref_);
      }
      const std::string message_name = EffectiveMessageName();
      const google::protobuf::Descriptor* existing =
          ProtoHelper::FindDescriptor(message_name.c_str());
      if (existing != nullptr) {
        return existing;
      }

      const google::protobuf::FileDescriptorProto file_proto =
          BuildFileDescriptorProto(message_name);
      return ProtoHelper::RegisterDynamicDescriptor(file_proto,
                                                    message_name.c_str());
    });
  }

  static constexpr uint64_t SchemaHash() {
    return SchemaHashImpl(std::make_index_sequence<ProtoT_::kNumFields>{});
  }

  static std::string EffectiveMessageName() {
    if constexpr (!ProtoT_::kIsAnonymous) {
      return std::string{ProtoT_::kName};
    } else {
      constexpr uint64_t kHash = SchemaHash();
      return absl::StrCat("ProtoBindAnon_", kHash);
    }
  }

 private:
  static inline bind_lang::metaprogramming::DoubleLockedValue<
      const google::protobuf::Descriptor*>
      descriptor_ref_;

  template <std::size_t... Is>
  static constexpr bool HasZeroTagImpl(std::index_sequence<Is...>) {
    return ((ProtoId<ProtoT_, bind_lang::IdType::FIELD, Is>::kTag == 0) || ...);
  }

  static constexpr bool HasZeroTag() {
    return HasZeroTagImpl(std::make_index_sequence<ProtoT_::kNumFields>{});
  }

  template <typename CDeclT>
  static constexpr google::protobuf::FieldDescriptorProto::Type MapFieldType() {
    if constexpr (std::is_same_v<CDeclT, bool>) {
      return google::protobuf::FieldDescriptorProto::TYPE_BOOL;
    } else if constexpr (std::is_same_v<CDeclT, int32_t>) {
      return google::protobuf::FieldDescriptorProto::TYPE_INT32;
    } else if constexpr (std::is_same_v<CDeclT, uint32_t>) {
      return google::protobuf::FieldDescriptorProto::TYPE_UINT32;
    } else if constexpr (std::is_same_v<CDeclT, int64_t>) {
      return google::protobuf::FieldDescriptorProto::TYPE_INT64;
    } else if constexpr (std::is_same_v<CDeclT, uint64_t>) {
      return google::protobuf::FieldDescriptorProto::TYPE_UINT64;
    } else if constexpr (std::is_same_v<CDeclT, float>) {
      return google::protobuf::FieldDescriptorProto::TYPE_FLOAT;
    } else if constexpr (std::is_same_v<CDeclT, double>) {
      return google::protobuf::FieldDescriptorProto::TYPE_DOUBLE;
    } else if constexpr (std::is_same_v<CDeclT, std::string>) {
      return google::protobuf::FieldDescriptorProto::TYPE_STRING;
    } else if constexpr (std::is_same_v<CDeclT, google::protobuf::Message*>) {
      return google::protobuf::FieldDescriptorProto::TYPE_MESSAGE;
    } else {
      static_assert(!std::is_same_v<CDeclT, CDeclT>,
                    "Unsupported field type in descriptor generation");
      return google::protobuf::FieldDescriptorProto::TYPE_MESSAGE;
    }
  }

  static constexpr google::protobuf::FieldDescriptorProto::Label MapFieldLabel(
      FieldLabel label) {
    switch (label) {
      case FieldLabel::OPTIONAL:
      case FieldLabel::REQUIRED:
        return google::protobuf::FieldDescriptorProto::LABEL_OPTIONAL;
      case FieldLabel::REPEATED:
        return google::protobuf::FieldDescriptorProto::LABEL_REPEATED;
    }
    return google::protobuf::FieldDescriptorProto::LABEL_OPTIONAL;
  }

  static constexpr uint64_t HashMix(uint64_t hash, std::string_view str) {
    for (char c : str) {
      hash = (hash ^ static_cast<uint64_t>(static_cast<unsigned char>(c))) *
             1099511628211ULL;
    }
    return hash;
  }

  static constexpr uint64_t HashMixInt(uint64_t hash, uint64_t val) {
    hash ^= val;
    hash *= 1099511628211ULL;
    return hash;
  }

  template <typename IdT,
            bool kIsSubMsg = IsSubMessage_v<typename IdT::RawValT>>
  struct SubTemplateHolder {
    static constexpr const auto& kVal = IdT::RawValT::kTemplate;
  };

  template <typename IdT>
  struct SubTemplateHolder<IdT, false> {
    static constexpr auto kVal = IdT::Val();
  };

  template <std::size_t I>
  static constexpr uint64_t HashOneField(uint64_t hash) {
    using IdT = ProtoId<ProtoT_, bind_lang::IdType::FIELD, I>;
    using CDeclT = typename IdT::MaterializeCDeclT;
    hash = HashMixInt(hash, IdT::kTag);
    hash = HashMixInt(hash, static_cast<uint64_t>(IdT::kLabel));
    hash = HashMixInt(hash, static_cast<uint64_t>(MapFieldType<CDeclT>()));
    hash = HashMix(hash, IdT::kName);
    if constexpr (IsSubMessage_v<typename IdT::RawValT>) {
      hash = HashMixInt(
          hash,
          DescriptorRef<ProtoT<SubTemplateHolder<IdT>::kVal>>::SchemaHash());
    }
    return hash;
  }

  template <std::size_t... Is>
  static constexpr uint64_t SchemaHashImpl(std::index_sequence<Is...>) {
    uint64_t hash = 14695981039346656037ULL;
    hash = HashMix(hash, ProtoT_::kName);
    hash = HashMixInt(hash, ProtoT_::kNumFields);
    ((hash = HashOneField<Is>(hash)), ...);
    return hash;
  }

  template <std::size_t I>
  static void PopulateFieldProto(google::protobuf::DescriptorProto& msg_proto) {
    using IdT = ProtoId<ProtoT_, bind_lang::IdType::FIELD, I>;
    using CDeclT = typename IdT::MaterializeCDeclT;

    google::protobuf::FieldDescriptorProto* field_proto = msg_proto.add_field();
    field_proto->set_name(IdT::kName.empty() ? absl::StrCat("field_", IdT::kTag)
                                             : std::string{IdT::kName});

    constexpr int kTagOffset = HasZeroTag() ? 1 : 0;
    field_proto->set_number(static_cast<int>(IdT::kTag) + kTagOffset);
    field_proto->set_label(MapFieldLabel(IdT::kLabel));
    field_proto->set_type(MapFieldType<CDeclT>());

    if constexpr (std::is_same_v<CDeclT, google::protobuf::Message*>) {
      using SubProtoT = ProtoT<SubTemplateHolder<IdT>::kVal>;
      DescriptorRef<SubProtoT>::GetAndMaybeLoadDescriptor();
      field_proto->set_type_name(
          "." + DescriptorRef<SubProtoT>::EffectiveMessageName());
    }
  }

  template <std::size_t... Is>
  static void PopulateAllFields(google::protobuf::DescriptorProto& msg_proto,
                                std::index_sequence<Is...>) {
    (PopulateFieldProto<Is>(msg_proto), ...);
  }

  static google::protobuf::FileDescriptorProto BuildFileDescriptorProto(
      const std::string& full_message_name) {
    google::protobuf::FileDescriptorProto file_proto;
    file_proto.set_syntax("proto2");
    file_proto.set_name(
        absl::StrCat("proto_bind_dynamic_", full_message_name, ".proto"));

    std::string package_name;
    std::string short_message_name = full_message_name;
    const std::size_t last_dot = full_message_name.rfind('.');
    if (last_dot != std::string::npos) {
      package_name = full_message_name.substr(0, last_dot);
      short_message_name = full_message_name.substr(last_dot + 1);
      file_proto.set_package(package_name);
    }

    google::protobuf::DescriptorProto* msg_proto =
        file_proto.add_message_type();
    msg_proto->set_name(short_message_name);

    PopulateAllFields(*msg_proto,
                      std::make_index_sequence<ProtoT_::kNumFields>{});
    return file_proto;
  }
};

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_DESCRIPTOR_REF_H_
