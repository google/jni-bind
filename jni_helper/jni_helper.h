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

/**
// JniHelper is a shim to using a JNIenv object.
// This extra layer of indirection allows for quickly shimming all JNICalls
// (e.g. adding exception checking, logging each JNI call, etc).
**/
class JniHelper {
 public:
  // Finds a class with "name".  Note, the classloader used is whatever is
  // present on the statck when this is caled.  No caching is performed,
  // returned jclass is a local.
  static jclass FindClass(const char* name);

  // Returns a local ref jclass for the given jobject.
  // Note, if the object is polymorphic it may be a sub or superclass.
  static jclass GetObjectClass(jobject object);

  // See FindClass and jni::Jvm.
  static void ReleaseClass(jclass clazz);

  // Gets a method for a signature (no caching is performed).
  static inline jmethodID GetMethodID(jclass clazz, const char* method_name,
                                      const char* method_signature);

  // FieldIds follow the same semantics as MethodIds.
  static inline jfieldID GetFieldID(jclass clazz, const char* name,
                                    const char* signature);

  // Objects.
  template <typename... CtorArgs>
  static jobject NewLocalObject(jclass clazz, jmethodID ctor_method,
                                CtorArgs&&... ctor_args);

  // Creates a new GlobalRef to |local_object|, then deletes the local
  // reference.
  static jobject PromoteLocalToGlobalObject(jobject local_object);

  // Creates a new GlobalRef to |local_class|, then deletes the local
  // reference.
  static jclass PromoteLocalToGlobalClass(jclass local_class);

  static void DeleteLocalObject(jobject object);

  template <typename... CtorArgs>
  static jobject NewGlobalObject(jclass clazz, jmethodID ctor_method,
                                 CtorArgs&&... ctor_args);

  static void DeleteGlobalObject(jobject obj_ref);

  // Strings.
  static jstring NewLocalString(const char*);

  // Creates a new GlobalRef to |local_string| , then deletes the local string.
  static jstring PromoteLocalToGlobalString(jstring local_string);

  static void DeleteGlobalString(jstring string);

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

inline void JniHelper::ReleaseClass(jclass clazz) {
  jni::JniEnv::GetEnv()->DeleteGlobalRef(clazz);
}

jmethodID JniHelper::GetMethodID(jclass clazz, const char* method_name,
                                 const char* method_signature) {
  return jni::JniEnv::GetEnv()->GetMethodID(clazz, method_name,
                                            method_signature);
}

jfieldID JniHelper::GetFieldID(jclass clazz, const char* name,
                               const char* signature) {
  return jni::JniEnv::GetEnv()->GetFieldID(clazz, name, signature);
}

template <typename... CtorArgs>
jobject JniHelper::NewLocalObject(jclass clazz, jmethodID ctor_method,
                                  CtorArgs&&... ctor_args) {
  return jni::JniEnv::GetEnv()->NewObject(clazz, ctor_method, ctor_args...);
}

inline void JniHelper::DeleteLocalObject(jobject object) {
  jni::JniEnv::GetEnv()->DeleteLocalRef(object);
}

inline jobject JniHelper::PromoteLocalToGlobalObject(jobject local_object) {
  JNIEnv* const env = jni::JniEnv::GetEnv();
  jobject global_object = env->NewGlobalRef(local_object);
  env->DeleteLocalRef(local_object);
  return global_object;
}

inline jclass JniHelper::PromoteLocalToGlobalClass(jclass local_class) {
  return reinterpret_cast<jclass>(JniEnv::GetEnv()->NewGlobalRef(local_class));
}

template <typename... CtorArgs>
inline jobject JniHelper::NewGlobalObject(jclass clazz, jmethodID ctor_method,
                                          CtorArgs&&... ctor_args) {
  // Note, this local ref handle created below is never leaked outside of this
  // scope and should naturally be cleaned up when invoking JNI function
  // completes.  That said, the maximum number of local refs can be extremely
  // limited (the standard only requires 16), and if the caller doesn't
  // explicitly reach for the performant option, it doesn't make sense to
  // provide a micro optimisation of skipping the delete call below.
  //
  // If consumers want the most performant option, they should use LocalRef
  // implementations when building their dynamic object.
  JNIEnv* const env = jni::JniEnv::GetEnv();
  jobject local_object = NewLocalObject(env, clazz, ctor_method,
                                        std::forward<CtorArgs>(ctor_args)...);
  jobject global_object = env->NewGlobalRef(local_object);

  env->DeleteLocalRef(local_object);

  return global_object;
}

inline void JniHelper::DeleteGlobalObject(jobject obj_ref) {
  jni::JniEnv::GetEnv()->DeleteGlobalRef(obj_ref);
}

inline jstring JniHelper::NewLocalString(const char* chars) {
  return jni::JniEnv::GetEnv()->NewStringUTF(chars);
}

inline jstring JniHelper::PromoteLocalToGlobalString(jstring local_string) {
  // jstrings follow the semantics of regular objects.
  JNIEnv* const env = jni::JniEnv::GetEnv();
  jstring global_string = static_cast<jstring>(env->NewGlobalRef(local_string));
  env->DeleteLocalRef(local_string);
  return global_string;
}

inline void JniHelper::DeleteGlobalString(jstring string) {
  jni::JniEnv::GetEnv()->DeleteGlobalRef(string);
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
