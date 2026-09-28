<!-- disableFinding(LINE_OVER_80) -->
<!-- disableFinding(WHITESPACE_TRAILING) -->
# Elephant-Goldfish Model (EGM) Writeup: Proto Bind (`proto::`)

## Executive Summary (The "Goldfish" Context)

If you are an agent or engineer entering `//third_party/jni_wrapper/` with no
prior session memory, here is everything you need to know in 60 seconds:

1.  **What Proto Bind Is**: A C++20 compile-time declarative binding layer for
    Protocol Buffers (`proto::Template`, `proto::Message`, `proto::Optional`,
    `proto::Required`, `proto::Repeated`, `proto::SubMessage`) built atop the
    shared, language-agnostic `bind_lang/` core (metaprogramming, handle and
    lifecycle vocabulary, proxy machinery) that also underlies JNI Bind. See
    [`bind_lang/README.md`](bind_lang/README.md).
2.  **Entry Points**:
    *   Public header: `third_party/jni_wrapper/proto_bind.h` (`//third_party/jni_wrapper:proto_bind`)
    *   Unit test fixture: `third_party/jni_wrapper/proto_test.h` (`//third_party/jni_wrapper:proto_test`)
    *   Mock environment: `third_party/jni_wrapper/mock_proto_env.h` (`//third_party/jni_wrapper:mock_proto_env`)
    *   Porcelain layer: `third_party/jni_wrapper/implementation/proto_helper/`
    *   Unit tests: `//third_party/jni_wrapper/implementation:message_test`, `proto_proxy_test`, `//third_party/jni_wrapper/implementation/proto_helper:proto_helper_test`
    *   Linux E2E test: `//third_party/jni_wrapper/implementation:proto_e2e_test`
3.  **Critical Repo Invariants (Do Not Break!)**:
    *   `build_jni_bind_release.sh` strips each header's license and include
        guard (it is include-guard aware; there is no fixed line count). Every
        `bind_lang` subpackage that `jni_bind` depends on must have its
        `headers_for_export` listed in both release genrules in the root
        `BUILD`. Keep `proto_bind` as a separate target from `jni_bind`.
    *   **Nothing in `bind_lang/` may depend on `implementation/`.** Check with
        `blaze query 'deps(//third_party/jni_wrapper/bind_lang/...) intersect //third_party/jni_wrapper/implementation/...'`
        (must be empty).
    *   Specializations of `bind_lang` customization points (e.g.
        `bind_lang::LifecycleHelper<google::protobuf::Message*, ...>`) must be
        declared in `namespace bind_lang`; a using-declared name cannot be
        specialized from another namespace.
    *   `README.md` is generated from `README_inc.md` and checked by
        `//third_party/jni_wrapper:readme_up_to_date_test`. Never edit `README.md`
        directly.

---

## Deep Architectural Context (The "Elephant" Memory)

### 1. Structural Isomorphism Between JNI Bind and Proto Bind

JNI Bind and Proto Bind solve the exact same fundamental problem in two
different reflection domains: bridging a string/id-keyed runtime reflection API
(`JNIEnv` vs `google::protobuf::Reflection`) to a zero-boilerplate, compile-time
type-checked C++20 interface.

| Architectural Layer | JNI Bind (`jni::`) | Proto Bind (`proto::`) | Shared / Reused Infrastructure |
| :--- | :--- | :--- | :--- |
| **Schema Definition** | `jni::Class` (`class.h`) | `proto::Template` (`proto_template.h`) | C++20 structural NTTPs, CTAD |
| **Field Specification** | `jni::Field` (`field.h`) | `Optional`, `Required`, `Repeated` (`proto_field.h`) | `bind_lang::metaprogramming::StringLiteral` |
| **Compile-Time Type Bundle** | `jni::JniT` (`jni_type.h`) | `proto::ProtoT` (`proto_type.h`) | Tuple introspection |
| **Compile-Time Selector** | `jni::Id` (`id.h`) | `proto::ProtoId` (`proto_id.h`) | `bind_lang::IdType`, `bind_lang::kNoIdx` |
| **Proxy Type Corpus** | `jni::Proxy` (`proxy_definitions.h`) | `proto::ProtoProxy` (`proxy_definitions_proto.h`) | `GenericProxyHelper`, `ProxyBase`, `ConvertibleIntegralProxyBase`, `ObjectProxyBase`, `Corpus_t` |
| **Schema Handle Cache** | `jni::ClassRef` (`class_ref.h`) | `proto::DescriptorRef` (`descriptor_ref.h`) | `bind_lang::metaprogramming::DoubleLockedValue` |
| **Field Accessor Proxy** | `jni::FieldRef` (`field_ref.h`) | `proto::ProtoFieldRef` (`proto_field_ref.h`) | `DoubleLockedValue`, `ForwardWithProxyTemporaryStrip` |
| **Object/Message CRTP Base** | `jni::ObjectRef` (`object_ref.h`) | `proto::MessageRef` (`message_ref.h`) | `QueryableMap_t`, `QueryableMap20`, `RefBase` |
| **Scoped Runtime Handle** | `jni::LocalObject` (`local_object.h`) | `proto::Message` (`message.h`) | `LifecycleHelper`, `AdoptLocal`, `NewRef` |
| **Porcelain Shim** | `implementation/jni_helper/` | `implementation/proto_helper/` | `Fake<T>()`, `Trace` |
| **Hermetic Unit Test Fixture** | `JniTest` + `MockJniEnv` | `ProtoTest` + `MockProtoEnv` | GoogleTest / gMock |

