# bind_lang

`bind_lang` ("a binding language for languages") is the language-agnostic core
shared by the binding libraries in `third_party/jni_wrapper/`:

*   **JNI Bind** (`implementation/`, `class_defs/`, `jni_bind.h`): C++ ⇄ Java.
*   **Proto Bind** (experimental): C++ ⇄ protobuf messages via
    `proto::Template` / `proto::Message`.

The long-term layout is:

```
third_party/jni_wrapper/
  bind_lang/   <- shared, language-agnostic core (this directory)
  jni/         <- JNI Bind (today: implementation/, class_defs/, ...)
  proto/       <- Proto Bind
```

The migration is incremental. `implementation/` has not moved to `jni/` yet.

## The dependency rule

> [!IMPORTANT]
> Nothing under `bind_lang/` may depend on `implementation/` (JNI Bind) or on
> any other language-specific package. Language packages depend on
> `bind_lang`, never the reverse.

Verify with:

```bash
blaze query 'deps(//third_party/jni_wrapper/bind_lang/...) intersect (//third_party/jni_wrapper/... - //third_party/jni_wrapper/bind_lang/... - //third_party/jni_wrapper:license)'
```

The result must be empty.

## Layout

Path                                 | Contents
------------------------------------ | ----------------------------------------
`metaprogramming/`                   | Type-level algorithms (`Corpus`, `TypeToTypeMap`, `Reduce`, `Invoke`, ...). `namespace bind_lang::metaprogramming`.
`id_type.h`                          | `IdType`, the kinds of entity an `Id` can address.
`no_idx.h`                           | `kNoIdx` sentinel.
`ref_base.h`                         | `RefBase<StorageType>`, the base of every RAII handle wrapper.
`lifecycle.h`                        | `LifecycleType` and the `LifecycleHelper<Span, LifecycleType>` customization point.
`proxy/is_convertible_key.h`         | `IsConvertibleKey`, the comparator used to find a proxy for a C++ type.
`proxy/proxy_base.h`                 | `ProxyBase`, `ConvertibleIntegralProxyBase`, `ObjectProxyBase`.
`proxy/generic_proxy_helper.h`       | `GenericProxyHelper<ProxyTemplate, AllKeys, T>`, which selects a language's `Proxy` specialization for `T`.

All of these are in `namespace bind_lang`.

## How a language package consumes bind_lang

### Re-exporting vocabulary

JNI Bind's public API refers to these names unqualified inside `namespace jni`
(e.g. `jni::IdType`, `jni::RefBase`). Instead of rewriting every use, they
are re-exported with using-declarations:

*   `implementation/bind_lang_imports.h`: `IdType`, `kNoIdx`, `RefBase`.
*   `implementation/jni_helper/lifecycle.h`: `LifecycleType`,
    `LifecycleHelper`.
*   `implementation/proxy.h`, `implementation/proxy_convenience_aliases.h`:
    `ProxyBase`, `ConvertibleIntegralProxyBase`, `ObjectProxyBase`,
    `IsConvertibleKey` / `IsConvertibleKey_v`.

New language packages should follow the same pattern: one small imports header
per package, and no new vocabulary or generic definitions in
`namespace bind_lang` that name the language (template specializations are the
intended exception, as described in the next section).

### Specializing customization points

C++ does not allow specializing a template through a using-declaration.
Specializations of `bind_lang` class templates must be written in
`namespace bind_lang`, with fully qualified references to the language's types:

```cpp
namespace bind_lang {

template <>
struct LifecycleHelper<jstring, LifecycleType::LOCAL>
    : public ::jni::LifecycleLocalBase<jstring> { ... };

}  // namespace bind_lang
```

### Proxies

A language defines its own `Proxy<CDecl, Enable>` family and key corpus, then
derives its `ProxyHelper` from `GenericProxyHelper`:

```cpp
namespace jni {

template <typename T, typename Enable = void>
struct Proxy;

template <typename TUndecayed>
struct ProxyHelper
    : public bind_lang::GenericProxyHelper<Proxy, AllKeys, TUndecayed> {};

// Primitives with several C++ spellings.
template <typename T>
struct Proxy<T, std::enable_if_t<std::is_same_v<T, jlong>>>
    : public ConvertibleIntegralProxyBase<jlong, long, jlong> {};

// Handle-backed objects.
template <typename T>
struct Proxy<T, std::enable_if_t<std::is_same_v<T, jobject>>>
    : public ObjectProxyBase<jobject, Object, LoaderTag> { ... };

}  // namespace jni
```

`proxy/proxy_test.cc` shows a complete toy language with no JNI dependency.

## Release header

`jni_bind_release.h` is stitched together from every `headers_for_export`
filegroup listed in the root `BUILD` genrules. When you add a `bind_lang`
subpackage that JNI Bind depends on, add its `headers_for_export` to both
`gen_jni_bind_release` and `gen_jni_bind_release_for_testing`.

## Migration log

1.  `build_jni_bind_release.sh` became include-guard aware, so it no longer
    relies on fixed license line counts.
2.  Visibility was constrained: external users depend on `:jni_bind` only.
3.  `metaprogramming/` moved to `bind_lang/metaprogramming/`.
4.  `jni::metaprogramming` was renamed to `bind_lang::metaprogramming`.
5.  `no_idx.h` and `id_type.h` moved to `bind_lang/`.
6.  `ref_base.h` moved, and the `LifecycleHelper` primary template was split
    out into `bind_lang/lifecycle.h`.
7.  Proxy machinery was extracted into `bind_lang/proxy/`.

Candidates for future extraction: the generic `Fake<T>` test constants
(`implementation/jni_helper/fake_test_constants.h`) and the array
abstractions.
