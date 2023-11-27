/*
 * Copyright 2023 Google LLC
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
#include "implementation/fake_test_constants.h"
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using ::jni::AdoptLocal;
using ::jni::Array;
using ::jni::Class;
using ::jni::Field;
using ::jni::LocalArray;
using ::jni::LocalObject;
using ::jni::Rank;
using ::jni::test::Fake;
using ::jni::test::JniTest;
using ::testing::_;
using ::testing::StrEq;

static constexpr Class kClass2{"kClass2"};

////////////////////////////////////////////////////////////////////////////////
// Fields: Smoke Tests.
////////////////////////////////////////////////////////////////////////////////
static constexpr Class kFieldClass{
    "kFieldClass",
    Field{"BooleanArray", Array{jboolean{}}},
    Field{"ByteArray", Array{jbyte{}}},
    Field{"CharArray", Array{jchar{}}},
    Field{"ShortArray", Array{jshort{}}},
    Field{"IntArray", Array{jint{}}},
    Field{"FloatArray", Array{jfloat{}}},
    Field{"DoubleArray", Array{jdouble{}}},
    Field{"LongArray", Array{jlong{}}},
    Field{"ObjectArrayRank1", Array{kClass2}},
    Field{"ObjectArrayRank2", Array{kClass2, Rank<2>{}}},
    Field{"ObjectArrayRank3", Array{kClass2, Rank<3>{}}},
};

TEST_F(JniTest, Array_FieldTests) {
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("BooleanArray"), StrEq("[Z")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ByteArray"), StrEq("[B")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("CharArray"), StrEq("[C")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ShortArray"), StrEq("[S")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("IntArray"), StrEq("[I")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("FloatArray"), StrEq("[F")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("DoubleArray"), StrEq("[D")));
  EXPECT_CALL(*env_, GetFieldID(_, StrEq("LongArray"), StrEq("[J")));
  EXPECT_CALL(*env_,
              GetFieldID(_, StrEq("ObjectArrayRank1"), StrEq("[LkClass2;")));
  EXPECT_CALL(*env_,
              GetFieldID(_, StrEq("ObjectArrayRank2"), StrEq("[[LkClass2;")));
  EXPECT_CALL(*env_,
              GetFieldID(_, StrEq("ObjectArrayRank3"), StrEq("[[[LkClass2;")));

  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillOnce(::testing::Return(Fake<jbooleanArray>()))
      .WillOnce(::testing::Return(Fake<jbyteArray>()))
      .WillOnce(::testing::Return(Fake<jcharArray>()))
      .WillOnce(::testing::Return(Fake<jshortArray>()))
      .WillOnce(::testing::Return(Fake<jintArray>()))
      .WillOnce(::testing::Return(Fake<jfloatArray>()))
      .WillOnce(::testing::Return(Fake<jdoubleArray>()))
      .WillOnce(::testing::Return(Fake<jlongArray>()))
      .WillOnce(::testing::Return(Fake<jobjectArray>(1)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(2)))
      .WillOnce(::testing::Return(Fake<jobjectArray>(3)));

  LocalObject<kFieldClass> obj{AdoptLocal{}, Fake<jobject>()};

  EXPECT_EQ(static_cast<jbooleanArray>(obj["BooleanArray"].Get()),
            Fake<jbooleanArray>());
  EXPECT_EQ(static_cast<jbyteArray>(obj["ByteArray"].Get()),
            Fake<jbyteArray>());
  EXPECT_EQ(static_cast<jcharArray>(obj["CharArray"].Get()),
            Fake<jcharArray>());
  EXPECT_EQ(static_cast<jshortArray>(obj["ShortArray"].Get()),
            Fake<jshortArray>());
  EXPECT_EQ(static_cast<jintArray>(obj["IntArray"].Get()), Fake<jintArray>());
  EXPECT_EQ(static_cast<jfloatArray>(obj["FloatArray"].Get()),
            Fake<jfloatArray>());
  EXPECT_EQ(static_cast<jdoubleArray>(obj["DoubleArray"].Get()),
            Fake<jdoubleArray>());
  EXPECT_EQ(static_cast<jlongArray>(obj["LongArray"].Get()),
            Fake<jlongArray>());
  EXPECT_EQ(static_cast<jobjectArray>(obj["ObjectArrayRank1"].Get()),
            Fake<jobjectArray>(1));
  EXPECT_EQ(static_cast<jobjectArray>(obj["ObjectArrayRank2"].Get()),
            Fake<jobjectArray>(2));
  EXPECT_EQ(static_cast<jobjectArray>(obj["ObjectArrayRank3"].Get()),
            Fake<jobjectArray>(3));
}

TEST_F(JniTest, Array_Field_Boolean_Test) {
  std::unique_ptr<jboolean> fake_storage_ptr(new jboolean());

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("BooleanArray"), StrEq("[Z")))
      .WillRepeatedly(::testing::Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillRepeatedly(::testing::Return(Fake<jbooleanArray>()));
  EXPECT_CALL(*env_, SetObjectField(Fake<jobject>(), Fake<jfieldID>(),
                                    Fake<jbooleanArray>()))
      .Times(4);
  EXPECT_CALL(*env_, GetBooleanArrayElements)
      .WillRepeatedly(::testing::Return(fake_storage_ptr.get()));
  EXPECT_CALL(*env_, ReleaseBooleanArrayElements(Fake<jbooleanArray>(),
                                                 fake_storage_ptr.get(), 0));
  EXPECT_CALL(*env_,
              ReleaseBooleanArrayElements(Fake<jbooleanArray>(),
                                          fake_storage_ptr.get(), JNI_ABORT));

  LocalObject<kFieldClass> obj{AdoptLocal{}, Fake<jobject>()};
  LocalArray<jboolean> arr{obj["BooleanArray"].Get()};
  LocalArray<jboolean> arr2{AdoptLocal{}, Fake<jbooleanArray>()};
  obj["BooleanArray"].Set(Fake<jbooleanArray>());
  obj["BooleanArray"].Set(
      LocalArray<jboolean>{AdoptLocal{}, Fake<jbooleanArray>()});
  obj["BooleanArray"].Set(arr2);
  obj["BooleanArray"].Set(obj["BooleanArray"].Get());

  EXPECT_EQ(obj["BooleanArray"].Get().Pin().ptr(), fake_storage_ptr.get());
  EXPECT_EQ(obj["BooleanArray"].Get().Pin(false).ptr(), fake_storage_ptr.get());
}

TEST_F(JniTest, Array_Field_Byte_Test) {
  std::unique_ptr<jbyte> fake_storage_ptr(new jbyte());

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ByteArray"), StrEq("[B")))
      .WillRepeatedly(::testing::Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillRepeatedly(::testing::Return(Fake<jbyteArray>()));
  EXPECT_CALL(*env_, SetObjectField(Fake<jobject>(), Fake<jfieldID>(),
                                    Fake<jbyteArray>()))
      .Times(4);
  EXPECT_CALL(*env_, GetByteArrayElements)
      .WillRepeatedly(::testing::Return(fake_storage_ptr.get()));
  EXPECT_CALL(*env_, ReleaseByteArrayElements(Fake<jbyteArray>(),
                                              fake_storage_ptr.get(), 0));
  EXPECT_CALL(*env_,
              ReleaseByteArrayElements(Fake<jbyteArray>(),
                                       fake_storage_ptr.get(), JNI_ABORT));

  LocalObject<kFieldClass> obj{AdoptLocal{}, Fake<jobject>()};
  LocalArray<jbyte> arr{obj["ByteArray"].Get()};
  LocalArray<jbyte> arr2{AdoptLocal{}, Fake<jbyteArray>()};
  obj["ByteArray"].Set(Fake<jbyteArray>());
  obj["ByteArray"].Set(LocalArray<jbyte>{AdoptLocal{}, Fake<jbyteArray>()});
  obj["ByteArray"].Set(arr2);
  obj["ByteArray"].Set(obj["ByteArray"].Get());
  EXPECT_EQ(obj["ByteArray"].Get().Pin().ptr(), fake_storage_ptr.get());
  EXPECT_EQ(obj["ByteArray"].Get().Pin(false).ptr(), fake_storage_ptr.get());
}

TEST_F(JniTest, Array_Field_Char_Test) {
  std::unique_ptr<jchar> fake_storage_ptr(new jchar());

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("CharArray"), StrEq("[C")))
      .WillRepeatedly(::testing::Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillRepeatedly(::testing::Return(Fake<jcharArray>()));
  EXPECT_CALL(*env_, SetObjectField(Fake<jobject>(), Fake<jfieldID>(),
                                    Fake<jcharArray>()))
      .Times(4);
  EXPECT_CALL(*env_, GetCharArrayElements)
      .WillRepeatedly(::testing::Return(fake_storage_ptr.get()));
  EXPECT_CALL(*env_, ReleaseCharArrayElements(Fake<jcharArray>(),
                                              fake_storage_ptr.get(), 0));
  EXPECT_CALL(*env_,
              ReleaseCharArrayElements(Fake<jcharArray>(),
                                       fake_storage_ptr.get(), JNI_ABORT));

  LocalObject<kFieldClass> obj{AdoptLocal{}, Fake<jobject>()};
  LocalArray<jchar> arr{obj["CharArray"].Get()};
  LocalArray<jchar> arr2{AdoptLocal{}, Fake<jcharArray>()};
  obj["CharArray"].Set(Fake<jcharArray>());
  obj["CharArray"].Set(LocalArray<jchar>{AdoptLocal{}, Fake<jcharArray>()});
  obj["CharArray"].Set(arr2);
  obj["CharArray"].Set(obj["CharArray"].Get());
  EXPECT_EQ(obj["CharArray"].Get().Pin().ptr(), fake_storage_ptr.get());
  EXPECT_EQ(obj["CharArray"].Get().Pin(false).ptr(), fake_storage_ptr.get());
}

TEST_F(JniTest, Array_Field_Short_Test) {
  std::unique_ptr<jshort> fake_storage_ptr(new jshort());

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("ShortArray"), StrEq("[S")))
      .WillRepeatedly(::testing::Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillRepeatedly(::testing::Return(Fake<jshortArray>()));
  EXPECT_CALL(*env_, SetObjectField(Fake<jobject>(), Fake<jfieldID>(),
                                    Fake<jshortArray>()))
      .Times(4);
  EXPECT_CALL(*env_, GetShortArrayElements)
      .WillRepeatedly(::testing::Return(fake_storage_ptr.get()));
  EXPECT_CALL(*env_, ReleaseShortArrayElements(Fake<jshortArray>(),
                                               fake_storage_ptr.get(), 0));
  EXPECT_CALL(*env_,
              ReleaseShortArrayElements(Fake<jshortArray>(),
                                        fake_storage_ptr.get(), JNI_ABORT));

  LocalObject<kFieldClass> obj{AdoptLocal{}, Fake<jobject>()};
  LocalArray<jshort> arr{obj["ShortArray"].Get()};
  LocalArray<jshort> arr2{AdoptLocal{}, Fake<jshortArray>()};
  obj["ShortArray"].Set(Fake<jshortArray>());
  obj["ShortArray"].Set(LocalArray<jshort>{AdoptLocal{}, Fake<jshortArray>()});
  obj["ShortArray"].Set(arr2);
  obj["ShortArray"].Set(obj["ShortArray"].Get());
  EXPECT_EQ(obj["ShortArray"].Get().Pin().ptr(), fake_storage_ptr.get());
  EXPECT_EQ(obj["ShortArray"].Get().Pin(false).ptr(), fake_storage_ptr.get());
}

TEST_F(JniTest, Array_Field_Int_Test) {
  std::unique_ptr<jint> fake_storage_ptr(new jint());

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("IntArray"), StrEq("[I")))
      .WillRepeatedly(::testing::Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillRepeatedly(::testing::Return(Fake<jintArray>()));
  EXPECT_CALL(*env_, SetObjectField(Fake<jobject>(), Fake<jfieldID>(),
                                    Fake<jintArray>()))
      .Times(4);
  EXPECT_CALL(*env_, GetIntArrayElements)
      .WillRepeatedly(::testing::Return(fake_storage_ptr.get()));
  EXPECT_CALL(*env_, ReleaseIntArrayElements(Fake<jintArray>(),
                                             fake_storage_ptr.get(), 0));
  EXPECT_CALL(*env_, ReleaseIntArrayElements(
                         Fake<jintArray>(), fake_storage_ptr.get(), JNI_ABORT));

  LocalObject<kFieldClass> obj{AdoptLocal{}, Fake<jobject>()};
  LocalArray<jint> arr{obj["IntArray"].Get()};
  LocalArray<jint> arr2{AdoptLocal{}, Fake<jintArray>()};
  obj["IntArray"].Set(Fake<jintArray>());
  obj["IntArray"].Set(LocalArray<jint>{AdoptLocal{}, Fake<jintArray>()});
  obj["IntArray"].Set(arr2);
  obj["IntArray"].Set(obj["IntArray"].Get());
  EXPECT_EQ(obj["IntArray"].Get().Pin().ptr(), fake_storage_ptr.get());
  EXPECT_EQ(obj["IntArray"].Get().Pin(false).ptr(), fake_storage_ptr.get());
}

TEST_F(JniTest, Array_Field_Float_Test) {
  std::unique_ptr<jfloat> fake_storage_ptr(new jfloat());

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("FloatArray"), StrEq("[F")))
      .WillRepeatedly(::testing::Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillRepeatedly(::testing::Return(Fake<jfloatArray>()));
  EXPECT_CALL(*env_, SetObjectField(Fake<jobject>(), Fake<jfieldID>(),
                                    Fake<jfloatArray>()))
      .Times(4);
  EXPECT_CALL(*env_, GetFloatArrayElements)
      .WillRepeatedly(::testing::Return(fake_storage_ptr.get()));
  EXPECT_CALL(*env_, ReleaseFloatArrayElements(Fake<jfloatArray>(),
                                               fake_storage_ptr.get(), 0));
  EXPECT_CALL(*env_,
              ReleaseFloatArrayElements(Fake<jfloatArray>(),
                                        fake_storage_ptr.get(), JNI_ABORT));

  LocalObject<kFieldClass> obj{AdoptLocal{}, Fake<jobject>()};
  LocalArray<jfloat> arr{obj["FloatArray"].Get()};
  LocalArray<jfloat> arr2{AdoptLocal{}, Fake<jfloatArray>()};
  obj["FloatArray"].Set(Fake<jfloatArray>());
  obj["FloatArray"].Set(LocalArray<jfloat>{AdoptLocal{}, Fake<jfloatArray>()});
  obj["FloatArray"].Set(arr2);
  obj["FloatArray"].Set(obj["FloatArray"].Get());
  EXPECT_EQ(obj["FloatArray"].Get().Pin().ptr(), fake_storage_ptr.get());
  EXPECT_EQ(obj["FloatArray"].Get().Pin(false).ptr(), fake_storage_ptr.get());
}

TEST_F(JniTest, Array_Field_Double_Test) {
  std::unique_ptr<jdouble> fake_storage_ptr(new jdouble());

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("DoubleArray"), StrEq("[D")))
      .WillRepeatedly(::testing::Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillRepeatedly(::testing::Return(Fake<jdoubleArray>()));
  EXPECT_CALL(*env_, SetObjectField(Fake<jobject>(), Fake<jfieldID>(),
                                    Fake<jdoubleArray>()))
      .Times(4);
  EXPECT_CALL(*env_, GetDoubleArrayElements)
      .WillRepeatedly(::testing::Return(fake_storage_ptr.get()));
  EXPECT_CALL(*env_, ReleaseDoubleArrayElements(Fake<jdoubleArray>(),
                                                fake_storage_ptr.get(), 0));
  EXPECT_CALL(*env_,
              ReleaseDoubleArrayElements(Fake<jdoubleArray>(),
                                         fake_storage_ptr.get(), JNI_ABORT));

  LocalObject<kFieldClass> obj{AdoptLocal{}, Fake<jobject>()};
  LocalArray<jdouble> arr{obj["DoubleArray"].Get()};
  LocalArray<jdouble> arr2{AdoptLocal{}, Fake<jdoubleArray>()};
  obj["DoubleArray"].Set(Fake<jdoubleArray>());
  obj["DoubleArray"].Set(
      LocalArray<jdouble>{AdoptLocal{}, Fake<jdoubleArray>()});
  obj["DoubleArray"].Set(arr2);
  obj["DoubleArray"].Set(obj["DoubleArray"].Get());
  EXPECT_EQ(obj["DoubleArray"].Get().Pin().ptr(), fake_storage_ptr.get());
  EXPECT_EQ(obj["DoubleArray"].Get().Pin(false).ptr(), fake_storage_ptr.get());
}

TEST_F(JniTest, Array_Field_Long_Test) {
  std::unique_ptr<jlong> fake_storage_ptr(new jlong());

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("LongArray"), StrEq("[J")))
      .WillRepeatedly(::testing::Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillRepeatedly(::testing::Return(Fake<jlongArray>()));
  EXPECT_CALL(*env_, SetObjectField(Fake<jobject>(), Fake<jfieldID>(),
                                    Fake<jlongArray>()))
      .Times(4);
  EXPECT_CALL(*env_, GetLongArrayElements)
      .WillRepeatedly(::testing::Return(fake_storage_ptr.get()));
  EXPECT_CALL(*env_, ReleaseLongArrayElements(Fake<jlongArray>(),
                                              fake_storage_ptr.get(), 0));
  EXPECT_CALL(*env_,
              ReleaseLongArrayElements(Fake<jlongArray>(),
                                       fake_storage_ptr.get(), JNI_ABORT));

  LocalObject<kFieldClass> obj{AdoptLocal{}, Fake<jobject>()};
  LocalArray<jlong> arr{obj["LongArray"].Get()};
  LocalArray<jlong> arr2{AdoptLocal{}, Fake<jlongArray>()};
  obj["LongArray"].Set(Fake<jlongArray>());
  obj["LongArray"].Set(LocalArray<jlong>{AdoptLocal{}, Fake<jlongArray>()});
  obj["LongArray"].Set(arr2);
  obj["LongArray"].Set(obj["LongArray"].Get());
  EXPECT_EQ(obj["LongArray"].Get().Pin().ptr(), fake_storage_ptr.get());
  EXPECT_EQ(obj["LongArray"].Get().Pin(false).ptr(), fake_storage_ptr.get());
}

TEST_F(JniTest, Array_Field_Object_Test) {
  EXPECT_CALL(*env_,
              GetFieldID(_, StrEq("ObjectArrayRank1"), StrEq("[LkClass2;")))
      .WillOnce(::testing::Return(Fake<jfieldID>()));
  EXPECT_CALL(*env_, GetObjectField(Fake<jobject>(), Fake<jfieldID>()))
      .WillRepeatedly(::testing::Return(Fake<jobjectArray>()));
  EXPECT_CALL(*env_, SetObjectField(Fake<jobject>(), Fake<jfieldID>(),
                                    Fake<jobjectArray>()))
      .Times(4);
  EXPECT_CALL(*env_, GetObjectArrayElement(Fake<jobjectArray>(), 2));

  LocalObject<kFieldClass> obj{AdoptLocal{}, Fake<jobject>()};
  LocalArray<jobject, 1, kClass2> arr2{AdoptLocal{}, Fake<jobjectArray>()};
  LocalArray<jobject, 1, kClass2> arr{obj["ObjectArrayRank1"].Get()};
  obj["ObjectArrayRank1"].Set(Fake<jobjectArray>());
  obj["ObjectArrayRank1"].Set(
      LocalArray<jobject, 1, kClass2>{AdoptLocal{}, Fake<jobjectArray>()});
  obj["ObjectArrayRank1"].Set(arr2);
  obj["ObjectArrayRank1"].Set(obj["ObjectArrayRank1"].Get());
  obj["ObjectArrayRank1"].Get().Get(2);
}

TEST_F(JniTest, Array_Field_HandlesLValueLocalObject) {
  static constexpr Class kClass2{"kClass2"};

  static constexpr Class kClass{
      "ArrayMultiTest",
      Field{"Foo", Array{kClass2}},
  };

  EXPECT_CALL(*env_, GetFieldID(_, StrEq("Foo"), StrEq("[LkClass2;")));

  LocalObject<kClass> obj{Fake<jobject>()};
  LocalArray<jobject, 1, kClass2>{obj["Foo"].Get()};
}

}  // namespace
