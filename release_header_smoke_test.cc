#ifndef JNI_BIND_RELEASE_HEADER_SMOKE_TEST_H_
#define JNI_BIND_RELEASE_HEADER_SMOKE_TEST_H_

#include <gtest/gtest.h>
#include "jni_bind_release_for_testing.h"
#include "jni_test.h"

namespace {

using ::jni::Class;
using ::jni::Field;
using ::jni::Method;
using ::jni::Params;
using ::jni::test::JniTest;

TEST_F(JniTest, SmokeTest_SimplyRun) {
  static constexpr Class object{
      "ARCore",
      Method{"Foo", jni::Return<jint>{}, Params<jint, jfloat>{}},
      Method{"Bar", jni::Return{jint{}}, Params<>{}},
      Method{"Baz", jni::Return<void>{}, Params<jfloat>{}},
      Field{"SomeField", jint{}},
  };

  jni::GlobalObject<object> obj{};
  obj("Foo", 1, 2.f);
  obj("Baz", 1.f);
  obj("Baz", 1.f);
  obj("Baz", 2.f);
  obj("Baz", 3.f);
  obj("Bar");
  obj["SomeField"].Get();
}

}  // namespace

#endif  // JNI_BIND_RELEASE_HEADER_SMOKE_TEST_H_
