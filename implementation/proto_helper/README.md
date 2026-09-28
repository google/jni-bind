# Proto Helper Porcelain Layer (`implementation/proto_helper/`)

This package is the Protocol Buffers counterpart to
`third_party/jni_wrapper/implementation/jni_helper/`. It isolates all direct
interactions with `google::protobuf::DescriptorPool`, `MessageFactory`,
`DynamicMessageFactory`, and `Reflection` behind a mockable interface
(`ProtoEnvInterface`).

## Files

*   `proto_env.h`: Defines `ProtoEnvInterface`, `RealProtoEnv` (production/E2E
    implementation backed by `DescriptorPool::generated_pool()`,
    `DynamicMessageFactory`, and `Reflection`), and `ProtoEnv::GetEnv()` /
    `ProtoEnv::SetEnv()`.
*   `proto_helper.h`: Defines `ProtoHelper::FindDescriptor`,
    `ProtoHelper::RegisterDynamicDescriptor`, and
    `ProtoHelper::GetFieldDescriptor` with `jni::Trace` instrumentation and
    `DRY_RUN` support.
*   `field_value.h`: Defines `ProtoFieldHelper<Raw, kRank>` specializations for
    scalar types (`bool`, `int32_t`, `uint32_t`, `int64_t`, `uint64_t`, `float`,
    `double`), `std::string`, nested `google::protobuf::Message*`, and repeated
    fields (`kRank == 1`).
*   `lifecycle_proto.h`: Specializes `bind_lang::LifecycleHelper` for
    `google::protobuf::Message*` and `bind_lang::LifecycleType::LOCAL`
    (`Construct`, `NewReference`, `Delete`).
*   `fake_proto_test_constants.h`: Specializes `jni::FakeImpl` for
    `const google::protobuf::Descriptor*`,
    `const google::protobuf::FieldDescriptor*`, `google::protobuf::Message*`,
    and `proto::ProtoEnvInterface*`.
