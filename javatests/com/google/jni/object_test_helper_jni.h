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

#ifndef JNI_BIND_JAVATESTS_COM_GOOGLE_AR_JNI_JNI_TEST_SECOND_OBJECT_JNI_H_
#define JNI_BIND_JAVATESTS_COM_GOOGLE_AR_JNI_JNI_TEST_SECOND_OBJECT_JNI_H_

#include "class.h"
#include "field.h"
#include "method.h"
#include "object.h"
#include "params.h"
#include "return.h"

// clang-format off
constexpr jni::Class kObjectClass {
    "java/lang/Object"
};

constexpr jni::Class kObjectTestHelperClass {
    "com/google/jni/ObjectTestHelper",

    jni::Constructor<>{},
    jni::Constructor{kObjectClass},
    jni::Constructor<int>{},
    jni::Constructor<int, int, int>{},

    jni::Method{"returnNewObjectWithFieldSetToSum",
      jni::Overload{
        jni::Return{jni::Class{"com/google/jni/ObjectTestHelper"}},
        jni::Params<int, int>{}
      },
      jni::Overload{
        jni::Return{jni::Class{"com/google/jni/ObjectTestHelper"}},
        jni::Params{jni::Class{"com/google/jni/ObjectTestHelper"}}
      },
    },

    jni::Field{"intVal1", int{}},
    jni::Field{"intVal2", int{}},
    jni::Field{"intVal3", int{}}
};
// clang-format on

#endif  // JNI_BIND_JAVATESTS_COM_GOOGLE_AR_JNI_JNI_TEST_SECOND_OBJECT_JNI_H_
