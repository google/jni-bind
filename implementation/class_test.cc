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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "jni_bind.h"
#include "jni_test.h"

namespace {

using jni::Class;
using jni::test::JniTest;

static constexpr Class k_class_one{"classOne"};
static constexpr Class k_class_one_prime{"classOne"};
static constexpr Class k_class_two{"classTwo"};

static_assert(k_class_one == k_class_one);
static_assert(k_class_one == k_class_one_prime);
static_assert(!(k_class_two == k_class_one_prime));

}  // namespace
