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

#ifndef JNI_BIND_IMPLEMENTATION_PROTO_HELPER_PROTO_ENV_H_
#define JNI_BIND_IMPLEMENTATION_PROTO_HELPER_PROTO_ENV_H_

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <mutex>
#include <string>
#include <string_view>
#include <utility>

#include "third_party/absl/strings/str_cat.h"
#include "proto_dep.h"

namespace proto {

// Abstract porcelain interface for Protocol Buffer reflection and lifecycle
// operations. Analogous to `JNIEnv` in JNI Bind: in production and E2E tests,
// `RealProtoEnv` dispatches to `google::protobuf::Reflection`,
// `DescriptorPool`, and `MessageFactory`; in unit tests, `MockProtoEnv`
// intercepts calls without requiring compiled `.proto` schemas.
class ProtoEnvInterface {
 public:
  virtual ~ProtoEnvInterface() = default;

  // Descriptor and FieldDescriptor lookup (analogous to FindClass /
  // GetFieldID).
  virtual const google::protobuf::Descriptor* FindMessageTypeByName(
      const char* name) = 0;

  virtual const google::protobuf::Descriptor* RegisterDynamicDescriptor(
      const google::protobuf::FileDescriptorProto& file_proto,
      const char* message_name) = 0;

  virtual const google::protobuf::FieldDescriptor* GetFieldDescriptor(
      const google::protobuf::Descriptor* descriptor, int field_number,
      const char* field_name) = 0;

  // Message lifecycle (analogous to NewObject / NewLocalRef / DeleteLocalRef).
  virtual google::protobuf::Message* NewMessage(
      const google::protobuf::Descriptor* descriptor) = 0;

  virtual google::protobuf::Message* CloneMessage(
      const google::protobuf::Message* message) = 0;

  virtual void DeleteMessage(google::protobuf::Message* message) = 0;

  virtual std::string SerializeAsString(
      const google::protobuf::Message* message) = 0;

  virtual bool ParseFromString(google::protobuf::Message* message,
                               std::string_view data) = 0;

  // Field presence operations.
  virtual bool HasField(const google::protobuf::Message* message,
                        const google::protobuf::FieldDescriptor* field) = 0;

  virtual void ClearField(google::protobuf::Message* message,
                          const google::protobuf::FieldDescriptor* field) = 0;

  // Scalar field accessors.
  virtual int32_t GetInt32Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) = 0;
  virtual void SetInt32Field(google::protobuf::Message* message,
                             const google::protobuf::FieldDescriptor* field,
                             int32_t value) = 0;

