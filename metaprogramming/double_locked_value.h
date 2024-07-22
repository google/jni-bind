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

#ifndef JNI_BIND_METAPROGRAMMING_DOUBLE_LOCKED_VALUE_H_
#define JNI_BIND_METAPROGRAMMING_DOUBLE_LOCKED_VALUE_H_

#include <atomic>
#include <mutex>

namespace jni::metaprogramming {

// This class is intended to mimic a function local static except that it
// supports being reset. The object is constructed with a lambda and all
// subsequent loads will be thread safe, invoking the lambda once (and only
// once) if the currently stored value is T{0}.
//
// This class is thread-safe.  Loads will be cheap (after a potentially
// expensive initial init), stores are expensive.
template <typename T_>
class DoubleLockedValue {
 public:
  template <typename Lambda>
  inline T_ LoadAndMaybeInit(Lambda lambda) {
    // Typical case, value already initialised, perform cheap load and return.
    T_ return_value = value_.load(std::memory_order_acquire);

    if (return_value != T_{0}) {
      return return_value;
    }

    // Value was nil (uninitialised), perform heavy-weight lock.
    std::lock_guard<std::mutex> lock_guard{lock_};

    // Check another thread didn't race to lock before.
    return_value = value_.load(std::memory_order_acquire);
    if (return_value != T_{}) {
      return return_value;
    }

    // Perform the potentially expensive initialisation and return.
    return_value = lambda();
    value_.store(return_value, std::memory_order_release);
    return return_value;
  }

  // Sets the value to {0}.
  inline void Reset() {
    std::lock_guard<std::mutex> lock_guard{lock_};
    value_.store(0, std::memory_order_release);
  }

  // Sets the value to {0} and iff the value was not {0} prior to being torn
  // down, the teardown lambda will be invoked with this value.
  template <typename TeardownLambda>
  inline void Reset(TeardownLambda lambda) {
    std::lock_guard<std::mutex> lock_guard{lock_};
    auto val = value_.load();
    if (val != 0) {
      lambda(val);
      value_.store(0, std::memory_order_release);
    }
  }

 private:
  std::atomic<T_> value_ = {0};
  std::mutex lock_;
};

}  // namespace jni::metaprogramming

#endif  // JNI_BIND_METAPROGRAMMING_DOUBLE_LOCKED_VALUE_H_
