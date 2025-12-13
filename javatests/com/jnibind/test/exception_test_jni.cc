#include <memory>

#include "implementation/local_exception.h"
#include "jni_bind.h"

static std::unique_ptr<jni::JvmRef<jni::kDefaultJvm>> jvm;

namespace {
static constexpr jni::Class kCustomException{
    "com/jnibind/test/CustomException",
    jni::Constructor<jstring>{},
    jni::Method{"getMessage", jni::Return<jstring>{}, jni::Params<>{}},
};

static constexpr jni::Class kClassWithStaticMethod{
    "com/jnibind/test/ClassWithStaticMethod",
    jni::Static{
        jni::Method{"build", jni::Return{jni::kJavaLangException}, jni::Params<>{}},
    },
};

}  // namespace

extern "C" {

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* pjvm, void* reserved) {
  jvm.reset(new jni::JvmRef<jni::kDefaultJvm>(pjvm));
  return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_com_jnibind_test_ExceptionTest_jniTearDown(
    JavaVM* pjvm, void* reserved) {
  jvm = nullptr;
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ExceptionTest_jniThrowException(JNIEnv* env, jobject) {
  jni::LocalException<> exception{"Test failed with unmet requirements"};
  exception.Throw();
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ExceptionTest_jniThrowCustomException(JNIEnv* env, jobject) {
  jni::LocalException<kCustomException> exception{"Custom failure message"};
  exception.Throw();
}

JNIEXPORT void JNICALL
Java_com_jnibind_test_ExceptionTest_jniThrowExceptionFromStaticMethod(
    JNIEnv* env, jobject) {
  jni::LocalException<> exception =
      jni::StaticRef<kClassWithStaticMethod>{}.Call<"build">();
  exception.Throw();
}

}  // extern "C"
