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

#include "class.h"
#include "jni_dep.h"
#include "method.h"
#include "params.h"
#include "return.h"

namespace jni {

inline constexpr Class kJavaLangClass{"java/lang/Class"};

inline constexpr Class kJavaLangObject{"java/lang/Object"};

inline constexpr Method kLoadClassMethod{"loadClass", Return{kJavaLangClass},
                                         Params<jstring>{}};

inline constexpr Class kJavaLangClassLoader{"java/lang/ClassLoader",
                                            kLoadClassMethod};

// TODO (b/143908983): This should be in kJavaLangClass, but we currently
// cannot represent dependency loops (ClassLoader.findClass() returns a class,
// Class.getClassLoader() returns a ClassLoader) without a dummy class.
inline constexpr Method kGetClassLoaderMethod{
    "getClassLoader", Return{kJavaLangClassLoader}, Params{}};

}  // namespace jni

#endif  // JNI_BIND_CLASS_DEFS_JAVA_LANG_CLASSES_H_
