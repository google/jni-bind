#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::test::JniTest;

constexpr jni::Class c1{
    "c1", jni::Method{"Foo", jni::Return<int>{}, jni::Params<int>{}}};

constexpr jni::Class c2{
    "c2", jni::Extends{c1},
    jni::Method{"Bar", jni::Return<int>{}, jni::Params<int>{}}};

TEST_F(JniTest, Inhertance_CallsSuper) {
  // jni::LocalObject<c1> local_object{};
  // jni::LocalObject<c2> local_object{};
  // local_object("Bar", 123);
  // local_object("Foo", 123);
}

}  // namespace
