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

#include <string_view>

#include "jni_bind.h"

namespace {

using ::jni::Class;
using ::jni::NameOrNothing_v;

static constexpr int kNoNameInt = 3;
static constexpr float kNoNameFloat = 4.f;
static_assert(NameOrNothing_v<kNoNameInt> == std::string_view{""});
static_assert(NameOrNothing_v<kNoNameFloat> == std::string_view{""});

static constexpr Class k_class_one{"classOne"};
static constexpr Class k_class_two{"classTwo"};
static_assert(NameOrNothing_v<k_class_one> == std::string_view{"classOne"});
static_assert(NameOrNothing_v<k_class_one> != std::string_view{"classTwo"});
static_assert(NameOrNothing_v<k_class_two> == std::string_view{"classTwo"});

}  // namespace
