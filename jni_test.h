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

#ifndef JNI_BIND_JNI_TEST_H_
#define JNI_BIND_JNI_TEST_H_

#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "mock_jni_env.h"
#include "mock_jvm.h"

#ifdef JNI_BIND_USE_FOR_TESTING_RELEASE_HEADER
#include "jni_bind_release_for_testing.h"
#else
#include "jni_bind.h"
#endif

namespace jni::test {
static constexpr std::size_t kGlobalOffset = 0xBABA0000000000;
static constexpr std::size_t kCopyOffset = 0X100000000000A0;

static constexpr Configuration kDefaultConfiguration{
    .release_class_ids_on_teardown_ = true,
    .release_method_ids_on_teardown_ = true,
};

// "Translates" a fake local object into its global counterpart.
// This obviously isn't doing anything meaningful, however, it provides distinct
// values for anything faking a promotion to a global.
inline jclass AsGlobal(jclass clazz) {
  return reinterpret_cast<jclass>(clazz + kGlobalOffset);
}

template <typename T>
inline T AsNewLocalReference(T obj) {
  return reinterpret_cast<T>(obj + kCopyOffset);
}

inline jobject AsGlobal(jobject object) {
  // Strangely the use of nullptr plus an integral constant causes sanitizer
  // failures.  nullptr is only used as a null (which mock NewObject returns).
  if (object == nullptr) {
    return Fake<jclass>();
  } else {
    return reinterpret_cast<jclass>(object + kGlobalOffset);
  }
}

// JniTest that provides some simple fake behaviour when using mock JNIEnv and
// mock JVM objects.  To skip setting up JvmRef (typical usage), use JniTest.
class JniTestWithNoDefaultJvmRef : public ::testing::Test {
public:
  // Notably, there is no TearDown call that calls Jvm::Destroy, but this is
  // fine, as calling DetachCurrentThread doesn't matter for unit testing.
  void SetUp() override {
    using ::testing::_;
    using ::testing::Return;

    jvm_ = std::make_unique<MockJvm>();
    env_ = std::make_unique<MockJniEnv>();

    // Different jni.h implementations define AttachCurrentThread with different
    // args.  If a different jni.h is used for testing, this would likely need
    // to be updated.
    static_assert(std::is_same_v<metaprogramming::FunctionTraitsArg_t<
                                   decltype(&JavaVM::AttachCurrentThread), 1>,
                                 void**>);

    ON_CALL(*jvm_, GetEnv)
        .WillByDefault(testing::Invoke([&](void** out_env, int vs_code) {
                            *reinterpret_cast<JNIEnv**>(out_env) = env_.get();
                            return JNI_OK;
                          }));
    ON_CALL(*jvm_, AttachCurrentThread)
        .WillByDefault(testing::Invoke([&](void** out_env, void*) {
                                         *reinterpret_cast<JNIEnv**>(out_env) =
                                             env_.get();
                                         return JNI_OK;
                                       }));

    // It's tedious to have logic in unit tests for the creation and destruction
    // of the global objects for jclasses.  This gives reasonable non-null
    // values to improve debugging and avoid behaviour based on null paths.
    //
    // It's worth noting that you can't omit the deletion.  If *any* constraints
    // are offered for a test, *all* invocations become strict.  If the consumer
    // of the fixture EXPECT_CALLs a DeleteGlobalRef (for their own custom class
    // e.g.) then the DeleteGlobalRef in JvmRef's dtor will fail.
    //
    // Why go through all this hassle?  Unfortunately, classes will reload if
    // they are null, so this causes behaviour unreflective of the tests that
    // exercise FindClass code.  You can delete this block and |TearDown|, and
    // only those tests will fail.
    ON_CALL(*env_, FindClass).WillByDefault(Return(Fake<jclass>()));
    ON_CALL(*env_, GetMethodID).WillByDefault(Return(Fake<jmethodID>()));
    ON_CALL(*env_, GetFieldID).WillByDefault(Return(Fake<jfieldID>()));
    ON_CALL(*env_, NewObjectArray).WillByDefault(Return(Fake<jobjectArray>()));
    ON_CALL(*env_, NewObjectV).WillByDefault(Return(Fake<jobject>()));

    ON_CALL(*env_, NewLocalRef)
        .WillByDefault(testing::Invoke(
                                   [&](jobject object) {
                                     return AsNewLocalReference(object);
                                   }));

    ON_CALL(*env_, NewGlobalRef)
        .WillByDefault(testing::Invoke([&](jobject object) {
                                  jobject return_value = AsGlobal(object);
                                  if (return_value ==
                                      AsGlobal(Fake<jclass>())) {
                                    default_globals_made_that_should_be_released_
                                        .push_back(
                                            return_value);
                                  }
                                  return return_value;
                                }));
  }

  void TearDown() override {
    if (!default_globals_made_that_should_be_released_.empty()) {
      EXPECT_CALL(*env_, DeleteGlobalRef(AsGlobal(Fake<jclass>())))
          .Times(default_globals_made_that_should_be_released_.size());
    }

    default_globals_made_that_should_be_released_.clear();
  }

protected:
  std::vector<jobject> default_globals_made_that_should_be_released_;

  std::unique_ptr<MockJvm> jvm_;
  std::unique_ptr<MockJniEnv> env_;
};

// Standard test with default Jvm setup and teardown.
// Set expectations on inherited members |jvm_| and |env_|.
class JniTest : public JniTestWithNoDefaultJvmRef {
public:
  void SetUp() override {
    // This mimics a JNI_OnLoad call.
    // https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/invocation.html#JNJI_OnLoad
    (*this).JniTestWithNoDefaultJvmRef::SetUp();
    default_jvm_ref_ = std::make_unique<jni::JvmRef<jni::kDefaultJvm>>(
        jvm_.get(), kDefaultConfiguration);
  }

  void TearDown() override {
    (*this).JniTestWithNoDefaultJvmRef::TearDown();

    // This mimics a JNI_OnUnLoad call.
    // https://docs.oracle.com/javase/8/docs/technotes/guides/jni/spec/invocation.html#JNI_OnUnload
    default_jvm_ref_ = nullptr;
  }

  std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> default_jvm_ref_;
};
} // namespace jni::test

#endif  // JNI_BIND_JNI_TEST_H_