---

### 2. How `proxy.h` and `proxy_definitions.h` Were Abstracted for Reuse

Before Proto Bind, `jni::ProxyHelper<TUndecayed>` in `implementation/proxy.h`
hardcoded `jni::Proxy` and `jni::AllKeys` (`Corpus_t<UserDefinedCorpusTag, void, jboolean, jbyte, ...>`).

The proxy resolution engine now lives in `bind_lang/proxy/` (`namespace
bind_lang`) and is consumed by both languages:

1.  **`GenericProxyHelper<ProxyTemplate, AllKeysT, TUndecayed>` (`bind_lang/proxy/generic_proxy_helper.h`)**:
    Parameterized the corpus search (`TypeToTypeMapFromKeyValues_t` over `AsDecl`
    and `Index_t` over `AsArg`/`AsDecl`) on any `ProxyTemplate` and `AllKeysT`
    corpus.
    *   `jni::ProxyHelper<TUndecayed>` is now a 1-line struct inheriting from
        `GenericProxyHelper<jni::Proxy, jni::AllKeys, TUndecayed>`.
    *   `proto::ProtoProxyHelper<TUndecayed>` inherits from
        `GenericProxyHelper<ProtoProxy, AllProtoKeys, TUndecayed>`.
2.  **`ConvertibleIntegralProxyBase<CanonicalT, ViableTs...>` (`bind_lang/proxy/proxy_base.h`)**:
    Factored out cross-platform 64-bit integer conversion (`int64_t` vs `long` vs
    `long long` across LP64 Linux and LLP64 Windows) and narrow integral types so
    both `jni::Proxy<jlong>` and `proto::ProtoProxy<int64_t>` / `uint64_t`
    inherit from the same base. Non-canonical spellings are lowered with
    `static_cast` (brace-init would be narrowing for e.g. `char` -> `jchar`).
3.  **`ObjectProxyBase<RawHandleT, DeclBaseT, ExtraAsArgTs...>` (`bind_lang/proxy/proxy_base.h`)**:
    Factored out the object/message handle proxy logic (matching any compile-time
    schema type derived from `DeclBaseT`—`jni::Object` for JNI,
    `proto::TemplateBase` for Proto—and converting any `RefBase<RawHandleT>`
    handle via `ProxyAsArg`).
4.  **`IsConvertibleKey` (`bind_lang/proxy/is_convertible_key.h`)** and the
    pass-through **`ProxyBase`** moved alongside them.

`bind_lang/proxy:proxy_test` exercises all of these with a toy, JNI-free
language, which is the best template for adding a third binding language.

---

### 2b. The bind_lang Restructuring (Status and Remaining Gaps)

The top-level project is becoming **bind-lang**: shared code in `bind_lang/`,
JNI Bind eventually in `jni/`, Proto Bind eventually in `proto/`. The stack
beneath this change performed the first steps (each is a separate commit, with
file moves recorded via `jj piper rename` / `JJCOPY:`):

1.  Include-guard-aware `build_jni_bind_release.sh`.
2.  Visibility constrained so external users depend only on `:jni_bind`.
3.  `metaprogramming/` -> `bind_lang/metaprogramming/`; namespace
    `jni::metaprogramming` -> `bind_lang::metaprogramming`.
