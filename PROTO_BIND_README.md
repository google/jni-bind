<!-- disableFinding(LINE_OVER_80) -->
<!-- disableFinding(WHITESPACE_TRAILING) -->
# Proto Bind (`proto::`)

**Proto Bind** is a header-only C++20 metaprogramming library inside
`//third_party/jni_wrapper/` that brings [JNI Bind](https://github.com/google/jni-bind)'s
declarative compile-time schema definitions, `QueryableMap` string-indexed
accessors, proxy type translation, and porcelain abstraction layer to
**Protocol Buffers**.

For the **Elephant-Goldfish Model (EGM)** architectural writeup and engineering
discoveries, see [`EGM_PROTO_BIND.md`](EGM_PROTO_BIND.md).

Proto Bind and JNI Bind share a language-agnostic core in `bind_lang/`
(metaprogramming, `RefBase`, `LifecycleHelper`, proxy machinery). See
[`bind_lang/README.md`](bind_lang/README.md).

---

## Quick Start

### 1. Include & Build Dependency

Depend on `//third_party/jni_wrapper:proto_bind` in your `BUILD` file and
include:

```cpp
#include "third_party/jni_wrapper/proto_bind.h"
```

### 2. Declare a Compile-Time Schema (`proto::Template`)

Proto Bind supports three declaration styles:

#### Style A: CTAD with Field Names in Field Constructors (Recommended)
```cpp
static constexpr proto::Template kTemplate{
    proto::Optional<0, int>{"some_optional_int_field"},
    proto::Required<1, std::string>{"some_string_field"},
};
```

#### Style B: Explicit Template Arguments with Field Names in `Template` Constructor
```cpp
static constexpr proto::Template<
    proto::Optional<0, int>,
    proto::Required<1, std::string>
> kTemplate{"some_optional_int_field", "some_string_field"};
```

#### Style C: C++20 `StringLiteral` NTTP Field Names
```cpp
static constexpr proto::Template<
    proto::Optional<0, int, "some_optional_int_field">,
    proto::Required<1, std::string, "some_string_field">
> kTemplate{};
```

#### Binding to a Compiled `.proto` Message
Pass the full protobuf message name (`"package.MessageName"`) as the first
constructor argument to bind directly to a descriptor in
`google::protobuf::DescriptorPool::generated_pool()`:

```cpp
static constexpr proto::Template kSampleTemplate{
    "proto_bind.test.SampleMessage",
    proto::Optional<1, int>{"some_optional_int_field"},
    proto::Required<2, std::string>{"some_string_field"},
};
```

When no message name is specified (or the message name is not present in
`generated_pool()`), `DescriptorRef` automatically synthesizes a
`google::protobuf::FileDescriptorProto` from the compile-time `proto::Template`
and registers it in a runtime `DynamicMessageFactory`.

---

## 3. Constructing & Mutating Runtime Messages (`proto::Message`)

```cpp
// Construct and populate all fields in declaration order:
proto::Message<kTemplate> runtime_msg{123, "string_val"};

// Or pass std::nullopt for Optional fields (Required fields reject std::nullopt at compile time!):
proto::Message<kTemplate> partial_msg{std::nullopt, "only_required"};

// Read and write fields by name using operator[] (Clang QueryableMap):
int32_t v = runtime_msg["some_optional_int_field"].Get();
runtime_msg["some_optional_int_field"].Set(456);

// Read and write fields by C++20 StringLiteral NTTP (QueryableMap20):
std::string s = runtime_msg.Access<"some_string_field">().Get();
runtime_msg.Access<"some_string_field">().Set("updated");

// Read and write fields by tag number:
runtime_msg.AccessByTag<0>().Set(789);

// Presence and std::optional access on Optional fields:
bool has_int = runtime_msg["some_optional_int_field"].Has();
std::optional<int32_t> opt_int = runtime_msg["some_optional_int_field"].GetOptional();
runtime_msg["some_optional_int_field"].Set(std::nullopt);  // Clears field
```

---

## 4. Nested Messages & Repeated Fields

Use `proto::SubMessage<kSubTemplate>` for nested message fields and
`proto::Repeated<kTag, T>` for repeated fields:

```cpp
static constexpr proto::Template kNestedPayload{
    "proto_bind.test.NestedPayload",
    proto::Optional<1, int>{"code"},
    proto::Optional<2, std::string>{"label"},
};

static constexpr proto::Template kParent{
    "proto_bind.test.SampleMessage",
    proto::Optional<6, proto::SubMessage<kNestedPayload>>{"payload"},
    proto::Repeated<7, std::string>{"tags"},
    proto::Repeated<9, proto::SubMessage<kNestedPayload>>{"items"},
};

proto::Message<kParent> msg{};
msg["payload"].Get()["code"].Set(200);
msg["payload"].Get()["label"].Set("OK");
// Or assign a SubMessage directly from an rvalue/lvalue proto::Message or native Message:
msg["payload"].Set(proto::Message<kNestedPayload>{201, "Created"});

msg["tags"].Add("alpha");
msg["tags"].Add("beta");
msg["tags"].Set(1, "beta_updated");
int n = msg["tags"].Size();          // 2
std::string t1 = msg["tags"][1];     // "beta_updated" (or .At(1) / .Get(1))

// Repeated SubMessage elements can be added via zero-arg .Add() or .Add(sub_msg):
auto item0 = msg["items"].Add();
item0["code"].Set(100);
msg["items"].Add(proto::Message<kNestedPayload>{404, "NotFound"});
int code1 = msg["items"][1]["code"].Get();  // 404
```

---

## 5. Ownership & Interop with Generated C++ Protos

`proto::Message<kTemplate>` manages `google::protobuf::Message*` lifetime via
`bind_lang::LifecycleHelper<google::protobuf::Message*, bind_lang::LifecycleType::LOCAL>`:

*   `proto::Message<kTemplate> msg{...}`: Allocates and owns a new message; deletes on scope exit.
*   `proto::Message<kTemplate> msg{proto::AdoptLocal{}, raw_ptr}`: Adopts ownership of `raw_ptr`.
*   `proto::Message<kTemplate> msg{proto::NewRef{}, raw_ptr}`: Deep-clones `raw_ptr` (`New()` + `CopyFrom()`) and owns the clone.
*   `proto::Message<kTemplate> msg{proto::Unowned{}, raw_ptr}`: Wraps `raw_ptr` without taking ownership (ideal for mutating an existing stack-allocated generated proto in place).

---

## 6. Testing: Unit Tests (`ProtoTest`) vs E2E Tests

Just as JNI Bind separates fast, hermetic unit tests (`JniTest` + `MockJniEnv`)
from JVM E2E tests, Proto Bind provides:

*   **Unit Testing (`//third_party/jni_wrapper:proto_test`)**: Inherit from
    `proto::test::ProtoTest` to install a `MockProtoEnv` (`gMock` over
    `ProtoEnvInterface`). All descriptor lookups, message allocations, and
    reflection calls are intercepted with fake pointers (`Fake<const Descriptor*>()`,
    `Fake<const FieldDescriptor*>()`, `Fake<Message*>()`) without requiring any
    `.proto` compilation.
*   **E2E Testing (`//third_party/jni_wrapper/implementation:proto_e2e_test`)**:
    When no `MockProtoEnv` is installed, `ProtoEnv::GetEnv()` defaults to
    `RealProtoEnv::Instance()`, which dispatches to real `DescriptorPool`,
    `MessageFactory`, `DynamicMessageFactory`, and `Reflection`.
