/*
 * Copyright 2021 Google LLC
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

#ifndef JNI_BIND_JNI_HELPER_JNI_HELPER_H_
#define JNI_BIND_JNI_HELPER_JNI_HELPER_H_

#include <utility>

#include "jni_env.h"
#include "jni_dep.h"

namespace jni {

// Helper JNI shim for object, method, class, etc. lookup.
class JniHelper {
 public:
  // Finds a class with "name".  Note, the classloader used is whatever is
  // present on the stack when this is called.  No caching is performed,
  // returned `jclass` is a local.
  static jclass FindClass(const char* name);

  // Returns a local ref jclass for the given jobject.
  // Note, if the object is polymorphic it may be a sub or superclass.
  static jclass GetObjectClass(jobject object);

  // Gets a method for a signature (no caching is performed).
  static inline jmethodID GetMethodID(jclass clazz, const char* method_name,
                                      const char* method_signature);

  // Gets a static method for a signature (no caching is performed).
  static inline jmethodID GetStaticMethodID(jclass clazz,
                                            const char* method_name,
                                            const char* method_signature);

  // FieldIds follow the same semantics as MethodIds.
  static inline jfieldID GetFieldID(jclass clazz, const char* name,
                                    const char* signature);

  // Gets a static field for a signature (no caching is performed).
  static inline jfieldID GetStaticFieldID(jclass clazz, const char* field_name,
                                          const char* field_signature);

  // Strings.
  static const char* GetStringUTFChars(jstring str);

  static void ReleaseStringUTFChars(jstring str, const char* chars);
};

//==============================================================================

inline jclass JniHelper::FindClass(const char* name) {
  return jni::JniEnv::GetEnv()->FindClass(name);
}

inline jclass JniHelper::GetObjectClass(jobject object) {
  return jni::JniEnv::GetEnv()->GetObjectClass(object);
}

jmethodID JniHelper::GetMethodID(jclass clazz, const char* method_name,
                                 const char* method_signature) {
  return jni::JniEnv::GetEnv()->GetMethodID(clazz, method_name,
                                            method_signature);
}

jmethodID JniHelper::GetStaticMethodID(jclass clazz, const char* method_name,
                                       const char* method_signature) {
  return jni::JniEnv::GetEnv()->GetStaticMethodID(clazz, method_name,
                                                  method_signature);
}

jfieldID JniHelper::GetFieldID(jclass clazz, const char* name,
                               const char* signature) {
  return jni::JniEnv::GetEnv()->GetFieldID(clazz, name, signature);
}

jfieldID JniHelper::GetStaticFieldID(jclass clazz, const char* name,
                                     const char* signature) {
  return jni::JniEnv::GetEnv()->GetStaticFieldID(clazz, name, signature);
}

inline const char* JniHelper::GetStringUTFChars(jstring str) {
  // If is_copy is an address of bool it will be set to true or false if a copy
  // is made.  That said, this seems to be of no consequence, as the API still
  // requires you to release the string at the end. There's no discernible
  // reason you would ever be able to meaningfully act differently based on
  // this parameter of the API (except to do the wrong thing).
  return jni::JniEnv::GetEnv()->GetStringUTFChars(str, /*isCopy=*/nullptr);
}

inline void JniHelper::ReleaseStringUTFChars(jstring str, const char* chars) {
  jni::JniEnv::GetEnv()->ReleaseStringUTFChars(str, chars);
}

}  // namespace jni

#endif  // JNI_BIND_JNI_HELPER_JNI_HELPER_H_
