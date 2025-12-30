/*
 * Copyright 2026 Google LLC
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

#ifndef JNI_BIND_CLASS_DEFS_ANDROID_APPLICATION_H_
#define JNI_BIND_CLASS_DEFS_ANDROID_APPLICATION_H_

#include "class_defs/java_lang_classes.h"
#include "implementation/class.h"
#include "implementation/method.h"
#include "implementation/return.h"

namespace jni {

static constexpr Class kApplicationClass{
    "android/app/Application",
    Method{"getClassLoader", Return{kJavaLangClassLoader}},
};

}  // namespace jni

#endif  // JNI_BIND_CLASS_DEFS_ANDROID_APPLICATION_H_
