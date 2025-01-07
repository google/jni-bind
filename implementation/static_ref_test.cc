/*
 * Copyright 2022 Google LLC
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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "implementation/jni_helper/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptLocal;
using ::jni::Array;
using ::jni::Class;
using ::jni::Fake;
using ::jni::Field;
using ::jni::LocalObject;
using ::jni::LocalString;
using ::jni::Method;
using ::jni::Params;
using ::jni::Rank;
using ::jni::Self;
using ::jni::Static;
using ::jni::StaticRef;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::Return;
using ::testing::StrEq;

////////////////////////////////////////////////////////////////////////////////
// Fields.
////////////////////////////////////////////////////////////////////////////////

static constexpr Class kClass2{"kClass2"};

// clang-format off
static constexpr Class kClass{
  "kClass",
      Static {
        Field{"booleanField", jboolean{}},
        Field{"byteField", jbyte{}},
        Field{"charField", jchar{}},
        Field{"shortField", jshort{}},
        Field{"intField", jint{}},
        Field{"longField", jlong{}},
        Field{"floatField", jfloat{}},
        Field{"doubleField", jdouble{}},
        Field{"stringField", jstring{}},
        Field{"classField", Class{"kClass2"}},
        Field{"selfField", Self{}}
      },
};
// clang-format on

TEST_F(JniTest, StaticBooleanField) {
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("booleanField"), StrEq("Z")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetStaticBooleanField(_, Fake<jfieldID>()))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticBooleanField(_, Fake<jfieldID>(), true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}.Access<"booleanField">().Get());
  StaticRef<kClass>{}.Access<"booleanField">().Set(true);
}

TEST_F(JniTest, StaticByteField) {
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("byteField"), StrEq("B")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetStaticByteField(_, Fake<jfieldID>()))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticByteField(_, Fake<jfieldID>(), true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}.Access<"byteField">().Get());
  StaticRef<kClass>{}.Access<"byteField">().Set(true);
}

TEST_F(JniTest, StaticCharField) {
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("charField"), StrEq("C")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetStaticCharField(_, Fake<jfieldID>()))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticCharField(_, Fake<jfieldID>(), true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}.Access<"charField">().Get());
  StaticRef<kClass>{}.Access<"charField">().Set(true);
}

TEST_F(JniTest, StaticShortField) {
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("shortField"), StrEq("S")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetStaticShortField(_, Fake<jfieldID>()))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticShortField(_, Fake<jfieldID>(), true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}.Access<"shortField">().Get());
  StaticRef<kClass>{}.Access<"shortField">().Set(true);
}

TEST_F(JniTest, StaticIntField) {
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("intField"), StrEq("I")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetStaticIntField(_, Fake<jfieldID>()))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticIntField(_, Fake<jfieldID>(), true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}.Access<"intField">().Get());
  StaticRef<kClass>{}.Access<"intField">().Set(true);
}

TEST_F(JniTest, StaticLongField) {
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("longField"), StrEq("J")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetStaticLongField(_, Fake<jfieldID>()))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticLongField(_, Fake<jfieldID>(), true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}.Access<"longField">().Get());
  StaticRef<kClass>{}.Access<"longField">().Set(true);
}

TEST_F(JniTest, StaticFloatField) {
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("floatField"), StrEq("F")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetStaticFloatField(_, Fake<jfieldID>()))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticFloatField(_, Fake<jfieldID>(), true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}.Access<"floatField">().Get());
  StaticRef<kClass>{}.Access<"floatField">().Set(true);
}

TEST_F(JniTest, StaticDoubleField) {
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("doubleField"), StrEq("D")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetStaticDoubleField(_, Fake<jfieldID>()))
      .WillOnce(Return(true));
  EXPECT_CALL(*env_, SetStaticDoubleField(_, Fake<jfieldID>(), true));

  LocalObject<kClass> obj;
  EXPECT_TRUE(StaticRef<kClass>{}.Access<"doubleField">().Get());
  StaticRef<kClass>{}.Access<"doubleField">().Set(true);
}

TEST_F(JniTest, StaticField_ObjectGet) {
  EXPECT_CALL(*env_,
              GetStaticFieldID(_, StrEq("classField"), StrEq("LkClass2;")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetStaticObjectField(_, Fake<jfieldID>()))
      .WillOnce(Return(Fake<jobject>()));

  jni::LocalObject<kClass2> obj =
      StaticRef<kClass>{}.Access<"classField">().Get();
}

TEST_F(JniTest, StaticField_StringSet) {
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("stringField"),
                                      StrEq("Ljava/lang/String;")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_,
              SetStaticObjectField(_, Fake<jfieldID>(), Fake<jstring>()));

  StaticRef<kClass>{}.Access<"stringField">().Set(
      LocalString{AdoptLocal{}, Fake<jstring>()});
}

TEST_F(JniTest, StaticField_ObjectSet) {
  EXPECT_CALL(*env_,
              GetStaticFieldID(_, StrEq("classField"), StrEq("LkClass2;")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_,
              SetStaticObjectField(_, Fake<jfieldID>(), Fake<jobject>()));

  StaticRef<kClass>{}.Access<"classField">().Set(
      LocalObject<kClass2>{AdoptLocal{}, Fake<jobject>()});
}

TEST_F(JniTest, StaticField_SelfGet) {
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("selfField"), StrEq("LkClass;")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetStaticObjectField(_, Fake<jfieldID>()))
      .WillOnce(Return(Fake<jobject>()));

  jni::LocalObject<kClass> obj =
      StaticRef<kClass>{}.Access<"selfField">().Get();
}

TEST_F(JniTest, StaticField_SelfSet) {
  EXPECT_CALL(*env_, GetStaticFieldID(_, StrEq("selfField"), StrEq("LkClass;")))
      .WillOnce(Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_,
              SetStaticObjectField(_, Fake<jfieldID>(), Fake<jobject>()));

  StaticRef<kClass>{}.Access<"selfField">().Set(
      LocalObject<kClass>{AdoptLocal{}, Fake<jobject>()});
}

////////////////////////////////////////////////////////////////////////////////
// Static Methods.
////////////////////////////////////////////////////////////////////////////////

// clang-format off
static constexpr Class kMethodClass{
  "kMethodClass",
      Static {
        Method{"booleanMethod", ::jni::Return{jboolean{}}},
        Method{"byteMethod", ::jni::Return{jbyte{}}},
        Method{"charMethod", ::jni::Return{jchar{}}},
        Method{"shortMethod", ::jni::Return{jshort{}}},
        Method{"intMethod", ::jni::Return{jint{}}},
        Method{"longMethod", ::jni::Return{jlong{}}},
        Method{"floatMethod", ::jni::Return{jfloat{}}},
        Method{"doubleMethod", ::jni::Return{jdouble{}}},
        Method{"stringMethod", ::jni::Return{jstring{}}},
        Method{"objectMethod", ::jni::Return{Class{"kClass2"}}},
        Method{"rank1ArrayMethod", ::jni::Return{Array{Class{"kClass2"}}}},
        Method{"rank2ArrayMethod", ::jni::Return{Array{Class{"kClass2"}, Rank<2>{}}}},
        Method{"selfMethod", ::jni::Return{Self{}}},

        Method{"simpleFunc", ::jni::Return{int{}}, Params{jfloat{}}},
        Method{"complexFunc", ::jni::Return{float{}},
          Params{
            Array{Class{"kClass2"}, Rank<2>{}}, int{}, float{}, Class{"kClass3"},
          }
        }
      },
};
// clang-format on

TEST_F(JniTest, StaticExerciseAllReturns) {
  EXPECT_CALL(*env_,
              GetStaticMethodID(_, StrEq("booleanMethod"), StrEq("()Z")));
  EXPECT_CALL(*env_, CallStaticBooleanMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("byteMethod"), StrEq("()B")));
  EXPECT_CALL(*env_, CallStaticByteMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("charMethod"), StrEq("()C")));
  EXPECT_CALL(*env_, CallStaticCharMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("shortMethod"), StrEq("()S")));
  EXPECT_CALL(*env_, CallStaticShortMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("intMethod"), StrEq("()I")));
  EXPECT_CALL(*env_, CallStaticIntMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("longMethod"), StrEq("()J")));
  EXPECT_CALL(*env_, CallStaticLongMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("floatMethod"), StrEq("()F")));
  EXPECT_CALL(*env_, CallStaticFloatMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("doubleMethod"), StrEq("()D")));
  EXPECT_CALL(*env_, CallStaticDoubleMethodV);
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("stringMethod"),
                                       StrEq("()Ljava/lang/String;")));

  EXPECT_CALL(
      *env_, GetStaticMethodID(_, StrEq("objectMethod"), StrEq("()LkClass2;")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("rank1ArrayMethod"),
                                       StrEq("()[LkClass2;")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("rank2ArrayMethod"),
                                       StrEq("()[[LkClass2;")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("selfMethod"),
                                       StrEq("()LkMethodClass;")));

  EXPECT_CALL(*env_, CallStaticObjectMethodV).Times(5);

  StaticRef<kMethodClass>{}.Call<"booleanMethod">();
  StaticRef<kMethodClass>{}.Call<"byteMethod">();
  StaticRef<kMethodClass>{}.Call<"charMethod">();
  StaticRef<kMethodClass>{}.Call<"shortMethod">();
  StaticRef<kMethodClass>{}.Call<"intMethod">();
  StaticRef<kMethodClass>{}.Call<"longMethod">();
  StaticRef<kMethodClass>{}.Call<"floatMethod">();
  StaticRef<kMethodClass>{}.Call<"doubleMethod">();
  StaticRef<kMethodClass>{}.Call<"stringMethod">();

  StaticRef<kMethodClass>{}.Call<"objectMethod">();
  StaticRef<kMethodClass>{}.Call<"rank1ArrayMethod">();
  StaticRef<kMethodClass>{}.Call<"rank2ArrayMethod">();

  LocalObject<kClass> self_ret = StaticRef<kMethodClass>{}.Call<"selfMethod">();
}

// clang-format off
static constexpr Class kMethodClassSingleParam{
  "kMethodClassSingleParam",
      Static {
        Method{"booleanMethod", ::jni::Return<void>{}, Params<jboolean>{}},
        Method{"byteMethod", ::jni::Return<void>{}, Params<jbyte>{}},
        Method{"charMethod", ::jni::Return<void>{}, Params<jchar>{}},
        Method{"shortMethod", ::jni::Return<void>{}, Params<jshort>{}},
        Method{"intMethod", ::jni::Return<void>{}, Params<jint>{}},
        Method{"longMethod", ::jni::Return<void>{}, Params<jlong>{}},
        Method{"floatMethod", ::jni::Return<void>{}, Params<jfloat>{}},
        Method{"doubleMethod", ::jni::Return<void>{}, Params<jdouble>{}},
        Method{"stringMethod", ::jni::Return<void>{}, Params<jstring>{}},
        Method{"objectMethod", ::jni::Return<void>{}, Params{Class{"kClass2"}}},
        Method{"rank1ArrayMethod", ::jni::Return<void>{}, Params{Array{Class{"kClass2"}}}},
        Method{"rank2ArrayMethod", ::jni::Return<void>{}, Params{Array{Class{"kClass2"}, Rank<2>{}}}},
        Method{"selfMethod", ::jni::Return<void>{}, Params{Self{}}}
      },
};
// clang-format on

TEST_F(JniTest, StaticExerciseAllTypesThroughSingleParam) {
  EXPECT_CALL(*env_,
              GetStaticMethodID(_, StrEq("booleanMethod"), StrEq("(Z)V")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("byteMethod"), StrEq("(B)V")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("charMethod"), StrEq("(C)V")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("shortMethod"), StrEq("(S)V")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("intMethod"), StrEq("(I)V")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("longMethod"), StrEq("(J)V")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("floatMethod"), StrEq("(F)V")));
  EXPECT_CALL(*env_,
              GetStaticMethodID(_, StrEq("doubleMethod"), StrEq("(D)V")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("stringMethod"),
                                       StrEq("(Ljava/lang/String;)V")));

  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("objectMethod"),
                                       StrEq("(LkClass2;)V")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("rank1ArrayMethod"),
                                       StrEq("([LkClass2;)V")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("rank2ArrayMethod"),
                                       StrEq("([[LkClass2;)V")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("selfMethod"),
                                       StrEq("(LkMethodClassSingleParam;)V")));

  EXPECT_CALL(*env_, CallStaticVoidMethodV).Times(13);

  StaticRef<kMethodClassSingleParam>{}.Call<"booleanMethod">(jboolean{true});
  StaticRef<kMethodClassSingleParam>{}.Call<"byteMethod">(jbyte{1});
  StaticRef<kMethodClassSingleParam>{}.Call<"charMethod">(jchar{'a'});
  StaticRef<kMethodClassSingleParam>{}.Call<"shortMethod">(jshort{1});
  StaticRef<kMethodClassSingleParam>{}.Call<"intMethod">(jint{123});
  StaticRef<kMethodClassSingleParam>{}.Call<"longMethod">(jlong{456});
  StaticRef<kMethodClassSingleParam>{}.Call<"floatMethod">(jfloat{789.f});
  StaticRef<kMethodClassSingleParam>{}.Call<"doubleMethod">(jdouble{101.});
  StaticRef<kMethodClassSingleParam>{}.Call<"stringMethod">("test");

  // It would be more complete to exercise all types here.
  StaticRef<kMethodClassSingleParam>{}.Call<"objectMethod">(Fake<jobject>());
  StaticRef<kMethodClassSingleParam>{}.Call<"rank1ArrayMethod">(
      Fake<jobjectArray>());
  StaticRef<kMethodClassSingleParam>{}.Call<"rank2ArrayMethod">(
      Fake<jobjectArray>());
  StaticRef<kMethodClassSingleParam>{}.Call<"selfMethod">(Fake<jobject>());
}

TEST_F(JniTest, StaticExerciseComplexSetOfParams) {
  // The primary difference for statics are how they handle their returns.
  // Coverage is already fairly extensive for params.
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("simpleFunc"), StrEq("(F)I")));
  EXPECT_CALL(*env_, GetStaticMethodID(_, StrEq("complexFunc"),
                                       StrEq("([[LkClass2;IFLkClass3;)F")));

  StaticRef<kMethodClass>{}.Call<"simpleFunc">(123.f);
  StaticRef<kMethodClass>{}.Call<"complexFunc">(jobjectArray{nullptr}, 123,
                                                456.f, jobject{nullptr});
}

}  // namespace
