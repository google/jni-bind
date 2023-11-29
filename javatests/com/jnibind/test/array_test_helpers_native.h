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

#ifndef JNI_BIND_JAVATESTS_COM_JNIBIND_TEST_ARRAY_TEST_HELPERS_NATIVE_H_
#define JNI_BIND_JAVATESTS_COM_JNIBIND_TEST_ARRAY_TEST_HELPERS_NATIVE_H_

#include "object_test_helper_jni.h"
#include "implementation/method.h"
#include "jni_bind.h"

// Shared static methods to validate method and field arrays.
// Currently WIP, as tests are migrated while fields reach parity.
constexpr jni::Class kArrayTestHelperClass{
    "com/jnibind/test/ArrayTestHelpers",

    ::jni::Static{

        // Rank 0 Verification.
        ::jni::Method{"assertString", ::jni::Return{},
                      ::jni::Params{jstring{}, jstring{}}},
        ::jni::Method{"assertObject", ::jni::Return{},
                      ::jni::Params{jint{}, kObjectTestHelperClass}},

        // Rank 1 Verification.
        ::jni::Method{
            "assertBoolean1D", ::jni::Return{},
            ::jni::Params{jboolean{}, jboolean{}, ::jni::Array<jboolean>{}}},
        ::jni::Method{"assertByte1D", ::jni::Return{},
                      ::jni::Params{jbyte{}, jbyte{}, ::jni::Array<jbyte>{}}},
        ::jni::Method{"assertChar1D", ::jni::Return{},
                      ::jni::Params{jchar{}, jchar{}, ::jni::Array<jchar>{}}},
        ::jni::Method{
            "assertShort1D", ::jni::Return{},
            ::jni::Params{jshort{}, jshort{}, ::jni::Array<jshort>{}}},
        ::jni::Method{"assertInt1D", ::jni::Return{},
                      ::jni::Params{jint{}, jint{}, ::jni::Array<jint>{}}},
        ::jni::Method{"assertLong1D", ::jni::Return{},
                      ::jni::Params{jlong{}, jlong{}, ::jni::Array<jlong>{}}},
        ::jni::Method{
            "assertFloat1D", ::jni::Return{},
            ::jni::Params{jfloat{}, jfloat{}, ::jni::Array<jfloat>{}}},
        ::jni::Method{
            "assertDouble1D", ::jni::Return{},
            ::jni::Params{jdouble{}, jdouble{}, ::jni::Array<jdouble>{}}},
        ::jni::Method{"assertString1D", ::jni::Return{},
                      ::jni::Params{::jni::Array{jstring{}}, jboolean{}}},
        ::jni::Method{
            "assertObject1D", ::jni::Return{},
            ::jni::Params{jint{}, ::jni::Array{kObjectTestHelperClass}}},
        ::jni::Method{"assertObjectArrayOfNulls1D", ::jni::Return{},
                      ::jni::Params{::jni::Array{kObjectTestHelperClass}}},

        // Rank 2 Verification.
        ::jni::Method{"assertBoolean2D", ::jni::Return{},
                      ::jni::Params{jboolean{}, ::jni::Array<jboolean, 2>{}}},
        ::jni::Method{"assertByte2D", ::jni::Return{},
                      ::jni::Params{jbyte{}, ::jni::Array<jbyte, 2>{}}},
        ::jni::Method{"assertChar2D", ::jni::Return{},
                      ::jni::Params{jchar{}, ::jni::Array<jchar, 2>{}}},
        ::jni::Method{"assertShort2D", ::jni::Return{},
                      ::jni::Params{jshort{}, ::jni::Array<jshort, 2>{}}},
        ::jni::Method{"assertInt2D", ::jni::Return{},
                      ::jni::Params{jint{}, ::jni::Array<jint, 2>{}}},
        ::jni::Method{"assertLong2D", ::jni::Return{},
                      ::jni::Params{jlong{}, ::jni::Array<jlong, 2>{}}},
        ::jni::Method{"assertFloat2D", ::jni::Return{},
                      ::jni::Params{jfloat{}, ::jni::Array<jfloat, 2>{}}},
        ::jni::Method{"assertDouble2D", ::jni::Return{},
                      ::jni::Params{jdouble{}, ::jni::Array<jdouble, 2>{}}},
        ::jni::Method{"assertObject2D", ::jni::Return{},
                      ::jni::Params{int{}, ::jni::Array{kObjectTestHelperClass,
                                                        ::jni::Rank<2>{}}}},
    }};

#endif  // JNI_BIND_JAVATESTS_COM_JNIBIND_TEST_ARRAY_TEST_HELPERS_NATIVE_H_
