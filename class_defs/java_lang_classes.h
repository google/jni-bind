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

#include "implementation/array.h"
#include "implementation/class.h"
#include "implementation/constructor.h"
#include "implementation/method.h"
#include "implementation/params.h"
#include "implementation/return.h"
#include "jni_dep.h"

namespace jni {

// clang-format off

inline constexpr Class kJavaLangClass{
  "java/lang/Class",
  Method{"getClassLoader", Return{ Class { "java/lang/ClassLoader" } }},
};

inline constexpr Class kJavaLangObject{
  "java/lang/Object",
  Method{"getClass", Return{kJavaLangClass}},
};

inline constexpr Class kJavaLangClassLoader{
  "java/lang/ClassLoader",
  Method{"loadClass", Return{kJavaLangClass}, Params<jstring>{}},
  Method{"toString", Return{jstring{}}, Params<>{}},
};

static constexpr Class kJavaLangString{
  "java/lang/String",

  Constructor{jstring{}},
  Constructor{Array{jbyte{}}},

  Method{"toString", Return{jstring{}}, Params<>{}},
};
// clang-format on

}  // namespace jni

#endif  // JNI_BIND_CLASS_DEFS_JAVA_LANG_CLASSES_H_
