/*
 * Copyright 2024 Google LLC
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

#ifndef JNI_BIND_CLASS_DEFS_JAVA_LANG_EXCEPTION_H_
#define JNI_BIND_CLASS_DEFS_JAVA_LANG_EXCEPTION_H_

#include "class_defs/java_lang_throwable.h"
#include "implementation/class.h"
#include "implementation/constructor.h"
#include "implementation/method.h"
#include "implementation/params.h"
#include "implementation/return.h"
#include "jni_dep.h"

namespace jni {

static constexpr Class kJavaLangException{
    "java/lang/Exception",
    Constructor{},
    Constructor<jstring>{},
    Constructor{jstring{}, kJavaLangThrowable, jboolean{}, jboolean{}},
    Constructor{kJavaLangThrowable},

    // Inherited.
    Method{"getMessage", Return{jstring{}}, Params<>{}},
    Method{"printStackTrace", Return{}, Params<>{}},
    Method{"toString", Return{jstring{}}, Params<>{}},
};

}  // namespace jni

#endif  // JNI_BIND_CLASS_DEFS_JAVA_LANG_EXCEPTION_H_