4.  `no_idx.h`, `id_type.h` -> `bind_lang/`.
5.  `ref_base.h` -> `bind_lang/`, with the `LifecycleHelper` primary template in
    `bind_lang/lifecycle.h` (JNI's `LifecycleLocalBase`/`LifecycleGlobalBase`
    stay in `jni_helper/lifecycle.h`).
6.  Proxy machinery -> `bind_lang/proxy/`.
7.  `bind_lang/README.md`.

JNI Bind keeps its public spelling (`jni::IdType`, `jni::RefBase`, ...) via
using-declarations in `implementation/bind_lang_imports.h` and friends. Proto
Bind refers to these names as `bind_lang::` directly.

**Proto Bind now uses `bind_lang` directly for** metaprogramming, `IdType`,
`kNoIdx`, `RefBase`, `LifecycleHelper`/`LifecycleType`, and all
proxy machinery (`proxy_definitions_proto` no longer depends on JNI's `proxy`
targets).

**Remaining Proto -> JNI dependencies** (candidates for the next extractions):

*   `jni::Trace` / `ArgString` (`jni_helper/trace.h`, `arg_string.h`): already
    JNI-free; can move wholesale to `bind_lang/`.
*   `jni::Fake` / `FakeImpl` (`jni_helper/fake_test_constants.h`): the primary
    template and pointer specialization are generic; the JNI constants are
    not. Split, with specializations declared in `namespace bind_lang`.
*   `proxy_temporary.h`:
    reused by `proto::ProtoFieldRef`; can be moved to `bind_lang/proxy/`.
*   `proto_proxy_test.cc` includes `jni_bind.h` on purpose, to assert that the
    shared bases are the same types in both languages.
*   JNI's generic `LifecycleHelper<Span, LOCAL>` partial specialization (in
    `jni_helper/lifecycle.h`) is greedy; it should eventually be constrained to
    JNI handle types so it cannot collide with other languages' handles.

---

### 3. Key Engineering Discoveries & Traps Solved

#### Discovery 1: C++20 Structural NTTP Rules vs `std::string` Field Specs

*   **Problem**: In `proto::Required<1, std::string>{"some_string_field"}`, the
    template is instantiated as a `static constexpr proto::Template kTemplate`
    and passed as a non-type template parameter (`proto::Message<kTemplate>`).
    C++20 requires all non-type template parameters to be *structural types*
    (all base classes and non-static data members must be public, non-mutable,
    and structural/literal). `std::string` is not a literal type in C++20
    `static constexpr` NTTPs!
*   **Solution (`implementation/proto_field.h`)**: `RawStorageHelper<Raw_>` maps
    non-trivially-destructible declaration types (such as `std::string`) to a
    literal `const char*` placeholder for the `raw_` member while preserving
    `using Raw = Raw_;` (`std::string`) for `ProtoId` and `ProtoProxy` lookup.

#### Discovery 2: Nested `proto::Template` Values in Type Template Parameters

*   **Problem**: `proto::Optional<kTag, Raw>` takes a *type* as its second
    template argument (`typename Raw_`), whereas a nested schema
    `static constexpr proto::Template kSub{...}` is a *value*.
*   **Solution (`implementation/proto_template.h`)**: Introduced
    `proto::SubMessage<kSubTemplate>`, a structural type adapter inheriting from
    `TemplateBase` that carries `kSubTemplate` as a `static constexpr` member.
    `ObjectProxyBase<google::protobuf::Message*, TemplateBase>` recognizes
    `SubMessage<kSubTemplate>` as a message declaration type, and `ProtoId::Val()`
    unwraps `SubMessage<kSubTemplate>::kTemplate` so `.Get()` returns
    `proto::Message<kSubTemplate>`.

#### Discovery 3: Zero-Based Tags in C++ vs One-Based Wire Tags in Protobuf

*   **Problem**: C++ users naturally write 0-based field tags
    (`proto::Optional<0, int>`, `proto::Required<1, std::string>`), whereas
    Protocol Buffers wire format and `DescriptorPool::BuildFile` require field
    numbers $\ge 1$.
*   **Solution (`implementation/descriptor_ref.h` & `proto_env.h`)**:
    *   In `ProtoFieldRef`, `ProtoHelper::GetFieldDescriptor` is called with the
        exact `IdT::kTag` written in C++, so unit tests with `MockProtoEnv` can
        assert on the exact tag numbers (`0`, `1`, ...).
    *   In `DescriptorRef::BuildFileDescriptorProto` (used when dynamically
        synthesizing a descriptor at runtime), if any field in `ProtoT` has
        `kTag == 0`, wire field numbers are shifted by $+1$.
    *   In `RealProtoEnv::GetFieldDescriptor`, fields are resolved by
        `FindFieldByName` first, then synthesized `"field_" + std::to_string(tag)`
        (preventing unnamed or sparse 0-based fields from colliding on $+1$
        shifted wire numbers), then `FindFieldByNumber`.

