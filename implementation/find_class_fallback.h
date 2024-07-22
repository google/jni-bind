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

#ifndef JNI_BIND_IMPLEMENTATION_FIND_CLASS_FALLBACK_H_
#define JNI_BIND_IMPLEMENTATION_FIND_CLASS_FALLBACK_H_

// IWYU pragma: private, include "../jni_bind.h"

#include "implementation/default_class_loader.h"
#include "implementation/global_class_loader.h"
#include "implementation/jni_helper/jni_helper.h"
#include "implementation/local_object.h"
#include "implementation/promotion_mechanics_tags.h"
#include "jni_dep.h"

namespace jni {

// This is currently using JNI Bind itself and so cannot be directly included.
// By delaying linkage for this we can use a bootstrapped JNI Bind. This could
// be rewritten in porcelain JNI, but this flow ought to be only on
// secondary threads with previously unused `jclass`'s (which is mostly rare),
// and this is simpler to reason about than porcelain JNI.
inline jclass FindClassFallback(const char* class_name) {
  // The loader will be primed by the JVM, however, it needs to be accessible
  // from the jni_helper layer. See `JvmRef` for how this is primed.
  GlobalClassLoader<kDefaultClassLoader> loader{AdoptGlobal{},
                                                FallbackLoader()};

  jni::LocalObject loaded_class = loader("loadClass", class_name);
  jclass ret{static_cast<jclass>(static_cast<jobject>(loaded_class.Release()))};

  loader.Release();

  return ret;
}

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_FIND_CLASS_FALLBACK_H_