  virtual int64_t GetInt64Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) = 0;
  virtual void SetInt64Field(google::protobuf::Message* message,
                             const google::protobuf::FieldDescriptor* field,
                             int64_t value) = 0;

  virtual uint32_t GetUInt32Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) = 0;
  virtual void SetUInt32Field(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              uint32_t value) = 0;

  virtual uint64_t GetUInt64Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) = 0;
  virtual void SetUInt64Field(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              uint64_t value) = 0;

  virtual float GetFloatField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) = 0;
  virtual void SetFloatField(google::protobuf::Message* message,
                             const google::protobuf::FieldDescriptor* field,
                             float value) = 0;

  virtual double GetDoubleField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) = 0;
  virtual void SetDoubleField(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              double value) = 0;

  virtual bool GetBoolField(const google::protobuf::Message* message,
                            const google::protobuf::FieldDescriptor* field) = 0;
  virtual void SetBoolField(google::protobuf::Message* message,
                            const google::protobuf::FieldDescriptor* field,
                            bool value) = 0;

  virtual std::string GetStringField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) = 0;
  virtual void SetStringField(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              std::string value) = 0;

  // Nested message accessors.
  virtual const google::protobuf::Message* GetMessageField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) = 0;
  virtual google::protobuf::Message* MutableMessageField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) = 0;
  virtual void SetMessageField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field,
      const google::protobuf::Message* sub_message) = 0;

  // Repeated field accessors.
  virtual int FieldSize(const google::protobuf::Message* message,
                        const google::protobuf::FieldDescriptor* field) = 0;

  virtual bool GetRepeatedBoolField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) = 0;
  virtual void SetRepeatedBoolField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index,
      bool value) = 0;
  virtual void AddBoolField(google::protobuf::Message* message,
                            const google::protobuf::FieldDescriptor* field,
                            bool value) = 0;

  virtual int32_t GetRepeatedInt32Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) = 0;
  virtual void SetRepeatedInt32Field(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index,
      int32_t value) = 0;
  virtual void AddInt32Field(google::protobuf::Message* message,
                             const google::protobuf::FieldDescriptor* field,
                             int32_t value) = 0;

  virtual uint32_t GetRepeatedUInt32Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) = 0;
  virtual void SetRepeatedUInt32Field(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index,
      uint32_t value) = 0;
  virtual void AddUInt32Field(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              uint32_t value) = 0;

  virtual int64_t GetRepeatedInt64Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) = 0;
  virtual void SetRepeatedInt64Field(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index,
      int64_t value) = 0;
  virtual void AddInt64Field(google::protobuf::Message* message,
                             const google::protobuf::FieldDescriptor* field,
                             int64_t value) = 0;

  virtual uint64_t GetRepeatedUInt64Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) = 0;
  virtual void SetRepeatedUInt64Field(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index,
      uint64_t value) = 0;
  virtual void AddUInt64Field(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              uint64_t value) = 0;

  virtual float GetRepeatedFloatField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) = 0;
  virtual void SetRepeatedFloatField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index,
      float value) = 0;
  virtual void AddFloatField(google::protobuf::Message* message,
                             const google::protobuf::FieldDescriptor* field,
                             float value) = 0;

  virtual double GetRepeatedDoubleField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) = 0;
  virtual void SetRepeatedDoubleField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index,
      double value) = 0;
  virtual void AddDoubleField(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              double value) = 0;

  virtual std::string GetRepeatedStringField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) = 0;
  virtual void SetRepeatedStringField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index,
      std::string value) = 0;
  virtual void AddStringField(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              std::string value) = 0;

  virtual const google::protobuf::Message* GetRepeatedMessageField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) = 0;
  virtual google::protobuf::Message* MutableRepeatedMessageField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) = 0;
  virtual void SetRepeatedMessageField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index,
      const google::protobuf::Message* sub_message) = 0;
  virtual google::protobuf::Message* AddMutableMessageField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) = 0;
  virtual void AddMessageField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field,
      const google::protobuf::Message* sub_message) = 0;
};

// Concrete implementation backed by `google::protobuf::DescriptorPool`,
// `MessageFactory`, `DynamicMessageFactory`, and `Reflection`.
class RealProtoEnv : public ProtoEnvInterface {
 public:
  RealProtoEnv()
      : dynamic_pool_(google::protobuf::DescriptorPool::generated_pool()),
        dynamic_factory_(&dynamic_pool_) {
    dynamic_factory_.SetDelegateToGeneratedFactory(true);
  }

  static RealProtoEnv* Instance() {
    static auto* instance = new RealProtoEnv();
    return instance;
  }

  const google::protobuf::Descriptor* FindMessageTypeByName(
      const char* name) override {
    if (name == nullptr || name[0] == '\0') {
      return nullptr;
    }
    const google::protobuf::Descriptor* generated =
        google::protobuf::DescriptorPool::generated_pool()
            ->FindMessageTypeByName(name);
    if (generated != nullptr) {
      return generated;
    }
    std::lock_guard<std::mutex> lock(mutex_);  // NOLINT(build/c++11)
    return dynamic_pool_.FindMessageTypeByName(name);
  }

