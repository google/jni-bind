#include "jni_bind.h"

using ::jni::Array;
using ::jni::Class;
using ::jni::InputParamSelection;
using ::jni::kIsReturnIdx;
using ::jni::Method;
using ::jni::MethodSelection_t;
using ::jni::Overload;
using ::jni::Params;
using ::jni::SelectorStaticInfo;

namespace {

static constexpr jni::Class kClass2{"kClass2"};

// This class is illegal (multiple signatures with differing return types),
// however it doesn't matter, we are just interested in signatures.
static constexpr jni::Class kClass{
    "kClass",
    Method{
        "Foo",
        // Primitive types.
        Overload{jni::Return{int{}}, Params<>{}},
        Overload{jni::Return{float{}}, Params<>{}},

        // Object in return signature.
        Overload{jni::Return{Class{"kClass2"}}, Params<>{}},
        Overload{jni::Return{kClass2}, Params{jint{}, jfloat{}, kClass2}},

        // Returns of arrays.
        Overload{jni::Return{Array{jint{}}}, Params<>{}},
        Overload{jni::Return{Array{Class{"kClass3"}}}, Params<>{}},

        // Arrays in args.
        Overload{jni::Return{}, Params{Array{int{}}}},
        Overload{jni::Return{Array{kClass2}},
                 Params{Array{int{}}, Array{kClass2}}},
    }};

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
static_assert(
    SelectorStaticInfo<
        InputParamSelection<FooOverload<0>, kIsReturnIdx>>::kTypeName == "I");
static_assert(
    SelectorStaticInfo<
        InputParamSelection<FooOverload<1>, kIsReturnIdx>>::kTypeName == "F");
static_assert(
    SelectorStaticInfo<
        InputParamSelection<FooOverload<2>, kIsReturnIdx>>::kTypeName ==
    "LkClass2;");
static_assert(
    SelectorStaticInfo<
        InputParamSelection<FooOverload<3>, kIsReturnIdx>>::kTypeName ==
    "LkClass2;");
static_assert(
    SelectorStaticInfo<
        InputParamSelection<FooOverload<4>, kIsReturnIdx>>::kTypeName == "[I");
static_assert(
    SelectorStaticInfo<
        InputParamSelection<FooOverload<5>, kIsReturnIdx>>::kTypeName ==
    "[LkClass3;");

////////////////////////////////////////////////////////////////////////////////
// Param Index Signature Tests.
////////////////////////////////////////////////////////////////////////////////
static_assert(
    SelectorStaticInfo<InputParamSelection<FooOverload<3>, 0>>::kTypeName ==
    "I");
static_assert(
    SelectorStaticInfo<InputParamSelection<FooOverload<3>, 1>>::kTypeName ==
    "F");
static_assert(
    SelectorStaticInfo<InputParamSelection<FooOverload<3>, 2>>::kTypeName ==
    "LkClass2;");

static_assert(
    SelectorStaticInfo<InputParamSelection<FooOverload<6>, 0>>::kTypeName ==
    "[I");

////////////////////////////////////////////////////////////////////////////////
// Overload Signature Generation.
////////////////////////////////////////////////////////////////////////////////
static_assert(FooOverload<0>::GetOverloadSignature() == "()I");
static_assert(FooOverload<1>::GetOverloadSignature() == "()F");
static_assert(FooOverload<2>::GetOverloadSignature() == "()LkClass2;");
static_assert(FooOverload<3>::GetOverloadSignature() ==
              "(IFLkClass2;)LkClass2;");

static_assert(FooOverload<6>::GetOverloadSignature() == "([I)V");
static_assert(FooOverload<7>::GetOverloadSignature() ==
              "([I[LkClass2;)[LkClass2;");

}  // namespace
