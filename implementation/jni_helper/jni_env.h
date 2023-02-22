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

#ifndef JNI_BIND_JNI_HELPER_JNI_ENV_H_
#define JNI_BIND_JNI_HELPER_JNI_ENV_H_

#include <cassert>

#include "jni_dep.h"

namespace jni {

template <const auto& jvm_v_>
class JvmRef;
class ThreadGuard;

// This class represents a static accessor for a ::JNIEnv*.
//
// JNIEnv* is passed into each JNI function but cannot be safely cached because
// it is not safe to share across threads.  That said, it is safe to save across
// the same thread.
//
// Thread local accesses ought to be free because this header will only ever be
// used in JNI implementations and in a statically linked context:
//    http://david-grs.github.io/tls_performance_overhead_cost_linux/
//
// The contract requires that any new thread must have Jvm::ThreadInit
// called once on every new thread (single threaded apps do not need to).
//
// In a single threaded context if an object is wrapped or created, the JniEnv
// will be cached in a thread local static.  If a std::unique_ptr<GlobalObject>
// were to be moved onto a new thread no JNIEnv* would be cached.  You could
// cache the JNIEnv on every move constructor, but this would almost certainly
// result in unnecessary and excessive writes.
class JniEnv {
 public:
  static inline JNIEnv* GetEnv() {
    assert(env_ != nullptr && "remember to create a jni::JvmRef");
    return env_;
  }

 protected:
  template <const auto& jvm_v_>
  friend class JvmRef;
  friend class ThreadGuard;

  static inline void SetEnv(JNIEnv* env) { env_ = env; }

  // This will always be set when a new object is created (see above).
  static inline thread_local JNIEnv* env_;
};

}  // namespace jni

#endif  // JNI_BIND_JNI_HELPER_JNI_ENV_H_
