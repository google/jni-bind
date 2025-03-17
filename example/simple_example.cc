// Example of using JNI Bind
#include "jni_bind.h"
#include <iostream>
#include <memory>
#include <string>

// This is a dummy example that shows JNI Bind syntax
int main() {
  std::cout << "JNI Bind Example" << std::endl;
  std::cout << "================" << std::endl;
  
  // In a real application, you would get the JavaVM* from JNI_OnLoad
  // For this example, we'll just use nullptr
  JavaVM* jvm = nullptr;
  
  // In a real application, you would initialize JvmRef
  // static auto jvm_ref = std::make_unique<jni::JvmRef<jni::kDefaultJvm>>(jvm);
  
  std::cout << "In a real application, you would:" << std::endl;
  std::cout << "1. Get a JavaVM* pointer from JNI_OnLoad" << std::endl;
  std::cout << "2. Initialize JvmRef with that pointer" << std::endl;
  std::cout << "3. Define your Java classes with jni::Class" << std::endl;
  std::cout << "4. Create instances and call methods" << std::endl;
  std::cout << std::endl;
  
  std::cout << "Example class definition:" << std::endl;
  std::cout << "=========================" << std::endl;
  std::cout << R"(
  static constexpr jni::Class kMyClass{
    "com/example/MyClass",
    jni::Method{"doSomething", jni::Return<jint>{}, jni::Params<jfloat, jstring>{}},
    jni::Field{"intField", jint{}},
  };
  
  // Create a local object
  jni::LocalObject<kMyClass> obj{};
  
  // Call methods
  int result = obj.Call<"doSomething">(1.5f, "hello");
  
  // Access fields
  obj.Access<"intField">().Set(42);
  int fieldValue = obj.Access<"intField">().Get();
  )" << std::endl;
  
  std::cout << "For more information, see the README.md" << std::endl;
  
  return 0;
}