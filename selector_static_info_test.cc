#include "selector_static_info.h"

#include "jni_bind.h"

using jni::Class;
using jni::InputParamSelection;
using jni::Method;
using jni::MethodSelection_t;
using jni::Overload;
using jni::Params;
using jni::ReturnSelection;
using jni::SelectorStaticInfo;

namespace {

static constexpr jni::Class kClass2{"kClass2"};

static constexpr jni::Class kClass{
    "kClass",
    Method{"Foo", Overload{jni::Return{int{}}, Params<>{}},
           Overload{jni::Return{float{}}, Params<>{}},
           Overload{jni::Return{Class{"kClass2"}}, Params<>{}},
           Overload{jni::Return{kClass2}, Params{jint{}, jfloat{}, kClass2}}}};

template <const auto& class_v, bool is_constructor, size_t method_idx>
using MethodSelection_t = MethodSelection_t<jni::kDefaultClassLoader, class_v,
                                            is_constructor, method_idx>;

using FooMethodT = MethodSelection_t<kClass, false, 0>;

// Helper for tests.
template <size_t overload_idx>
using FooOverload = jni::OverloadSelection<FooMethodT, overload_idx>;

////////////////////////////////////////////////////////////////////////////////
// Return signature tests.
////////////////////////////////////////////////////////////////////////////////
static_assert(SelectorStaticInfo<
                  ReturnSelection<FooMethodT, FooOverload<0>>>::kTypeName ==
              "I");
static_assert(SelectorStaticInfo<
                  ReturnSelection<FooMethodT, FooOverload<1>>>::kTypeName ==
              "F");
static_assert(SelectorStaticInfo<
                  ReturnSelection<FooMethodT, FooOverload<2>>>::kTypeName ==
              "LkClass2;");
static_assert(SelectorStaticInfo<
                  ReturnSelection<FooMethodT, FooOverload<3>>>::kTypeName ==
              "LkClass2;");

////////////////////////////////////////////////////////////////////////////////
// Param Index Signature Tests.
////////////////////////////////////////////////////////////////////////////////
static_assert(SelectorStaticInfo<InputParamSelection<FooMethodT, FooOverload<3>,
                                                     0>>::kTypeName == "I");
static_assert(SelectorStaticInfo<InputParamSelection<FooMethodT, FooOverload<3>,
                                                     1>>::kTypeName == "F");
static_assert(SelectorStaticInfo<InputParamSelection<FooMethodT, FooOverload<3>,
                                                     2>>::kTypeName ==
              "LkClass2;");

////////////////////////////////////////////////////////////////////////////////
// Overload Signature Generation.
////////////////////////////////////////////////////////////////////////////////
static_assert(FooOverload<0>::GetOverloadSignature() == "()I");
static_assert(FooOverload<1>::GetOverloadSignature() == "()F");
static_assert(FooOverload<2>::GetOverloadSignature() == "()LkClass2;");
static_assert(FooOverload<3>::GetOverloadSignature() ==
              "(IFLkClass2;)LkClass2;");

}  // namespace
