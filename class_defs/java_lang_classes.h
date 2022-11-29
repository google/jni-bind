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

#ifndef JNI_BIND_CLASS_DEFS_JAVA_LANG_CLASSES_H_
#define JNI_BIND_CLASS_DEFS_JAVA_LANG_CLASSES_H_

#include "implementation/class.h"
#include "implementation/method.h"
#include "implementation/params.h"
#include "implementation/return.h"
#include "jni_dep.h"

namespace jni {

inline constexpr Class kJavaLangClass{"java/lang/Class"};

inline constexpr Class kJavaLangObject{"java/lang/Object"};

inline constexpr Class kJavaLangClassLoader{
    "java/lang/ClassLoader",
    Method{"loadClass", Return{kJavaLangClass}, Params<jstring>{}}};

inline constexpr Method kGetClassLoaderMethod{
    "getClassLoader",
    Return{Method{"loadClass", Return{kJavaLangClass}, Params<jstring>{}}},
    Params{}};

}  // namespace jni

#endif  // JNI_BIND_CLASS_DEFS_JAVA_LANG_CLASSES_H_
