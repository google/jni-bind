#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

#if __clang__

using ::jni::AdoptLocal;
using ::jni::Class;
using ::jni::ClassLoader;
using ::jni::Constructor;
using ::jni::Fake;
using ::jni::Jvm;
using ::jni::JvmRef;
using ::jni::kNullClassLoader;
using ::jni::LocalClassLoader;
using ::jni::Method;
using ::jni::Params;
using ::jni::Return;
using ::jni::SupportedClassSet;
using ::jni::test::AsGlobal;
using ::jni::test::JniTestWithNoDefaultJvmRef;
using ::testing::_;
using ::testing::InSequence;
using ::testing::StrEq;

// This test is isolated to correctly observe querying to Class + ClassLoader
// class definitions. Ids are now cached statically against their name, so this
// results in crosstalk.
TEST_F(JniTestWithNoDefaultJvmRef,
       ClassLoaderRefTest_ClassLoadersDoNotConflict) {
  static constexpr Class kClass{
      "com/google/kClass", Constructor<jint>{},
      Method{"methodNoCrossTalk", Return<jint>{}, Params<jint>{}}};
  static constexpr ClassLoader kClassLoader{kNullClassLoader,
                                            SupportedClassSet{kClass}};

  // We will use this ClassLoader instead of the default loader to load
  // Class.
  static constexpr Jvm kClassLoaderJvm{kClassLoader};

  InSequence sequence;

  EXPECT_CALL(*env_, FindClass(StrEq("java/lang/ClassLoader")))
      .WillOnce(testing::Return(Fake<jclass>(1)));

  EXPECT_CALL(*env_, NewGlobalRef(Fake<jclass>(1)))
      .WillOnce(testing::Return(AsGlobal(Fake<jclass>(1))));

  EXPECT_CALL(*env_,
              GetMethodID(AsGlobal(Fake<jclass>(1)), StrEq("loadClass"),
                          StrEq("(Ljava/lang/String;)Ljava/lang/Class;")))
      .WillOnce(testing::Return(Fake<jmethodID>(1)));

  EXPECT_CALL(*env_, NewStringUTF(_))
      .WillOnce(testing::Return(Fake<jstring>()));

  // We should only try to load the class once even if we create multiple
  // instances.
  EXPECT_CALL(*env_, CallObjectMethodV(Fake<jobject>(1), Fake<jmethodID>(1), _))
      .WillOnce(testing::Return(Fake<jclass>(2)));

  EXPECT_CALL(*env_, NewGlobalRef(Fake<jclass>(2)))
      .WillOnce(testing::Return(AsGlobal(Fake<jclass>(2))));
  EXPECT_CALL(*env_, GetMethodID(AsGlobal(Fake<jclass>(2)), StrEq("<init>"),
                                 StrEq("(I)V")))
      .WillOnce(testing::Return(Fake<jmethodID>(2)));
  EXPECT_CALL(*env_,
              NewObjectV(AsGlobal(Fake<jclass>(2)), Fake<jmethodID>(2), _))
      .WillOnce(testing::Return(Fake<jobject>(2)));
  EXPECT_CALL(*env_,
              NewObjectV(AsGlobal(Fake<jclass>(2)), Fake<jmethodID>(2), _))
      .WillOnce(testing::Return(Fake<jobject>(3)));

  EXPECT_CALL(*env_,
              GetMethodID(AsGlobal(Fake<jclass>(2)), StrEq("methodNoCrossTalk"),

                          StrEq("(I)I")))
      .WillOnce(testing::Return(Fake<jmethodID>(3)));
  EXPECT_CALL(*env_, CallIntMethodV(Fake<jobject>(2), Fake<jmethodID>(3), _))
      .WillOnce(testing::Return(123));

  // Code under test.
  JvmRef<kClassLoaderJvm> jvm_ref{jvm_.get()};
  LocalClassLoader<kClassLoader, kClassLoaderJvm> class_loader{
      AdoptLocal{}, Fake<jobject>(1)};

  auto custom_loader_object = class_loader.BuildLocalObject<kClass>(jint{1});

  auto second_custom_loader_object =
      class_loader.BuildLocalObject<kClass>(jint{2});

  EXPECT_EQ(custom_loader_object("methodNoCrossTalk", jint{2}), 123);

  TearDown();
}

#endif  // __clang__

}  // namespace
