#include <string_view>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
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
