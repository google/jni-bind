// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "jni_type_proxy.h"

#include "class.h"
#include "global_object.h"
#include "jni_helper/jni_helper.h"
#include "jni_test.h"
#include "jni_type_proxy.h"
#include "local_object.h"
#include "mock_jni_env.h"
#include "object_ref.h"
#include <gtest/gtest.h>
#include "jni_dep.h"

namespace {

using jni::Class;
using jni::Method;
using jni::Params;
using jni::test::JniTest;

TEST_F(JniTest, JniTypeProxy_CallsInvokeForObjectBase) {
  static constexpr Class c1{
      "ARCore",
      Method{"Foo", jni::Return<jint>{}, Params<jint, jfloat>{}},
  };
  static constexpr Class c2{
      "ARCore",
      Method{"Foo", jni::Return{c1}, Params<jint, jfloat>{}},
  };

  jni::LocalObject<c2> local_object{};
  local_object("Foo", 1, 2.f);
}

}  // namespace