  const google::protobuf::Descriptor* RegisterDynamicDescriptor(
      const google::protobuf::FileDescriptorProto& file_proto,
      const char* message_name) override {
    std::lock_guard<std::mutex> lock(mutex_);  // NOLINT(build/c++11)
    if (const google::protobuf::Descriptor* existing =
            dynamic_pool_.FindMessageTypeByName(message_name);
        existing != nullptr) {
      return existing;
    }
    return BuildDynamicDescriptor(file_proto, message_name);
  }

  const google::protobuf::FieldDescriptor* GetFieldDescriptor(
      const google::protobuf::Descriptor* descriptor, int field_number,
      const char* field_name) override {
    if (descriptor == nullptr) {
      return nullptr;
    }
    const std::string_view name_view = field_name != nullptr
                                           ? std::string_view{field_name}
                                           : std::string_view{};
    if (!name_view.empty()) {
      if (const auto* by_name = descriptor->FindFieldByName(name_view);
          by_name != nullptr) {
        return by_name;
      }
    }
    if (name_view.empty() && descriptor->file()->pool() == &dynamic_pool_) {
      if (const auto* by_synth_name = descriptor->FindFieldByName(
              absl::StrCat("field_", field_number));
          by_synth_name != nullptr) {
        return by_synth_name;
      }
    }
    if (const auto* by_number = descriptor->FindFieldByNumber(field_number);
        by_number != nullptr) {
      return by_number;
    }
    return nullptr;
  }

  google::protobuf::Message* NewMessage(
      const google::protobuf::Descriptor* descriptor) override {
    if (descriptor == nullptr) {
      return nullptr;
    }
    if (descriptor->file()->pool() ==
        google::protobuf::DescriptorPool::generated_pool()) {
      const google::protobuf::Message* prototype =
          google::protobuf::MessageFactory::generated_factory()->GetPrototype(
              descriptor);
      if (prototype != nullptr) {
        return prototype->New();
      }
    }
    std::lock_guard<std::mutex> lock(mutex_);  // NOLINT(build/c++11)
    const google::protobuf::Message* prototype =
        dynamic_factory_.GetPrototype(descriptor);
    return prototype != nullptr ? prototype->New() : nullptr;
  }

  google::protobuf::Message* CloneMessage(
      const google::protobuf::Message* message) override {
    if (message == nullptr) {
      return nullptr;
    }
    google::protobuf::Message* clone = message->New();
    clone->CopyFrom(*message);
    return clone;
  }

  void DeleteMessage(google::protobuf::Message* message) override {
    delete message;
  }

  std::string SerializeAsString(
      const google::protobuf::Message* message) override {
    return message != nullptr ? message->SerializeAsString() : std::string{};
  }

  bool ParseFromString(google::protobuf::Message* message,
                       std::string_view data) override {
    return message != nullptr && message->ParseFromString(data);
  }

  bool HasField(const google::protobuf::Message* message,
                const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->HasField(*message, field);
  }

  void ClearField(google::protobuf::Message* message,
                  const google::protobuf::FieldDescriptor* field) override {
    message->GetReflection()->ClearField(message, field);
  }

