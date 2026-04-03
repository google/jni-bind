<!-- disableFinding(LINE_OVER_80) -->
<!-- disableFinding(WHITESPACE_TRAILING) -->
# JNI Bind Usage & Integration Guidelines

This document provides foundational guidance for using **JNI Bind**, a
header-only C++20 metaprogramming library that simplifies JNI interactions.

> [!IMPORTANT]
> **The Golden Rule:** Do not use raw ("porcelain") JNI (e.g.,
> `env->CallIntMethod`, `jobject` handles, manual `DeleteLocalRef`) unless
> absolutely necessary. JNI Bind provides type-safe, RAII-compliant wrappers
> for all standard JNI operations.

---

## 1. Global Setup

The library requires a `jni::JvmRef` to manage the `JavaVM` and thread
attachment.

```cpp
#include "third_party/jni_wrapper/jni_bind.h"

// This object must outlive all JNI Bind calls.
static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm = std::make_unique<jni::JvmRef<jni::kDefaultJvm>>(pjvm);
  return JNI_VERSION_1_6;
}
```

---

## 2. Defining Classes

Define Java classes as `static constexpr jni::Class`.

```cpp
static constexpr jni::Class kMyClass{
    "com/example/MyClass",
    jni::Constructor{jint{}},
    jni::Method{"foo", jni::Return<jint>{}, jni::Params<jfloat>{}},
    jni::Field{"bar", jint{}}
};
```

### Overloads
Use `jni::Overload` within `jni::Method` to handle multiple signatures.

```cpp
jni::Method{"foo",
  jni::Overload{jni::Return<int>{}, jni::Params<>{}},
  jni::Overload{jni::Return<int>{}, jni::Params<jfloat>{}}
}
```

### Builders (Self)
For methods that return `this`, use `jni::Self{}` to maintain full class
decoration.

```cpp
jni::Method{"setVal", jni::Return{jni::Self{}}, jni::Params<jint>{}}
```

---

## 3. Objects & Lifetime

*   **`jni::LocalObject`**: RAII wrapper for local `jobject` references.
*   **`jni::GlobalObject`**: RAII wrapper for global `jobject` references (thread-safe).

```cpp
// Construction from Java jobject
jni::LocalObject<kMyClass> obj{java_jobject};

// Manual construction from C++
jni::LocalObject<kMyClass> new_obj{123};

// Returning to Java (Releases RAII ownership)
return obj.Release();
```

---

## 4. Methods & Fields

### Calling Methods
```cpp
jint result = obj.Call<"foo">(1.5f);
```

### Accessing Fields
```cpp
obj.Access<"bar">().Set(42);
jint val = obj.Access<"bar">().Get();
```

---

## 5. Strings & Arrays

### Strings
Use `jni::LocalString` or `jni::GlobalString`. Use `PinAsStr()` for raw access.

```cpp
jni::LocalString my_str{"Hello world"};
std::string cpp_str = my_str.PinAsStr().ToString();
```

### Arrays
Use `jni::LocalArray`. Use `Pin()` for high-performance bulk access.

```cpp
jni::LocalArray<jint> arr{10};
{
    auto view = arr.Pin();
    view.ptr()[0] = 100;
}
```

---

## 6. Statics

Access static methods/fields via `jni::StaticRef`.

```cpp
static constexpr jni::Class kHelper{
    "com/example/Helper",
    jni::Static{
        jni::Method{"staticFoo", jni::Return<void>{}}
    }
};

jni::StaticRef<kHelper>{}.Call<"staticFoo">();
```

---

## 7. Exceptions

Use `jni::LocalException` to catch, build, and throw Java exceptions.

```cpp
static constexpr jni::Class kCustomException{"com/example/MyException"};

void ThrowSomething() {
    jni::LocalException<kCustomException>{"Failed!"}.Throw();
}
```

---

## 8. ClassLoaders

Advanced loading from non-primordial loaders.

```cpp
static constexpr jni::ClassLoader kLoader{
    jni::kDefaultClassLoader, jni::SupportedClassSet{kMyClass}};

jni::LocalClassLoader<kLoader> loader{java_loader_obj};
auto my_obj = loader.BuildLocalObject<kMyClass>();
```

---

## 9. Multi-threading

Use `jni::ThreadGuard` in every new native thread.

```cpp
std::thread([]() {
  jni::ThreadGuard guard;
  jni::StaticRef<kMyClass>{}.Call<"foo">();
}).detach();
```

---

## 10. Development & Maintenance

### Running Tests
```bash
blaze test //third_party/jni_wrapper/...
```

### Release Management
To update `jni_bind_release.h`:
1. `blaze build //third_party/jni_wrapper:gen_jni_bind_release`
2. `cp ../../blaze-genfiles/third_party/jni_wrapper/jni_bind_release.h jni_bind_release.h`
