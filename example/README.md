# JNI Bind Example

This directory contains a simple example that demonstrates how to use JNI Bind with CMake.

## Building the Example

### Option 1: Build as part of the main project

```bash
# From the root directory of jni-bind
mkdir cmake-build && cd cmake-build
cmake -DJNI_BIND_BUILD_EXAMPLE=ON ..
cmake --build .
```

### Option 2: Build standalone (after installing jni-bind)

```bash
# From this example directory
mkdir cmake-build && cd cmake-build
cmake ..
cmake --build .
```

## Running the Example

```bash
./simple_example
```

## Notes

This is a simple example that doesn't actually connect to a JVM. It just demonstrates the JNI Bind syntax. In a real application, you would:

1. Get a JavaVM* pointer from JNI_OnLoad
2. Initialize JvmRef with that pointer
3. Define your Java classes with jni::Class
4. Create objects and call methods

For a more complete example that actually interacts with Java, see the tests in the parent directory.

## Creating a Real JNI Application

For a real application that uses JNI Bind, you would typically:

1. Create a Java class with native methods:

```java
package com.example;

public class MyClass {
    static {
        System.loadLibrary("my_native_lib");
    }
    
    public native int doSomethingNative(float f, String s);
}
```

2. Create a C++ file that implements the native methods using JNI Bind:

```cpp
#include "jni_bind.h"
#include <memory>

static constexpr jni::Class kMyClass{
  "com/example/MyClass",
  jni::Method{"doSomethingNative", jni::Return<jint>{}, jni::Params<jfloat, jstring>{}},
};

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm_ref;

extern "C" JNIEXPORT jint JNICALL
Java_com_example_MyClass_doSomethingNative(JNIEnv* env, jclass clazz, jfloat f, jstring s) {
  // Process inputs
  jni::UtfStringView str = jni::LocalString{s}.Pin();
  std::string_view cpp_str = str.ToString();
  
  // Do something with the inputs
  float result = f * 2;
  
  return static_cast<jint>(result);
}

extern "C" JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void* reserved) {
  jvm_ref = std::make_unique<jni::JvmRef<jni::kDefaultJvm>>(vm);
  return JNI_VERSION_1_6;
}
```

3. Build your native library with CMake, linking against JNI Bind:

```cmake
add_library(my_native_lib SHARED my_native_lib.cc)
target_link_libraries(my_native_lib PRIVATE jni_bind ${JNI_LIBRARIES})
```

This provides a general starting point for using JNI Bind in real applications.