#### Discovery 4: Google3 `proto2` Namespace & `go/required-field-restrictions`

*   **Problem**: In Google3 (`#ifndef PROTO2_OPENSOURCE`), protobuf headers
    declare symbols in `namespace proto2` rather than `namespace google::protobuf`,
    and `descriptor.pb.h` requires the `//third_party/protobuf:descriptor_pb`
    target. Additionally, Google3's `protoc_minimal` rejects the `required`
    keyword in newly added `.proto` files (`go/required-field-restrictions`).
*   **Solution (`proto_dep.h` & `descriptor_ref.h`)**:
    *   Created `third_party/jni_wrapper/proto_dep.h` (`//third_party/jni_wrapper:proto_dep`,
        mirroring `jni_dep.h`), which includes the protobuf headers, depends on
        `//third_party/protobuf:descriptor_pb`, and aliases
        `namespace google::protobuf { using namespace ::proto2; }` when
        `!defined(PROTO2_OPENSOURCE)`.
    *   `proto::Required<kTag, T>` enforces requiredness at **C++ compile time**
        (rejecting `std::nullopt` in `ProtoFieldSelection`), while dynamic and
        `.proto` descriptors use `LABEL_OPTIONAL` at the protobuf descriptor
        layer.

#### Discovery 5: Tag-Dispatched Constructors vs Variadic Field Constructor

*   **Problem**: For a 2-field `proto::Template`, calling
    `proto::Message<kTemplate>{proto::NewRef{}, raw_msg_ptr}` with a non-const
    `google::protobuf::Message*` preferred the unconstrained 2-argument variadic
    field constructor `Message(Args&&...)` over `Message(proto::NewRef, const google::protobuf::Message*)`.
*   **Solution (`implementation/message.h`)**: Constrained the variadic
    field-initializing constructor via SFINAE so it is disabled whenever the
    first argument is `Message`, `proto::AdoptLocal`, `proto::NewRef`, or
    `proto::Unowned`.

#### Discovery 6: Rvalue `SubMessage` Lifetime & Dynamic `DescriptorPool` Underlay

*   **Problem**:
    1.  `bind_lang::ObjectProxyBase::ProxyAsArg(RefBase<RawHandleT>&&)` calls
        `.Release()` on rvalue handles because JNI C calls consume local
        references. However, protobuf's `SetMessageField` and `AddMessageField`
        deep-copy the source message (`CopyFrom`) rather than taking ownership
        of the pointer; calling `.Release()` on an rvalue `proto::Message`
        temporary leaked the heap-allocated `google::protobuf::Message*`.
    2.  When a dynamically synthesized parent `proto::Template` embeds a
        `SubMessage`, `DescriptorRef::PopulateFieldProto` must not dereference
        `sub_desc` (which is a `Fake<const Descriptor*>()` under `MockProtoEnv`),
        and `RealProtoEnv::dynamic_pool_` must resolve both dynamically
        synthesized and compiled `SubMessage` file dependencies without
        duplicate `file_proto.dependency()` entries.
*   **Solution (`proxy_definitions_proto.h`, `descriptor_ref.h`, `proto_env.h`)**:
    *   `ProtoProxy<google::protobuf::Message*>::ProxyAsArg` accepts
        `const bind_lang::RefBase<google::protobuf::Message*>&` without calling
        `.Release()`, so rvalue `proto::Message` temporaries are properly
        deleted at the end of the full-expression, and preserves the exact
        `SubMessage::kTemplate` reference in `AsReturn` so `.Get()` / `.Add()`
        share the same `DescriptorRef` cache as `proto::Message<kSubTemplate>`.
    *   `DescriptorRef::PopulateFieldProto` sets `field_proto->set_type_name`
        compile-time/statically via `DescriptorRef<SubProtoT>::EffectiveMessageName()`,
        while `RealProtoEnv` initializes `dynamic_pool_` with
        `DescriptorPool::generated_pool()` as its underlay, enables
        `dynamic_factory_.SetDelegateToGeneratedFactory(true)`, and
        automatically resolves and deduplicates `TYPE_MESSAGE` file dependencies
        in `RegisterDynamicDescriptor`.