  int32_t GetInt32Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->GetInt32(*message, field);
  }

  void SetInt32Field(google::protobuf::Message* message,
                     const google::protobuf::FieldDescriptor* field,
                     int32_t value) override {
    message->GetReflection()->SetInt32(message, field, value);
  }

  int64_t GetInt64Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->GetInt64(*message, field);
  }

  void SetInt64Field(google::protobuf::Message* message,
                     const google::protobuf::FieldDescriptor* field,
                     int64_t value) override {
    message->GetReflection()->SetInt64(message, field, value);
  }

  uint32_t GetUInt32Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->GetUInt32(*message, field);
  }

  void SetUInt32Field(google::protobuf::Message* message,
                      const google::protobuf::FieldDescriptor* field,
                      uint32_t value) override {
    message->GetReflection()->SetUInt32(message, field, value);
  }

  uint64_t GetUInt64Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->GetUInt64(*message, field);
  }

  void SetUInt64Field(google::protobuf::Message* message,
                      const google::protobuf::FieldDescriptor* field,
                      uint64_t value) override {
    message->GetReflection()->SetUInt64(message, field, value);
  }

  float GetFloatField(const google::protobuf::Message* message,
                      const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->GetFloat(*message, field);
  }

  void SetFloatField(google::protobuf::Message* message,
                     const google::protobuf::FieldDescriptor* field,
                     float value) override {
    message->GetReflection()->SetFloat(message, field, value);
  }

  double GetDoubleField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->GetDouble(*message, field);
  }

  void SetDoubleField(google::protobuf::Message* message,
                      const google::protobuf::FieldDescriptor* field,
                      double value) override {
    message->GetReflection()->SetDouble(message, field, value);
  }

  bool GetBoolField(const google::protobuf::Message* message,
                    const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->GetBool(*message, field);
  }

  void SetBoolField(google::protobuf::Message* message,
                    const google::protobuf::FieldDescriptor* field,
                    bool value) override {
    message->GetReflection()->SetBool(message, field, value);
  }

  std::string GetStringField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->GetString(*message, field);
  }

  void SetStringField(google::protobuf::Message* message,
                      const google::protobuf::FieldDescriptor* field,
                      std::string value) override {
    message->GetReflection()->SetString(message, field, std::move(value));
  }

  const google::protobuf::Message* GetMessageField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) override {
    return &message->GetReflection()->GetMessage(*message, field);
  }

  google::protobuf::Message* MutableMessageField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->MutableMessage(message, field);
  }

  void SetMessageField(google::protobuf::Message* message,
                       const google::protobuf::FieldDescriptor* field,
                       const google::protobuf::Message* sub_message) override {
    if (sub_message == nullptr) {
      message->GetReflection()->ClearField(message, field);
      return;
    }
    CopyMessageContent(message->GetReflection()->MutableMessage(message, field),
                       sub_message);
  }

  int FieldSize(const google::protobuf::Message* message,
                const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->FieldSize(*message, field);
  }

  bool GetRepeatedBoolField(const google::protobuf::Message* message,
                            const google::protobuf::FieldDescriptor* field,
                            int index) override {
    return message->GetReflection()->GetRepeatedBool(*message, field, index);
  }

  void SetRepeatedBoolField(google::protobuf::Message* message,
                            const google::protobuf::FieldDescriptor* field,
                            int index, bool value) override {
    message->GetReflection()->SetRepeatedBool(message, field, index, value);
  }

  void AddBoolField(google::protobuf::Message* message,
                    const google::protobuf::FieldDescriptor* field,
                    bool value) override {
    message->GetReflection()->AddBool(message, field, value);
  }

  int32_t GetRepeatedInt32Field(const google::protobuf::Message* message,
                                const google::protobuf::FieldDescriptor* field,
                                int index) override {
    return message->GetReflection()->GetRepeatedInt32(*message, field, index);
  }

  void SetRepeatedInt32Field(google::protobuf::Message* message,
                             const google::protobuf::FieldDescriptor* field,
                             int index, int32_t value) override {
    message->GetReflection()->SetRepeatedInt32(message, field, index, value);
  }

  void AddInt32Field(google::protobuf::Message* message,
                     const google::protobuf::FieldDescriptor* field,
                     int32_t value) override {
    message->GetReflection()->AddInt32(message, field, value);
  }

  uint32_t GetRepeatedUInt32Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) override {
    return message->GetReflection()->GetRepeatedUInt32(*message, field, index);
  }

  void SetRepeatedUInt32Field(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              int index, uint32_t value) override {
    message->GetReflection()->SetRepeatedUInt32(message, field, index, value);
  }

  void AddUInt32Field(google::protobuf::Message* message,
                      const google::protobuf::FieldDescriptor* field,
                      uint32_t value) override {
    message->GetReflection()->AddUInt32(message, field, value);
  }

  int64_t GetRepeatedInt64Field(const google::protobuf::Message* message,
                                const google::protobuf::FieldDescriptor* field,
                                int index) override {
    return message->GetReflection()->GetRepeatedInt64(*message, field, index);
  }

  void SetRepeatedInt64Field(google::protobuf::Message* message,
                             const google::protobuf::FieldDescriptor* field,
                             int index, int64_t value) override {
    message->GetReflection()->SetRepeatedInt64(message, field, index, value);
  }

  void AddInt64Field(google::protobuf::Message* message,
                     const google::protobuf::FieldDescriptor* field,
                     int64_t value) override {
    message->GetReflection()->AddInt64(message, field, value);
  }

  uint64_t GetRepeatedUInt64Field(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) override {
    return message->GetReflection()->GetRepeatedUInt64(*message, field, index);
  }

  void SetRepeatedUInt64Field(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              int index, uint64_t value) override {
    message->GetReflection()->SetRepeatedUInt64(message, field, index, value);
  }

  void AddUInt64Field(google::protobuf::Message* message,
                      const google::protobuf::FieldDescriptor* field,
                      uint64_t value) override {
    message->GetReflection()->AddUInt64(message, field, value);
  }

  float GetRepeatedFloatField(const google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              int index) override {
    return message->GetReflection()->GetRepeatedFloat(*message, field, index);
  }

  void SetRepeatedFloatField(google::protobuf::Message* message,
                             const google::protobuf::FieldDescriptor* field,
                             int index, float value) override {
    message->GetReflection()->SetRepeatedFloat(message, field, index, value);
  }

  void AddFloatField(google::protobuf::Message* message,
                     const google::protobuf::FieldDescriptor* field,
                     float value) override {
    message->GetReflection()->AddFloat(message, field, value);
  }

  double GetRepeatedDoubleField(const google::protobuf::Message* message,
                                const google::protobuf::FieldDescriptor* field,
                                int index) override {
    return message->GetReflection()->GetRepeatedDouble(*message, field, index);
  }

  void SetRepeatedDoubleField(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              int index, double value) override {
    message->GetReflection()->SetRepeatedDouble(message, field, index, value);
  }

  void AddDoubleField(google::protobuf::Message* message,
                      const google::protobuf::FieldDescriptor* field,
                      double value) override {
    message->GetReflection()->AddDouble(message, field, value);
  }

  std::string GetRepeatedStringField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) override {
    return message->GetReflection()->GetRepeatedString(*message, field, index);
  }

  void SetRepeatedStringField(google::protobuf::Message* message,
                              const google::protobuf::FieldDescriptor* field,
                              int index, std::string value) override {
    message->GetReflection()->SetRepeatedString(message, field, index,
                                                std::move(value));
  }

  void AddStringField(google::protobuf::Message* message,
                      const google::protobuf::FieldDescriptor* field,
                      std::string value) override {
    message->GetReflection()->AddString(message, field, std::move(value));
  }

  const google::protobuf::Message* GetRepeatedMessageField(
      const google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) override {
    return &message->GetReflection()->GetRepeatedMessage(*message, field,
                                                         index);
  }

  google::protobuf::Message* MutableRepeatedMessageField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index) override {
    return message->GetReflection()->MutableRepeatedMessage(message, field,
                                                            index);
  }

  void SetRepeatedMessageField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field, int index,
      const google::protobuf::Message* sub_message) override {
    if (sub_message == nullptr) {
      return;
    }
    CopyMessageContent(
        message->GetReflection()->MutableRepeatedMessage(message, field, index),
        sub_message);
  }

  google::protobuf::Message* AddMutableMessageField(
      google::protobuf::Message* message,
      const google::protobuf::FieldDescriptor* field) override {
    return message->GetReflection()->AddMessage(message, field);
  }

  void AddMessageField(google::protobuf::Message* message,
                       const google::protobuf::FieldDescriptor* field,
                       const google::protobuf::Message* sub_message) override {
    if (sub_message == nullptr) {
      return;
    }
    CopyMessageContent(message->GetReflection()->AddMessage(message, field),
                       sub_message);
  }

 private:
  const google::protobuf::Descriptor* BuildDynamicDescriptor(
      google::protobuf::FileDescriptorProto file_proto,
      const char* message_name) {
    auto original_deps = std::move(*file_proto.mutable_dependency());
    file_proto.clear_dependency();
    auto add_dep_if_needed = [&](std::string_view dep_name) {
      if (dep_name.empty() || dep_name == file_proto.name()) {
        return;
      }
      const auto& deps = file_proto.dependency();
      if (std::ranges::find(deps, dep_name) ==  // NOLINT(build/c++20)
          deps.end()) {
        file_proto.add_dependency(std::string{dep_name});
      }
    };
    std::ranges::for_each(  // NOLINT(build/c++20)
        original_deps, add_dep_if_needed);
    std::ranges::for_each(  // NOLINT(build/c++20)
        file_proto.message_type(),
        [&](const google::protobuf::DescriptorProto& msg_proto) {
          std::ranges::for_each(  // NOLINT(build/c++20)
              msg_proto.field(),
              [&](const google::protobuf::FieldDescriptorProto& field_proto) {
                if (field_proto.type() ==
                        google::protobuf::FieldDescriptorProto::TYPE_MESSAGE &&
                    field_proto.has_type_name()) {
                  std::string_view type_name = field_proto.type_name();
                  if (!type_name.empty() && type_name.front() == '.') {
                    type_name.remove_prefix(1);
                  }
                  const google::protobuf::Descriptor* sub_desc =
                      dynamic_pool_.FindMessageTypeByName(type_name);
                  if (sub_desc != nullptr && sub_desc->file() != nullptr) {
                    add_dep_if_needed(sub_desc->file()->name());
                  }
                }
              });
        });

    const google::protobuf::FileDescriptor* file_desc =
        dynamic_pool_.BuildFile(file_proto);
    if (file_desc == nullptr) {
      return nullptr;
    }
    if (const google::protobuf::Descriptor* found =
            file_desc->FindMessageTypeByName(message_name);
        found != nullptr) {
      return found;
    }
    return dynamic_pool_.FindMessageTypeByName(message_name);
  }

  static void CopyMessageContent(google::protobuf::Message* target,
                                 const google::protobuf::Message* source) {
    if (target == nullptr || source == nullptr) {
      return;
    }
    if (target->GetDescriptor() == source->GetDescriptor()) {
      target->CopyFrom(*source);
      return;
    }
    std::string serialized;
    if (source->SerializeToString(&serialized)) {
      const bool parsed = target->ParseFromString(serialized);
      assert(parsed &&
             "Failed to copy message content via serialization due to "
             "incompatible wire format or parse failure.");
      (void)parsed;
      if (!parsed) {
        target->Clear();
      }
    } else {
      assert(false &&
             "Failed to serialize source message in CopyMessageContent.");
      target->Clear();
    }
  }

  std::mutex mutex_;  // NOLINT(build/c++11)
  google::protobuf::DescriptorPool dynamic_pool_;
  google::protobuf::DynamicMessageFactory dynamic_factory_;
};

// Static/thread-local accessor for the active `ProtoEnvInterface*`.
// Defaults to `RealProtoEnv::Instance()` when no override is installed, so E2E
// and production code require zero boilerplate setup.
class ProtoEnv {
 public:
  static ProtoEnvInterface* GetEnv() {
    return env_ != nullptr ? env_ : RealProtoEnv::Instance();
  }

  static void SetEnv(ProtoEnvInterface* env) { env_ = env; }

 private:
  static inline thread_local ProtoEnvInterface* env_ = nullptr;
};

}  // namespace proto

#endif  // JNI_BIND_IMPLEMENTATION_PROTO_HELPER_PROTO_ENV_H_
