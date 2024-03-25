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

/*******************************************************************************
 * JNI Bind Version __JNI_BIND_VERSION__.
 * Beta Public Release.
 ********************************************************************************
 * This header is the single header version which you can use to quickly test or
 * deploy in your own project without using Bazel. It is self contained.
 *
 * To use this header you must be compiling with clang and with C++17 support.
 *
 * It must also have jni.h in its build path. I.e. the following must compile:
 * #include <jni.h>
 *
 * See GitHub for sample API usage.
 * https://github.com/google/jni-bind
 *
 * 🚨 Are you enjoying JNI Bind? Please consider adding a ⭐️ on GitHub! 🚨
 * JNI Bind is the culmination of a lot of hard work and your support is greatly
 * appreciated.
 *
 * CODE BELOW IS AUTO GENERATED.
 *******************************************************************************/

#ifndef JNI_BIND_RELEASE_
#define JNI_BIND_RELEASE_

#include <jni.h>

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// Helper struct for figuring out the contents of a tuple.  Use Unwrap if you
// plan to use this as a metafunction (this breaks the paradigm as type is not
// templatized, but instead embedded in the type).
template <typename Func, typename TupleType>
struct TupleUnroller {
  static_assert(std::is_same_v<TupleType, void>,
                "You used a non tuple type when a tuple was expected.");
};

template <typename Func, typename... Ts>
struct TupleUnroller<Func, std::tuple<Ts...>> {
  // Templated to avoid invalid instantiations.
  template <typename = void>
  struct TypeHelperStruct {
    using type = typename Func::template type<Ts...>;
  };

  // Templated to avoid invalid instantiations.
  template <typename = void>
  struct ValueHelperStruct {
    static constexpr auto value = Func::template value<Ts...>;
  };
};

template <typename Func, typename TupleType>
using TupleUnroller_t =
    typename TupleUnroller<Func,
                           TupleType>::template TypeHelperStruct<void>::type;

template <typename Func, typename TupleType>
static constexpr auto TupleUnroller_v =
    TupleUnroller<Func, TupleType>::template ValueHelperStruct<void>::value;

// Takes a variadic pack and injects it inside another type.
// e.g. TupleToType<std::tuple<int, float>>::type<std::pair> =
//        std::pair<int, float>.
template <typename T>
struct TupleToType {
  static_assert(std::is_same_v<T, std::tuple<>>,
                "TupleToType should be templated on a std::tuple type");
  using type = std::tuple<>;
};

template <typename... Ts>
struct TupleToType<std::tuple<Ts...>> {
  template <template <typename...> class Container>
  using type = Container<Ts...>;
};

template <typename TupleType, template <typename...> class Container>
using TupleToType_t = typename TupleToType<TupleType>::template type<Container>;

// Takes a container and extracts its variadic arguments into a tuple.
template <typename T>
struct ExtractTupleFromType {
  static_assert(
      std::is_same_v<T, void>,
      "ExtractTupleFromType should be used on a type with variadic params.");
};

template <template <typename...> class ContainerType, typename... Ts>
struct ExtractTupleFromType<ContainerType<Ts...>> {
  using type = std::tuple<Ts...>;
};

template <typename ContainerType>
using ExtractTupleFromType_t =
    typename ExtractTupleFromType<ContainerType>::type;

}  // namespace jni::metaprogramming


namespace jni::metaprogramming {

// Metafunction to forward all args in a tuple on to a metafunction.
// It's useful if you want to "derefence" arguments from another meta function.
template <typename Func>
struct Unwrap {
  template <typename TupType>
  using type = TupleUnroller_t<Func, TupType>;
};

template <typename Func, typename... Ts>
using Unwrap_t = typename Unwrap<Func>::template type<Ts...>;

template <typename Func, typename... Ts>
static constexpr auto Unwrap_v = Unwrap_t<Func, Ts...>::value;

}  // namespace jni::metaprogramming


#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

template <std::size_t N>
struct TypeOfNthElement {
  template <std::size_t idx, typename... T>
  struct PackUnroller;

  template <typename T0, typename... T>
  struct PackUnroller<0, T0, T...> {
    using type = T0;
  };

  template <std::size_t idx, typename T0, typename... T>
  struct PackUnroller<idx, T0, T...> {
    using type = typename PackUnroller<idx - 1, T...>::type;
  };

  template <typename... Ts>
  using type = typename PackUnroller<N, Ts...>::type;
};

template <std::size_t N, typename... Ts>
using TypeOfNthElement_t = typename TypeOfNthElement<N>::template type<Ts...>;

template <std::size_t N, typename TupType>
using TypeOfNthTupleElement_t = Unwrap_t<TypeOfNthElement<N>, TupType>;

}  // namespace jni::metaprogramming


#include <tuple>

namespace jni::metaprogramming {

// Metafunction that takes variadic arguments and applies a bound metafunction
// to each element.  Type exposed will always be a tuple.  This is intended for
// use with a Monoid that you might want to apply across a set of every
// argument.  Otherwise, you would use the argument directly.
template <typename BoundType>
struct PerElement {
  template <typename Args, typename Is>
  struct Sequence {};

  template <typename... Args, std::size_t... Is>
  struct Sequence<std::tuple<Args...>, std::index_sequence<Is...>> {
    using type = std::tuple<
        typename BoundType::template type<TypeOfNthElement_t<Is, Args...>>...>;
  };

  template <typename... Ts>
  using type = typename Sequence<std::tuple<Ts...>,
                                 std::make_index_sequence<sizeof...(Ts)>>::type;
};

template <typename Func, typename... Ts>
using PerElement_t = typename PerElement<Func>::template type<Ts...>;

}  // namespace jni::metaprogramming


#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// Metafunction to take a sequence of values and emit std::true or std::false
// per element based on their type equality.
template <typename... Ts>
struct Same {
  template <typename... Us>
  using type =
      typename std::is_same<std::tuple<Ts...>, std::tuple<Us...>>::type;

  template <typename... Us>
  constexpr static bool value = type<Us...>::value;
};

}  // namespace jni::metaprogramming

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// "Invokes" a metafunction (i.e. a type alias for Func::type).
// Handles both variadic or non-variadic args for a consistent syntax.
template <typename Func>
struct Invoke {
  template <typename, typename Enable = void>
  struct InvokeHelper {};

  // Variadic forwarding.
  template <typename... Ts>
  struct InvokeHelper<std::tuple<Ts...>> {
    using type = typename Func::template type<Ts...>;
  };

  // Non-variadic forwarding.
  template <typename Ts>
  struct InvokeHelper<std::tuple<Ts>> {
    using type = typename Func::template type<Ts>;
  };

  template <typename... Args>
  using type = typename InvokeHelper<std::tuple<Args...>>::type;
};

// Helper alias to invoke a metafunction with args as a variadic pack.
template <typename Func, typename... Args>
using Invoke_t = typename Invoke<Func>::template type<Args...>;

// Helper alias to take a pack from a tuple and use it as args to invocation.
template <typename Func, typename TupleOfArgs>
using InvokeAsPack_t = TupleUnroller_t<Invoke<Func>, TupleOfArgs>;

// Helper alias to invoke a metafunction with args contained in a tuple. Each
// tuple element will itself be an arg.
template <typename Func, typename TupleOfArgs>
using InvokePerTupArg_t = TupleUnroller_t<PerElement<Func>, TupleOfArgs>;

// Helper alias to invoke a metafunction with args contained in each element of
// a tuple. Each tuple element will itself be a tuple of arguments.
template <typename Func, typename TupleOfTupleOfArgs>
using InvokePerTupArgAsPack_t =
    InvokePerTupArg_t<Unwrap<Func>, TupleOfTupleOfArgs>;

}  // namespace jni::metaprogramming

#include <tuple>

namespace jni::metaprogramming {

// Takes two tuples and collapses them into a single tuple.
struct Combine {
  template <typename T1, typename T2>
  struct CombineHelper {
    static_assert(std::is_same_v<T1, void> && std::is_same_v<T2, void>,
                  "Combine requires tuples.");
  };

  template <typename... T1s, typename... T2s>
  struct CombineHelper<std::tuple<T1s...>, std::tuple<T2s...>> {
    using type = std::tuple<T1s..., T2s...>;
  };

  template <typename Tup1, typename Tup2>
  using type = typename CombineHelper<Tup1, Tup2>::type;
};

template <typename Tup1, typename Tup2>
using Combine_t = typename Combine::template type<Tup1, Tup2>;

}  // namespace jni::metaprogramming


#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

template <typename Func>
struct Any {
  template <typename... Ts>
  using type =
      typename std::disjunction<typename Func::template type<Ts>...>::type;
};

template <typename Func, typename... Ts>
using Any_t = typename Any<Func>::template type<Ts...>;

template <typename Func, typename... Ts>
static constexpr bool Any_v = Any_t<Func, Ts...>::value;

template <typename Func, typename Ts>
using Any_Tup = TupleUnroller_t<Any<Func>, Ts>;

template <typename Func, typename Ts>
static constexpr bool Any_Tup_v = TupleUnroller_t<Any<Func>, Ts>::value;

}  // namespace jni::metaprogramming


#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// Metafunction for a left fold reduction of a sequence of types with an
// operation. Single element sequences return the input type with no use of
// Operation.
template <class Operation>
struct Reduce {
  template <typename... Ts>
  struct SequenceGenerator {};

  template <typename T1, typename T2>
  using Operation_t = typename Operation::template type<T1, T2>;

  template <typename... Ts>
  using SequenceGenerator_t = typename SequenceGenerator<Ts...>::type;

  // Used for single element sequence.
  template <typename T1, typename... Ts>
  struct SequenceGenerator<T1, Ts...> {
    using type = T1;
  };

  // Terminal element in sequence.
  template <typename T1, typename T2>
  struct SequenceGenerator<T1, T2> {
    using type = Operation_t<T1, T2>;
  };

  template <typename T1, typename T2, typename... Ts>
  struct SequenceGenerator<T1, T2, Ts...> {
    using type = SequenceGenerator_t<Operation_t<T1, T2>, Ts...>;
  };

  template <typename... Args>
  using type = typename SequenceGenerator<Args...>::type;
};

template <typename Operation, typename... Ts>
using Reduce_t = typename Reduce<Operation>::template type<Ts...>;

template <typename Operation, typename TupleOfTs>
using ReduceAsPack_t = TupleUnroller_t<Reduce<Operation>, TupleOfTs>;

template <typename Operation, typename TupleOfTs>
static constexpr auto ReduceAsPack_v =
    TupleUnroller_t<Reduce<Operation>, TupleOfTs>::val;

}  // namespace jni::metaprogramming


#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

template <typename SoughtType, typename... Ts>
using Contains_t = Invoke_t<Any<Same<SoughtType>>, Ts...>;

template <typename SoughtType, typename... Tup>
static constexpr bool Contains_v = Contains_t<SoughtType, Tup...>::value;

template <typename SoughtType, typename Tup>
using TupContains_t = TupleUnroller_t<Any<Same<SoughtType>>, Tup>;

template <typename SoughtType, typename Tup>
static constexpr bool TupContains_v = TupContains_t<SoughtType, Tup>::value;

// Constexpr value implementation.
template <typename SoughtType, typename... Ts>
constexpr bool ContainsValue(const SoughtType& sought_value, Ts&&... ts) {
  return ((sought_value == ts) || ...);
}

}  // namespace jni::metaprogramming

#include <string_view>
#include <type_traits>
#include <utility>

#define STR(x) []() { return x; }

namespace jni::metaprogramming {

template <typename Identifier>
using identifier_type = decltype(std::declval<Identifier>()());

constexpr std::size_t ConstexprStrlen(const char* str) {
  return str[0] == 0 ? 0 : ConstexprStrlen(str + 1) + 1;
}

struct StringAsTypeBase {};

// Represents a string by embedding a sequence of characters in a type.
template <char... chars>
struct StringAsType : StringAsTypeBase {
  static constexpr char static_chars[] = {chars..., 0};
  static constexpr std::string_view chars_as_sv = {static_chars,
                                                   sizeof...(chars)};
};

template <typename Identifier, std::size_t... I>
constexpr auto LambdaToStr(Identifier id, std::index_sequence<I...>) {
  return StringAsType<id()[I]...>{};
}

template <
    typename Identifier,
    std::enable_if_t<std::is_same_v<identifier_type<Identifier>, const char*>,
                     int> = 0>
constexpr auto LambdaToStr(Identifier id) {
  return LambdaToStr(id, std::make_index_sequence<ConstexprStrlen(id())>{});
}

template <typename NameLambda>
using LambdaStringToType = decltype(LambdaToStr(std::declval<NameLambda>()));

}  // namespace jni::metaprogramming

#include <tuple>

namespace jni::metaprogramming {

// Metafunction to generate a concatenation of variadic args.
struct Concatenate {
  template <typename... Ts>
  using type = std::tuple<Ts...>;
};

template <typename... Ts>
using Concatenate_t = typename Concatenate::template type<Ts...>;

// Metafunction to concatenate tuples into a single tuple.
template <typename... Ts>
struct ConcatenateTup {
  static_assert(sizeof...(Ts) != 0, "ConcatenateTup must only take tuples.");
};

template <typename... T1s>
struct ConcatenateTup<std::tuple<T1s...>> {
  using type = std::tuple<T1s...>;
};

template <typename... Tups>
using ConcatenateTup_t = Reduce_t<Combine, Tups...>;

}  // namespace jni::metaprogramming

#include <string>
#include <string_view>

namespace jni::metaprogramming {

struct Color {
  static constexpr std::string_view kNone{"\033[0m"};

  // --- Text Colors ---
  static constexpr std::string_view kBlack = "\033[0;30m";
  static constexpr std::string_view kBlackBold = "\033[1;30m";
  static constexpr std::string_view kRed = "\033[0;31m";
  static constexpr std::string_view kRedBold = "\033[1;31m";
  static constexpr std::string_view kGreen = "\033[0;32m";
  static constexpr std::string_view kGreenBold = "\033[1;32m";
  static constexpr std::string_view kYellow = "\033[0;33m";
  static constexpr std::string_view kYellowBold = "\033[1;33m";
  static constexpr std::string_view kBlue = "\033[0;34m";
  static constexpr std::string_view kBlueBold = "\033[1;34m";
  static constexpr std::string_view kPurple = "\033[0;35m";
  static constexpr std::string_view kPurpleBold = "\033[1;35m";
  static constexpr std::string_view kCyan = "\033[0;36m";
  static constexpr std::string_view kCyanBold = "\033[1;36m";
  static constexpr std::string_view kWhite = "\033[0;37m";
  static constexpr std::string_view kWhiteBold = "\033[1;37m";

  // --- Background Colors ---
  static constexpr std::string_view kBgBlack = "\033[40m";
  static constexpr std::string_view kBgBlackBold = "\033[100m";
  static constexpr std::string_view kBgRed = "\033[41m";
  static constexpr std::string_view kBgRedBold = "\033[101m";
  static constexpr std::string_view kBgGreen = "\033[42m";
  static constexpr std::string_view kBgGreenBold = "\033[102m";
  static constexpr std::string_view kBgYellow = "\033[43m";
  static constexpr std::string_view kBgYellowBold = "\033[103m";
  static constexpr std::string_view kBgBlue = "\033[44m";
  static constexpr std::string_view kBgBlueBold = "\033[104m";
  static constexpr std::string_view kBgPurple = "\033[45m";
  static constexpr std::string_view kBgPurpleBold = "\033[105m";
  static constexpr std::string_view kBgCyan = "\033[46m";
  static constexpr std::string_view kBgCyanBold = "\033[106m";
  static constexpr std::string_view kBgWhite = "\033[47m";
  static constexpr std::string_view kBgWhiteBold = "\033[107m";
};

// Colorize function for runtime manipulation of text. This is suitable when
// the string is unknowable until runtime.
inline std::string Colorize(std::string_view colour, std::string_view str,
                            bool colourize = true) {
  return !colourize ? std::string{str}
                    : std::string{colour} + std::string{str} +
                          std::string{Color::kNone};
}

}  // namespace jni::metaprogramming

#include <type_traits>

namespace jni::metaprogramming {

template <typename...>
struct AllUnique {
  static constexpr bool value = true;
  using type = std::bool_constant<value>;
};

template <typename T, typename... Ts>
struct AllUnique<T, Ts...> {
  static constexpr bool value =
      !Contains_v<T, Ts...> && AllUnique<Ts...>::value;
  using type = std::bool_constant<value>;
};

template <typename... Ts>
static constexpr bool AllUnique_v = AllUnique<Ts...>::value;

// Constexpr value implementation.
constexpr bool AllUniqueValues(...) { return true; }

template <typename T1, typename... Ts>
constexpr bool AllUniqueValues(const T1&& t1, const Ts&&... ts) {
  return (!ContainsValue(t1, ts...)) && AllUniqueValues(ts...);
}

}  // namespace jni::metaprogramming


namespace jni {

// Convenience struct for returning results from pinning array.
template <typename SpanType>
struct GetArrayElementsResult {
  SpanType* ptr_;
  jboolean is_copy;
};

}  // namespace jni

#include <sstream>
#include <string>

namespace jni {

// Converts a single argument into a std::string (useful for debugging).
template <typename T>
struct ArgStringify {
  static std::string Str(const T& val) {
    std::stringstream ss;
    ss << val;

    return ss.str();
  }
};

template <typename T>
struct ArgStringify<T*> {
  static std::string Str(const T* val) {
    if (val == nullptr) {
      return "nullptr";
    }

    std::stringstream ss;
    ss << val;

    return ss.str();
  }
};

// Helper function to select correct ArgString partial specialization.
template <typename T>
std::string ArgString(const T& val) {
  return ArgStringify<T>::Str(val);
}

////////////////////////////////////////////////////////////////////////////////
// Void.
////////////////////////////////////////////////////////////////////////////////
template <>
struct ArgStringify<void> {
  static std::string Str() { return ""; }
};

inline std::string ArgString() { return ArgStringify<void>::Str(); }

////////////////////////////////////////////////////////////////////////////////
// Bool.
////////////////////////////////////////////////////////////////////////////////
template <>
struct ArgStringify<bool> {
  static std::string Str(const bool val) { return val ? "TRUE" : "FALSE"; }
};

}  // namespace jni

namespace jni {

// Single type that be used as a value when expressing void.
struct Void {
  using Raw = void;
};

template <typename T>
struct VoidIfVoid {
  using type = T;
};

template <>
struct VoidIfVoid<Void> {
  using type = void;
};

template <typename T>
using VoidIfVoid_t = typename VoidIfVoid<T>::type;

}  // namespace jni

namespace jni {

struct Object {
  const char* name_;
  constexpr explicit Object(const char* name) : name_(name) {}
};

}  // namespace jni

using jclass = jclass;
using jthrowable = jthrowable;
using jstring = jstring;
using jarray = jarray;
using jbooleanArray = jbooleanArray;
using jbyteArray = jbyteArray;
using jcharArray = jcharArray;
using jshortArray = jshortArray;
using jintArray = jintArray;
using jlongArray = jlongArray;
using jfloatArray = jfloatArray;
using jdoubleArray = jdoubleArray;
using jobjectArray = jobjectArray;

using jweak = jweak;
using jvalue = jvalue;
using jfieldID = jfieldID;
using jmethodID = jmethodID;

using JavaVM = JavaVM;
using JNIEnv = JNIEnv;

// IWYU pragma: end_exports

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// Metafunction that binds a metafunction (e.g. Same), a query (e.g. "A"),  a
// variadic pack as arguments, and returns the idx of the query.  All members
// of the variadic pack must be unique.
//
// |Comparator| must express a static bool data member "value".
template <typename Comparator>
struct FindIdxOfVal {
  template <typename... Ts>
  struct FindIdxOfValHelper {
    static_assert(sizeof...(Ts) == -1, "The sought element is not in the set");
  };

  template <typename T, typename... Ts>
  struct FindIdxOfValHelper<T, Ts...> {
    template <std::size_t idx>
    static constexpr std::size_t GetIdxHelper() {
      if constexpr (Comparator::template value<T>) {
        return idx;
      } else {
        return FindIdxOfValHelper<Ts...>::template GetIdxHelper<idx - 1>();
      }
    }

    static constexpr std::size_t GetIdx() {
      return sizeof...(Ts) - GetIdxHelper<sizeof...(Ts)>();
    }
  };

  // Done once in a seperate struct for improved compilation time.
  template <typename... Ts>
  struct StaticAssertWrapper {
    static_assert(AllUnique_v<Ts...>,
                  "FindIdxOfVal only operates on unique sets.");
    static constexpr std::size_t value = FindIdxOfValHelper<Ts...>::GetIdx();
  };

  template <typename... Ts>
  static constexpr std::size_t value = StaticAssertWrapper<Ts...>::value;
};

template <typename Comparator, typename... Ts>
static constexpr std::size_t FindIdxOfValWithComparator_idx =
    FindIdxOfVal<Comparator>::template value<Ts...>;

// Convenience alias with default of std::is_same Comparator (typical use case).
template <typename Query, typename... Ts>
static constexpr std::size_t FindIdxOfVal_idx =
    FindIdxOfValWithComparator_idx<Same<Query>, Ts...>;

template <typename Comparator, typename TupType>
static constexpr std::size_t FindIdxOfValInTupWithComparator_idx =
    TupleUnroller_v<FindIdxOfVal<Comparator>, TupType>;

// Convenience alias with default of std::is_same Comparator (typical use case).
template <typename Query, typename TupType>
static constexpr std::size_t FindIdxOfValInTup_idx =
    FindIdxOfValInTupWithComparator_idx<Same<Query>, TupType>;

}  // namespace jni::metaprogramming


#include <tuple>

namespace jni::metaprogramming {

// Metafunction to return only even elements.
struct Even {
  template <typename... Ts>
  struct EvenHelper {};

  template <>
  struct EvenHelper<std::tuple<>> {
    using type = std::tuple<>;
  };

  template <typename... Ts>
  struct OddHelper {};

  template <>
  struct OddHelper<std::tuple<>> {
    using type = std::tuple<>;
  };

  template <typename T, typename... Ts>
  struct OddHelper<std::tuple<T, Ts...>> {
    using type = typename EvenHelper<std::tuple<Ts...>>::type;
  };

  template <typename T, typename... Ts>
  struct EvenHelper<std::tuple<T, Ts...>> {
    using type = ConcatenateTup_t<std::tuple<T>,
                                  typename OddHelper<std::tuple<Ts...>>::type>;
  };

  template <typename... Ts>
  using type = typename EvenHelper<std::tuple<Ts...>>::type;
};

template <typename... Ts>
using Even_t = typename Even::template type<Ts...>;

// Metafunction to return only odd elements.
struct Odd {
  template <typename... Ts>
  struct OddHelper {
    using type = std::tuple<>;
  };

  template <typename T, typename... Ts>
  struct OddHelper<std::tuple<T, Ts...>> {
    using type = Even_t<Ts...>;
  };

  template <typename... Ts>
  using type = typename OddHelper<std::tuple<Ts...>>::type;
};

template <typename... Ts>
using Odd_t = typename Odd::template type<Ts...>;

}  // namespace jni::metaprogramming

#include <cstddef>
#include <utility>

namespace jni {

// Called once, before any arguments are printed.
template <char... chars>
inline void PreTrace(metaprogramming::StringAsType<chars...> tag) {
  printf(
      "%s%s",
      metaprogramming::Colorize(metaprogramming::Color::kBlueBold,
                                tag.static_chars)
          .data(),
      metaprogramming::Colorize(metaprogramming::Color::kPurple, "(").data());
}

template <typename T1, typename T2>
struct ArgTrace {};

// Called per argument.
template <std::size_t... Is, char... chars>
struct ArgTrace<std::index_sequence<Is...>,
                metaprogramming::StringAsType<chars...>> {
  template <std::size_t I>
  struct Helper {
    static constexpr bool IsLastArgument() { return I == sizeof...(Is) - 1; }

    template <typename Arg>
    static inline void Do(const Arg& arg) {
      printf("%s", metaprogramming::Colorize(metaprogramming::Color::kCyan,
                                             ArgString(arg))
                       .c_str());
      if constexpr (!IsLastArgument()) {
        printf("%s",
               metaprogramming::Colorize(metaprogramming::Color::kYellow, ",")
                   .data());
      }
    }
  };

  template <typename... Args>
  static inline void Do(const Args&... args) {
    (Helper<Is>::Do(args), ...);
  }
};

// Called once, after all arguments.
template <char... chars>
inline void PostTrace(metaprogramming::StringAsType<chars...> tag) {
  printf(
      "%s\n",
      metaprogramming::Colorize(metaprogramming::Color::kPurple, ");").data());
}

template <char... chars, typename... Args>
inline void Trace(metaprogramming::StringAsType<chars...> tag,
                  const Args&... args) {
// WARNING: This define is temporary and will be replaced. This unblocks
// 1.0 release but will eventually be configurable at compile time.
#ifdef ENABLE_DEBUG_OUTPUT
  PreTrace(tag);
  ArgTrace<std::make_index_sequence<sizeof...(Args)>, decltype(tag)>::Do(
      args...);
  PostTrace(tag);
#endif
}

}  // namespace jni

namespace jni {

template <const auto& jvm_v_>
class JvmRef;
class ThreadGuard;

// This class represents a static accessor for a ::JNIEnv*.
//
// JNIEnv* is passed into each JNI function but cannot be safely cached because
// it is not safe to share across threads.  That said, it is safe to save across
// the same thread.
//
// Thread local accesses ought to be free because this header will only ever be
// used in JNI implementations and in a statically linked context:
//    http://david-grs.github.io/tls_performance_overhead_cost_linux/
//
// The contract requires that any new thread must have Jvm::ThreadInit
// called once on every new thread (single threaded apps do not need to).
//
// In a single threaded context if an object is wrapped or created, the JniEnv
// will be cached in a thread local static.  If a std::unique_ptr<GlobalObject>
// were to be moved onto a new thread no JNIEnv* would be cached.  You could
// cache the JNIEnv on every move constructor, but this would almost certainly
// result in unnecessary and excessive writes.
class JniEnv {
 public:
  static inline JNIEnv* GetEnv() { return env_; }

 protected:
  template <const auto& jvm_v_>
  friend class JvmRef;
  friend class ThreadGuard;

  static inline void SetEnv(JNIEnv* env) { env_ = env; }

  // This will always be set when a new object is created (see above).
  static inline thread_local JNIEnv* env_;
};

}  // namespace jni

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace jni {

template <typename T>
struct FakeImpl;

// Produces a fake value of type T with non-trivial default value.
// If modifier is not NONE, will add a modifier.
// Offset can be used to nudge off base value so fake values don't match.
template <typename T, typename OffsetT = int>
auto Fake(OffsetT offset = 0) {
  return reinterpret_cast<T>(FakeImpl<std::decay_t<T>>::Val(offset));
}

template <typename T>
struct FakeImpl<T*> {
  static T* Val(int offset) {
    if constexpr (std::is_pointer_v<T>) {
      return reinterpret_cast<T*>(FakeImpl<std::decay_t<T>>::Val(offset));
    } else {
      return reinterpret_cast<T*>(
          static_cast<intptr_t>(FakeImpl<std::decay_t<T>>::Val(offset)));
    }
  }
};

////////////////////////////////////////////////////////////////////////////////
// Non-JNI types.
////////////////////////////////////////////////////////////////////////////////
template <>
struct FakeImpl<std::size_t> {
  static std::size_t Val(std::size_t offset) { return std::size_t{123}; }
};

////////////////////////////////////////////////////////////////////////////////
// JNI C API constants.
////////////////////////////////////////////////////////////////////////////////
template <>
struct FakeImpl<JNIEnv*> {
  static JNIEnv* Val(int offset) {
    return reinterpret_cast<JNIEnv*>(0xFABCFABCFAB) + offset;
  }
};

template <>
struct FakeImpl<JNIEnv**> {
  static JNIEnv** Val(int offset) {
    return reinterpret_cast<JNIEnv**>(0xDABDABDABDA) + offset;
  }
};

template <>
struct FakeImpl<jclass> {
  static jclass Val(int offset) {
    return reinterpret_cast<jclass>(0xABCDEF12345 + offset);
  }
};

template <>
struct FakeImpl<jmethodID> {
  static jmethodID Val(int offset) {
    return reinterpret_cast<jmethodID>(0xBEBEBEBEBEBEBE + offset);
  }
};

template <>
struct FakeImpl<jfieldID> {
  static jfieldID Val(int offset) {
    return reinterpret_cast<jfieldID>(0xDEDEDEDEDEDEDE + offset);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Primitive constants.
////////////////////////////////////////////////////////////////////////////////
template <>
struct FakeImpl<jboolean> {
  static jboolean Val(int) { return false; }
};

template <>
struct FakeImpl<jbyte> {
  static jbyte Val(jbyte offset) { return jbyte{111} + offset; }
};

template <>
struct FakeImpl<jchar> {
  static jchar Val(jchar offset) { return jchar{222} + offset; }
};

template <>
struct FakeImpl<jshort> {
  static jshort Val(jshort offset) { return jshort{333} + offset; }
};

template <>
struct FakeImpl<jint> {
  static jint Val(jint offset) { return jint{444444444} + offset; }
};

template <>
struct FakeImpl<jlong> {
  static jlong Val(jlong offset) { return jlong{0x123456789ABCDE} + offset; }
};

template <>
struct FakeImpl<jfloat> {
  static jfloat Val(jfloat offset) { return jfloat{123.f} + offset; }
};

template <>
struct FakeImpl<jdouble> {
  static jdouble Val(jdouble offset) { return jdouble{77777777} + offset; }
};

template <>
struct FakeImpl<jobject> {
  static jobject Val(int offset) {
    return reinterpret_cast<jobject>(0xAAAAABBBBBB + offset);
  }
};

template <>
struct FakeImpl<jstring> {
  static jstring Val(int offset) {
    return reinterpret_cast<jstring>(0xEEEEFFFFF + offset);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Array constants.
////////////////////////////////////////////////////////////////////////////////
template <>
struct FakeImpl<jarray> {
  static jarray Val(int offset) {
    return reinterpret_cast<jarray>(0xCCCCCDDDDDD + offset);
  }
};

template <>
struct FakeImpl<jbooleanArray> {
  static jbooleanArray Val(int offset) {
    return reinterpret_cast<jbooleanArray>(0xEEEEEFFFFFFF + offset);
  }
};

template <>
struct FakeImpl<jbyteArray> {
  static jbyteArray Val(int offset) {
    return reinterpret_cast<jbyteArray>(0xAEAEAFBFBFBF + offset);
  }
};

template <>
struct FakeImpl<jcharArray> {
  static jcharArray Val(int offset) {
    return reinterpret_cast<jcharArray>(0xEFEFEF0A0A0A + offset);
  }
};

template <>
struct FakeImpl<jshortArray> {
  static jshortArray Val(int offset) {
    return reinterpret_cast<jshortArray>(0xBEEEEBFFFFFB + offset);
  }
};

template <>
struct FakeImpl<jintArray> {
  static jintArray Val(int offset) {
    return reinterpret_cast<jintArray>(0xEAABAAFAABAAF + offset);
  }
};

template <>
struct FakeImpl<jfloatArray> {
  static jfloatArray Val(int offset) {
    return reinterpret_cast<jfloatArray>(0xDAAAEDAAAAF + offset);
  }
};

template <>
struct FakeImpl<jlongArray> {
  static jlongArray Val(int offset) {
    return reinterpret_cast<jlongArray>(0xFAAFFBAAABFF + offset);
  }
};

template <>
struct FakeImpl<jdoubleArray> {
  static jdoubleArray Val(int offset) {
    return reinterpret_cast<jdoubleArray>(0xBEEEEEEEEEEB + offset);
  }
};

template <>
struct FakeImpl<jobjectArray> {
  static jobjectArray Val(int offset) {
    return reinterpret_cast<jobjectArray>(0xFECEEABCCCCF + offset);
  }
};

}  // namespace jni

#include <string_view>

namespace jni {

struct ReturnBase {};

template <typename Raw_>
struct Return : ReturnBase {
  const Raw_ raw_ = {};

  using Raw = Raw_;

  constexpr Return() = default;

  template <typename Raw>
  constexpr explicit Return(Raw raw) : raw_(raw) {}
};

template <>
struct Return<void> : ReturnBase {
  using Raw = void;
  const Void raw_{};

  constexpr Return() = default;
};

Return() -> Return<void>;

template <typename Raw>
Return(Raw) -> Return<Raw>;

template <typename T>
using Raw_t = typename T::Raw;

}  // namespace jni

#include <string>
#include <string_view>
#include <tuple>
#include <utility>

namespace jni {

struct ParamsBase {};

// Represents a sequence of parameters for a method in a compile time
// definition.
//
// These may be in their native type (e.g. int, float, etc) or they may have
// some minimal type wrapping such as Object<Name> (see JniTProxy).
template <typename... ValuesRaw_>
struct Params : ParamsBase {
 public:
  using ParamsRawTup = std::tuple<ValuesRaw_...>;

  const std::tuple<ValuesRaw_...> values_ = {};

  // Zero args constructor.
  template <std::size_t N = sizeof...(ValuesRaw_),
            typename std::enable_if<(N == 0), int>::type = 0>
  constexpr explicit Params() {}

  // CTAD constructor, fully defined args (e.g. Params{class_value}).
  template <
      std::size_t N = sizeof...(ValuesRaw_), typename... Args,
      typename std::enable_if<(N > 0 && sizeof...(Args) == N), int>::type = 0>
  constexpr explicit Params(Args... args) : values_(args...) {}

  // Constructor for explicit templatizing, no args (e.g. Params<jint>{}).
  template <
      std::size_t N = sizeof...(ValuesRaw_), typename... Args,
      typename std::enable_if<(N > 0 && sizeof...(Args) == 0), int>::type = 0>
  constexpr explicit Params() {}
};

template <typename... Ts>
Params(Ts...) -> Params<Ts...>;

template <typename... LhsValuesRaw, typename... RhsValuesRaw>
constexpr bool operator==(const Params<LhsValuesRaw...>& lhs,
                          const Params<RhsValuesRaw...>& rhs) {
  return lhs.values_ == rhs.values_;
}

//==============================================================================

template <typename T>
using ParamsRawTup_t = typename T::ParamsRawTup;

}  // namespace jni


#include <tuple>

namespace jni::metaprogramming {

// Maps types to other types.
// Keys do not need to be unique, although queries only return the first entry.
template <typename Tup1, typename Tup2>
struct TypeToTypeMap {};

template <typename... Keys_, typename... Values_>
struct TypeToTypeMap<std::tuple<Keys_...>, std::tuple<Values_...>> {
  static_assert(sizeof...(Keys_) == sizeof...(Values_),
                "Keys must be an equal size to the value.");

  using Keys = std::tuple<Keys_...>;
  using Values = std::tuple<Values_...>;
  using Invert = TypeToTypeMap<std::tuple<Values_...>, std::tuple<Keys_...>>;

  template <typename Comparator>
  using type =
      TypeOfNthElement_t<FindIdxOfValWithComparator_idx<Comparator, Keys_...>,
                         Values_...>;
};

template <typename TypeToTypeMap>
using TypeToTypeMap_Keys_t = typename TypeToTypeMap::Keys;

template <typename TypeToTypeMap>
using TypeToTypeMap_Values_t = typename TypeToTypeMap::Values;

// Metafunction to invert the map from keys->vals to vals->keys.
// Note, keys do not need to be unique.
template <typename TypeToTypeMap>
using TypeToTypeMap_Invert = typename TypeToTypeMap::Invert;

template <typename TypeToTypeMap, typename Query>
using TypeToTypeMapQuery_t = typename TypeToTypeMap::template type<Same<Query>>;

// Queries a type map using a custom comparator,
// If multiple keys satisfy, the first is returned.
template <typename TypeToTypeMap, typename Comparator>
using TypeToTypeMapQueryWithComparator_t =
    typename TypeToTypeMap::template type<Comparator>;

// Builds a TypeToTypeMap from interleaved Keys and Values.
template <typename... Ts>
using TypeToTypeMapFromKeyValues_t = TypeToTypeMap<Even_t<Ts...>, Odd_t<Ts...>>;

template <typename TupleOfKeyValuePairs>
using TypeToTypeMapFromKeyValuesTup_t =
    TupleToType_t<TupleOfKeyValuePairs, TypeToTypeMapFromKeyValues_t>;

}  // namespace jni::metaprogramming

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// Returns all elements derived from DesiredBase.
//
// Note, if no values found, this will be std::tuple<>.  If you'd like a default
// value use BaseFilterWithDefault.
template <typename DesiredBase, typename Enable = void,
          typename T = std::tuple<>>
struct BaseFilter {
  static_assert(std::is_same_v<T, std::tuple<>>);
  using type = std::tuple<>;
};

template <typename DesiredBase, typename... Ts>
using BaseFilter_t =
    typename BaseFilter<DesiredBase, void, std::tuple<Ts...>>::type;

template <typename DesiredBase, typename T, typename... Ts>
struct BaseFilter<DesiredBase,
                  std::enable_if_t<std::is_base_of_v<DesiredBase, T>, void>,
                  std::tuple<T, Ts...>> {
  using type =
      ConcatenateTup_t<std::tuple<T>, BaseFilter_t<DesiredBase, Ts...>>;
};

template <typename DesiredBase, typename T, typename... Ts>
struct BaseFilter<DesiredBase,
                  std::enable_if_t<!std::is_base_of_v<DesiredBase, T>, void>,
                  std::tuple<T, Ts...>> {
  using type = BaseFilter_t<DesiredBase, Ts...>;
};

//==============================================================================
template <typename PostFilterValue, typename DefaultValue>
using DefaultIfEmpty_Tup =
    std::conditional_t<std::is_same_v<PostFilterValue, std::tuple<>>,
                       DefaultValue, PostFilterValue>;

template <typename DesiredBase, typename DefaultValue, typename... Ts>
using BaseFilterWithDefault_t =
    DefaultIfEmpty_Tup<BaseFilter_t<DesiredBase, Ts...>,
                       std::tuple<DefaultValue>>;

}  // namespace jni::metaprogramming

#include <utility>

namespace jni {

// Implemented in "find_class_fallback.h".
extern jclass FindClassFallback(const char* class_name);

// Helper JNI shim for object, method, class, etc. lookup.
class JniHelper {
 public:
  // Finds a class with "name".  Note, the classloader used is whatever is
  // present on the stack when this is called.  No caching is performed,
  // returned `jclass` is a local.
  static jclass FindClass(const char* name);

  // Returns a local ref jclass for the given jobject.
  // Note, if the object is polymorphic it may be a sub or superclass.
  static jclass GetObjectClass(jobject object);

  // Gets a method for a signature (no caching is performed).
  static inline jmethodID GetMethodID(jclass clazz, const char* method_name,
                                      const char* method_signature);

  // Gets a static method for a signature (no caching is performed).
  static inline jmethodID GetStaticMethodID(jclass clazz,
                                            const char* method_name,
                                            const char* method_signature);

  // FieldIds follow the same semantics as MethodIds.
  static inline jfieldID GetFieldID(jclass clazz, const char* name,
                                    const char* signature);

  // Gets a static field for a signature (no caching is performed).
  static inline jfieldID GetStaticFieldID(jclass clazz, const char* field_name,
                                          const char* field_signature);

  // Strings.
  static const char* GetStringUTFChars(jstring str);

  static void ReleaseStringUTFChars(jstring str, const char* chars);
};

//==============================================================================

// This object shall be `null` or a global of a classloader (set by `JvmRef`).
// This is a hack for idiosyncracies with Android.
inline jobject& FallbackLoader() {
  static jobject loader{};

  return loader;
}

inline jclass JniHelper::FindClass(const char* name) {
  Trace(metaprogramming::LambdaToStr(STR("FindClass")), name);

#ifdef DRY_RUN
  return Fake<jclass>();
#else
  jclass jclass_from_thread_loader = jni::JniEnv::GetEnv()->FindClass(name);
  if (!jclass_from_thread_loader && FallbackLoader() != nullptr) {
    jni::JniEnv::GetEnv()->ExceptionClear();

    return FindClassFallback(name);
  }

  return jclass_from_thread_loader;
#endif  // DRY_RUN
}

inline jclass JniHelper::GetObjectClass(jobject object) {
  Trace(metaprogramming::LambdaToStr(STR("GetObjectClass")), object);

#ifdef DRY_RUN
  return Fake<jclass>();
#else
  return jni::JniEnv::GetEnv()->GetObjectClass(object);
#endif  // DRY_RUN
}

jmethodID JniHelper::GetMethodID(jclass clazz, const char* method_name,
                                 const char* method_signature) {
  Trace(metaprogramming::LambdaToStr(STR("GetMethodID")), clazz, method_name,
        method_signature);

#ifdef DRY_RUN
  return Fake<jmethodID>();
#else
  return jni::JniEnv::GetEnv()->GetMethodID(clazz, method_name,
                                            method_signature);
#endif  // DRY_RUN
}

jmethodID JniHelper::GetStaticMethodID(jclass clazz, const char* method_name,
                                       const char* method_signature) {
  Trace(metaprogramming::LambdaToStr(STR("GetStaticMethodID")), clazz,
        method_name, method_signature);

#ifdef DRY_RUN
  return Fake<jmethodID>();
#else
  return jni::JniEnv::GetEnv()->GetStaticMethodID(clazz, method_name,
                                                  method_signature);
#endif  // DRY_RUN
}

jfieldID JniHelper::GetFieldID(jclass clazz, const char* name,
                               const char* signature) {
  Trace(metaprogramming::LambdaToStr(STR("GetFieldID")), clazz, name,
        signature);

#ifdef DRY_RUN
  return Fake<jfieldID>();
#else
  return jni::JniEnv::GetEnv()->GetFieldID(clazz, name, signature);
#endif  // DRY_RUN
}

jfieldID JniHelper::GetStaticFieldID(jclass clazz, const char* name,
                                     const char* signature) {
  Trace(metaprogramming::LambdaToStr(STR("GetStaticFieldID")), clazz, name,
        signature);

#ifdef DRY_RUN
  return Fake<jfieldID>();
#else
  return jni::JniEnv::GetEnv()->GetStaticFieldID(clazz, name, signature);
#endif  // DRY_RUN
}

inline const char* JniHelper::GetStringUTFChars(jstring str) {
  Trace(metaprogramming::LambdaToStr(STR("GetStringUTFChars")), str);

#ifdef DRY_RUN
  return "DEAD_BEEF";
#else
  // If is_copy is an address of bool it will be set to true or false if a copy
  // is made.  That said, this seems to be of no consequence, as the API still
  // requires you to release the string at the end. There's no discernible
  // reason you would ever be able to meaningfully act differently based on
  // this parameter of the API (except to do the wrong thing).
  return jni::JniEnv::GetEnv()->GetStringUTFChars(str, /*isCopy=*/nullptr);
#endif  // DRY_RUN
}

inline void JniHelper::ReleaseStringUTFChars(jstring str, const char* chars) {
  Trace(metaprogramming::LambdaToStr(STR("ReleaseStringUTFChars")), str, chars);

#ifdef DRY_RUN
#else
  jni::JniEnv::GetEnv()->ReleaseStringUTFChars(str, chars);
#endif  // DRY_RUN
}

}  // namespace jni

#include <tuple>
#include <type_traits>

namespace jni {

struct OverloadBase {};
struct MethodBase {};

// Represents a single overload of a given method.
template <typename ReturnT_, typename Params_>
struct Overload : OverloadBase {
  const ReturnT_ return_;
  const Params_ params_;

  // `Return`, no `Params`.
  constexpr Overload(ReturnT_ return_type)
      : return_(return_type), params_(Params{}) {}

  // `Return` and `Params`.
  constexpr Overload(ReturnT_ return_type, Params_ params)
      : return_(return_type), params_(params) {}
};

template <typename ReturnT_>
Overload(ReturnT_) -> Overload<ReturnT_, Params<>>;

template <typename ReturnT_, typename Params_>
Overload(ReturnT_, Params_) -> Overload<ReturnT_, Params_>;

template <typename Returns_, typename Params_>
struct Method {};

template <typename... Returns, typename... Params_>
struct Method<std::tuple<Returns...>, std::tuple<Params_...>>
    : public MethodBase {
 public:
  const char* name_;
  const std::tuple<Overload<Returns, Params_>...> invocations_;

  // `Return`, no `Params`.
  template <typename ReturnT_,
            std::enable_if_t<std::is_base_of_v<ReturnBase, ReturnT_>, int> = 0>
  constexpr Method(const char* name, ReturnT_ return_type)
      : name_(name), invocations_(Overload{return_type}) {}

  // `Return` and `Params`.
  template <typename ReturnT_, typename ParamsT_,
            std::enable_if_t<std::is_base_of_v<ReturnBase, ReturnT_>, int> = 0>
  constexpr Method(const char* name, ReturnT_ return_type, ParamsT_ params)
      : name_(name), invocations_(Overload{return_type, params}) {}

  // `Overload` Set.
  constexpr Method(const char* name, Overload<Returns, Params_>... invocations)
      : name_(name), invocations_(invocations...) {}
};

// CTAD for Non-overloaded form, no Params.
template <typename ReturnT, typename = std::enable_if_t<
                                !std::is_base_of_v<OverloadBase, ReturnT>>>
Method(const char*,
       ReturnT) -> Method<std::tuple<ReturnT>, std::tuple<Params<>>>;

// CTAD for Non-overloaded form.
template <
    typename ReturnT, typename ParamsT,
    typename = std::enable_if_t<!std::is_base_of_v<OverloadBase, ReturnT> &&
                                !std::is_base_of_v<OverloadBase, ParamsT>>>
Method(const char*, ReturnT, ParamsT)
    -> Method<std::tuple<ReturnT>, std::tuple<ParamsT>>;

// CTAD for Overloaded form.
template <typename... Returns, typename... Params>
Method(const char*, Overload<Returns, Params>...)
    -> Method<std::tuple<Returns...>, std::tuple<Params...>>;

template <typename ReturnT1, typename ParamsT1, typename ReturnT2,
          typename ParamsT2>
constexpr bool operator==(const Method<ReturnT1, ParamsT1>& lhs,
                          const Method<ReturnT2, ParamsT2>& rhs) {
  return std::string_view(lhs.name_) == std::string_view(rhs.name_);
}

}  // namespace jni

#include <string>

namespace jni {

struct FieldBase {};

template <typename Raw_>
struct Field : public FieldBase {
 public:
  using Raw = Raw_;

  const char* name_;

  const Raw_ raw_ = {};

  constexpr Field(const char* name) : name_(name) {}
  constexpr Field(const char* name, Raw_ value_raw)
      : name_(name), raw_(value_raw) {}
};

template <typename Raw_>
Field(const char*, Raw_) -> Field<Raw_>;

template <typename T>
using Raw_t = typename T::Raw;

}  // namespace jni

#include <string_view>
#include <tuple>

namespace jni {

class StaticBase {};

template <typename Methods_, typename Fields_>
class Static;

template <typename... Methods_, typename... Fields_>
class Static<std::tuple<Methods_...>, std::tuple<Fields_...>>
    : public StaticBase {
 public:
  const std::tuple<Methods_...> methods_;
  const std::tuple<Fields_...> fields_;

  constexpr Static(Methods_... methods, Fields_... fields)
      : methods_(methods...), fields_(fields...) {}
};

Static()->Static<std::tuple<>, std::tuple<>>;

template <typename... Params>
Static(Params...)
    -> Static<metaprogramming::BaseFilter_t<MethodBase, Params...>,
              metaprogramming::BaseFilter_t<FieldBase, Params...>>;

}  // namespace jni

#include <cstddef>
#include <limits>

namespace jni {

static constexpr std::size_t kNoIdx{std::numeric_limits<std::size_t>::max()};

}  // namespace jni

namespace jni {

struct ConstructorBase {};

template <typename... ParamsRaw_>
class Constructor : public ConstructorBase {
 public:
  const Params<ParamsRaw_...> params_;

  // Zero args constructor.
  template <std::size_t N = sizeof...(ParamsRaw_),
            typename std::enable_if<(N == 0), int>::type = 0>
  constexpr explicit Constructor() {}

  // CTAD constructor, fully defined args (e.g. Params{class_value}).
  template <
      std::size_t N = sizeof...(ParamsRaw_), typename... Args,
      typename std::enable_if<(N > 0 && sizeof...(Args) == N), int>::type = 0>
  constexpr explicit Constructor(const Args... params_raw)
      : params_(params_raw...) {}

  // Constructor for explicit templatizing, no args (e.g. Params<jint>{}).
  template <
      std::size_t N = sizeof...(ParamsRaw_), typename... Args,
      typename std::enable_if<(N > 0 && sizeof...(Args) == 0), int>::type = 0>
  constexpr explicit Constructor(Args...) {}
};

template <typename... ParamsRaw>
Constructor(ParamsRaw...) -> Constructor<ParamsRaw...>;

template <typename... LhsParams, typename... RhsParams>
constexpr bool operator==(const Constructor<LhsParams...>& lhs,
                          const Constructor<RhsParams...>& rhs) {
  return lhs.params_ == rhs.params_;
}

//==============================================================================
// Represents a constructor used at runtime and has index data about where it
// exists in the static class definition which is embedded on the caller's
// behalf.  This is particular to constructors which exist as an overload set.
template <const auto& class_v_, size_t I>
class ConstructorRef : public ConstructorBase {
 public:
  static const auto& GetConstructor() {
    return std::get<I>(class_v_.constructors_);
  }

  static std::string* GetConstructorSignature() {
    static std::string* constructor_signature = new std::string{
        GetConstructor().params_.GetSignature() + std::string{"V"}};
    return constructor_signature;
  }

  static jmethodID GetConstructorMethodID(jclass class_ref) {
    // TODO(b/174287131): Store and release ID statically.
    return JniHelper::GetMethodID(class_ref, "<init>",
                                  GetConstructorSignature()->c_str());
  }
};

}  // namespace jni

namespace jni {

template <typename T>
struct ArrayTag {};

template <typename T>
static constexpr bool kIsArrayType =
    std::is_base_of_v<ArrayTag<jbyteArray>, T> ||
    std::is_base_of_v<ArrayTag<jcharArray>, T> ||
    std::is_base_of_v<ArrayTag<jshortArray>, T> ||
    std::is_base_of_v<ArrayTag<jintArray>, T> ||
    std::is_base_of_v<ArrayTag<jfloatArray>, T> ||
    std::is_base_of_v<ArrayTag<jdoubleArray>, T> ||
    std::is_base_of_v<ArrayTag<jlongArray>, T> ||
    std::is_base_of_v<ArrayTag<jbooleanArray>, T> ||
    std::is_base_of_v<ArrayTag<jobjectArray>, T> ||
    std::is_base_of_v<ArrayTag<jarray>, T>;

// Primitive Keys.
using PrimitiveKeys =
    std::tuple<jbyteArray, jcharArray, jshortArray, jintArray, jlongArray,
               jfloatArray, jdoubleArray, jbooleanArray>;

// Simple type for proxying types used in the API (e.g. jint) to their
// corresponding array type (e.g. jintarray). Only use the former type when
// using JNI Bind (e.g. LocalArray<jint>, not LocalArray<jintArray>).
using RegularToArrayTypeMap = metaprogramming::TypeToTypeMap<
    std::tuple<jbyte, jchar, jshort, jint, jlong, jfloat, jdouble, jboolean,
               jobject, jstring, jarray>,
    std::tuple<jbyteArray, jcharArray, jshortArray, jintArray, jlongArray,
               jfloatArray, jdoubleArray, jbooleanArray, jobjectArray,
               jobjectArray, jarray>>;

// Given a type, returns the corresponding array type (e.g. jint => jintArray).
template <typename T>
using RegularToArrayTypeMap_t =
    metaprogramming::TypeToTypeMapQuery_t<RegularToArrayTypeMap, T>;

// Array to CDecl type used for invocation.
// Defined separately since this map is not invertible (jobject, jstring =>
// jobject).
using ArrayToRegularTypeMap = metaprogramming::TypeToTypeMap<
    std::tuple<jbyteArray, jcharArray, jshortArray, jintArray, jlongArray,
               jfloatArray, jdoubleArray, jbooleanArray, jobjectArray, jarray>,
    std::tuple<jbyte, jchar, jshort, jint, jlong, jfloat, jdouble, jboolean,
               jobject, jarray>>;

template <typename T>
using ArrayToRegularTypeMap_t =
    metaprogramming::TypeToTypeMapQuery_t<ArrayToRegularTypeMap, T>;

////////////////////////////////////////////////////////////////////////////////
// Storage Helper Metafunction.
////////////////////////////////////////////////////////////////////////////////

// Figures out the underlying physical opaque handle used to store a type.
// e.g. A rank two int is a jobjectarray.
template <typename T, std::size_t kRank>
struct StorageHelper {
  using type = jobjectArray;
};

// HACK: jstring has its own type despite just being a jobject. To make the
// lookup tables above function, this is handled separately. This will hopefully
// be removed in the future.
template <>
struct StorageHelper<jstring, 1> {
  using type = jobjectArray;
};

template <typename T>
struct StorageHelper<T, 1> {
  using type = RegularToArrayTypeMap_t<T>;
};

template <typename T>
struct StorageHelper<T, 0> {
  using type = T;
};

template <typename T, std::size_t kRank>
using StorageHelper_t = typename StorageHelper<T, kRank>::type;

}  // namespace jni

#include <cstddef>
#include <initializer_list>
#include <limits>

namespace jni::metaprogramming {

constexpr std::size_t kNegativeOne = std::numeric_limits<std::size_t>::max();

// This function returns a std::size_t with a symbolic -1 that will not be
// included. This can be useful for building masks of indexes.
//
// When passed an empty set, this function returns kNegativeOne.
// When passed n elements, this function returns the maximum value, or
//   kNegativeOne if all elements are kNegativeOne.
constexpr std::size_t ModifiedMax(
    std::initializer_list<std::size_t> initializer_lister) {
  if (initializer_lister.size() == 0) {
    return kNegativeOne;
  }

  // If all values -1, return -1.
  bool non_trivial_value_found = false;
  for (const std::size_t val : initializer_lister) {
    non_trivial_value_found |= (val != kNegativeOne);
  }
  if (!non_trivial_value_found) {
    return kNegativeOne;
  }

  std::size_t max = 0;
  for (const std::size_t val : initializer_lister) {
    if (max < val && val != kNegativeOne) {
      max = val;
    }
  }

  return max;
}

}  // namespace jni::metaprogramming

#include <limits>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace jni {

static constexpr struct NoClass {
  const char* name_ = "__JNI_BIND__NO_CLASS__";
  const Static<std::tuple<>, std::tuple<>> static_{};
  const std::tuple<> methods_{};
  const std::tuple<> fields_{};

  constexpr bool operator==(const NoClass&) const { return true; }
  constexpr bool operator!=(const NoClass&) const { return true; }
} kNoClassSpecified;

template <typename Constructors_, typename Static_, typename Methods_,
          typename Fields_>
struct Class {};

template <typename... Constructors_, typename... StaticMethods_,
          typename... StaticFields_, typename... Methods_, typename... Fields_>
struct Class<std::tuple<Constructors_...>,
             std::tuple<Static<std::tuple<StaticMethods_...>,
                               std::tuple<StaticFields_...>>>,
             std::tuple<Methods_...>, std::tuple<Fields_...>> : public Object {
 public:
  const std::tuple<Constructors_...> constructors_;
  const Static<std::tuple<StaticMethods_...>, std::tuple<StaticFields_...>>
      static_;
  const std::tuple<Methods_...> methods_;
  const std::tuple<Fields_...> fields_;

  // Ctors + static.
  explicit constexpr Class(
      const char* class_name, Constructors_... constructors,
      Static<std::tuple<StaticMethods_...>, std::tuple<StaticFields_...>>
          statik,
      Methods_... methods, Fields_... fields)
      : Object(class_name),
        constructors_(constructors...),
        static_(statik),
        methods_(methods...),
        fields_(fields...) {}

  // No ctors, static.
  explicit constexpr Class(
      const char* class_name,
      Static<std::tuple<StaticMethods_...>, std::tuple<StaticFields_...>>
          statik,
      Methods_... methods, Fields_... fields)
      : Object(class_name),
        constructors_(Constructor<>{}),
        static_(statik),
        methods_(methods...),
        fields_(fields...) {}

  // Ctors, no static.
  explicit constexpr Class(const char* class_name,
                           Constructors_... constructors, Methods_... methods,
                           Fields_... fields)
      : Object(class_name),
        constructors_(constructors...),
        static_(Static{}),
        methods_(methods...),
        fields_(fields...) {}

  // No ctors, no static.
  explicit constexpr Class(const char* class_name, Methods_... methods,
                           Fields_... fields)
      : Class(class_name, Constructor<>{}, Static{}, methods..., fields...) {}

  template <typename... Params, typename... Constructors,
            typename... StaticMethods, typename... StaticFields,
            typename... Fields, typename... Methods>
  constexpr bool operator==(
      const Class<std::tuple<Constructors...>,
                  std::tuple<Static<std::tuple<StaticMethods...>,
                                    std::tuple<StaticFields...>>>,
                  std::tuple<Methods...>, std::tuple<Fields...>>& rhs) const {
    // Don't compare the other parameters so classes can be used as parameters
    // or return values before the class itself is defined.
    return std::string_view(name_) == std::string_view(rhs.name_);
  }

  constexpr bool operator==(const NoClass&) const { return false; }
  constexpr bool operator!=(const NoClass&) const { return false; }
};

template <typename... Params>
Class(const char*, Params...)
    -> Class<metaprogramming::BaseFilterWithDefault_t<ConstructorBase,
                                                      Constructor<>, Params...>,
             metaprogramming::BaseFilterWithDefault_t<
                 StaticBase, Static<std::tuple<>, std::tuple<>>, Params...>,
             metaprogramming::BaseFilter_t<MethodBase, Params...>,
             metaprogramming::BaseFilter_t<FieldBase, Params...>>;

Class(const char*) -> Class<std::tuple<Constructor<>>,
                            std::tuple<Static<std::tuple<>, std::tuple<>>>,
                            std::tuple<>, std::tuple<>>;

}  // namespace jni


#include <type_traits>

namespace jni {

template <typename RawType, std::size_t kRank = 1>
struct Array;

template <std::size_t kRank>
struct Rank {};

////////////////////////////////////////////////////////////////////////////////
// Array Non-Object Implementation.
////////////////////////////////////////////////////////////////////////////////
template <typename RawType, std::size_t kRank>
struct ArrayNonObjectTypeImpl {
  RawType raw_;

  constexpr ArrayNonObjectTypeImpl(RawType raw) : raw_(raw) {}

  constexpr ArrayNonObjectTypeImpl(RawType raw, Rank<kRank>) : raw_(raw) {}

  template <std::size_t kRank_, typename RawType_>
  constexpr ArrayNonObjectTypeImpl(Array<RawType, kRank>&& invalid_arg)
      : raw_(nullptr) {
    static_assert(std::is_same_v<RawType, void>,
                  "JNI Error: Invalid array declaration, use Array { type{}, "
                  "Rank<kRank>{} }.");
  }

  template <typename RawTypeRhs, std::size_t kRankRhs>
  constexpr bool operator==(const Array<RawTypeRhs, kRankRhs>& rhs) const {
    if constexpr (std::is_same_v<RawType, RawTypeRhs>) {
      return (raw_ == rhs.raw_);
    }
    return false;
  }

  template <typename RawTypeRhs, std::size_t kRankRhs>
  constexpr bool operator!=(const Array<RawTypeRhs, kRankRhs>& rhs) const {
    return !(*this == rhs);
  }
};

// Primitive array implementaiton.
template <typename T, std::size_t kRank, bool HoldsObject>
struct ArrayImpl : public ArrayNonObjectTypeImpl<T, kRank>,
                   ArrayTag<RegularToArrayTypeMap_t<T>> {
 public:
  using ArrayNonObjectTypeImpl<T, kRank>::ArrayNonObjectTypeImpl;
};

////////////////////////////////////////////////////////////////////////////////
// Array Object Implementation.
////////////////////////////////////////////////////////////////////////////////
template <typename RawType, std::size_t kRank_>
struct ArrayImpl<RawType, kRank_, true> : public ArrayTag<jobjectArray> {
  RawType raw_;

  constexpr ArrayImpl(RawType raw) : raw_(raw) {}

  template <std::size_t kRank>
  constexpr ArrayImpl(RawType raw, Rank<kRank>) : raw_(raw) {}

  template <typename RawTypeRhs, std::size_t kRank>
  constexpr bool operator==(const Array<RawTypeRhs, kRank>& rhs) const {
    if constexpr (std::is_same_v<RawType, RawTypeRhs>) {
      return (raw_ == rhs.raw_);
    }
    return false;
  }

  template <typename RawTypeRhs, std::size_t kRank>
  constexpr bool operator!=(const Array<RawTypeRhs, kRank>& rhs) const {
    return !(*this == rhs);
  }
};

// This type correlates to those used in declarations,
//   e.g. Field { Array { Array { jint {} } } }.
template <typename RawType, std::size_t kRank_>
struct Array
    : public ArrayImpl<RawType, kRank_, std::is_base_of_v<Object, RawType>> {
  constexpr Array()
      : ArrayImpl<RawType, kRank_, std::is_base_of_v<Object, RawType>>(
            RawType{}) {}

  constexpr Array(RawType raw)
      : ArrayImpl<RawType, kRank_, std::is_base_of_v<Object, RawType>>(raw) {}

  template <std::size_t kRank>
  constexpr Array(RawType raw, Rank<kRank>)
      : ArrayImpl<RawType, kRank, std::is_base_of_v<Object, RawType>>(raw) {}
};

template <typename RawType>
Array(RawType) -> Array<RawType, 1>;

template <typename RawType, std::size_t kRank>
Array(RawType, Rank<kRank>) -> Array<RawType, kRank>;

////////////////////////////////////////////////////////////////////////////////
// Rank Utilities.
////////////////////////////////////////////////////////////////////////////////
struct Rankifier {
  template <typename SpanType_>
  struct Helper;

  template <typename SpanType_, std::size_t kRank_>
  struct Helper<Array<SpanType_, kRank_>> {
    static constexpr std::size_t kRank = kRank_;
  };

  template <typename ArrayT>
  static inline constexpr std::size_t Rank(const ArrayT& maybe_array) {
    if constexpr (kIsArrayType<std::decay_t<decltype(maybe_array)>>) {
      return Helper<ArrayT>::kRank;
    } else {
      return 0;
    }
  }
};

////////////////////////////////////////////////////////////////////////////////
// Strip Utilities.
// Takes an native array like type, and emits the innermost type.
// e.g. {Array<int>, Array{Rank<2>{}, int{}, Array{Rank<3>{}, int{}} } => int.
////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct ArrayStrip {
  using type = T;
};

template <typename T>
using ArrayStrip_t = typename ArrayStrip<T>::type;

template <typename T, std::size_t kRank>
struct ArrayStrip<Array<T, kRank>> {
  using type = T;
};

template <typename T>
constexpr auto FullArrayStripV(const T& val) {
  if constexpr (kIsArrayType<std::decay_t<decltype(val)>>) {
    return FullArrayStripV(val.raw_);
  } else {
    return val;
  }
}

}  // namespace jni


#include <tuple>

namespace jni {

// The set of classes that a ClassLoader can load.
template <typename... Classes_>
class SupportedClassSet {
 public:
  const std::tuple<Classes_...> supported_classes_;

  constexpr SupportedClassSet(Classes_... supported_classes)
      : supported_classes_(supported_classes...) {
    // TODO(b/143908983): Classloaders should enforce unique classes.
    // static_assert(metaprogramming::AllUniqueValues(supported_classes...),
    //"All classes supported by the class loader must be unique.");
  }
};

template <typename... Classes>
SupportedClassSet(Classes...) -> SupportedClassSet<Classes...>;

}  // namespace jni


#include <tuple>

namespace jni {

static constexpr std::size_t kDefaultClassLoaderIdx = 0;
static constexpr std::size_t kClassNotInLoaderSetIdx =
    metaprogramming::kNegativeOne;

// Class loader that can supply any class (to be used when none is specified).
// Setting this as the root loader for user defined classes will disable checks
// that classes are explicitly listed under a loader's class list.
class DefaultClassLoader {
 public:
  const char* name_ = "__JNI_BIND_DEFAULT_CLASS_LOADER__";
  std::tuple<> supported_classes_{};

  // Note, this will return true iff ignore_default_loader is true, but the
  // corresponding IdxOfAncestor will be kClassNotInLoaderSetIdx.
  template <const auto&, bool ignore_default_loader = false>
  constexpr bool SupportedDirectlyOrIndirectly() const {
    if constexpr (ignore_default_loader) {
      // Note, it's an implementation detail that kDefaultClassLoader returns
      // false for |SupportedByThisSet|.  Otherwise, all classes will always
      // defer to the default loader.  See |ParentLoaderForClass|.
      return false;
    } else {
      return true;
    }
  }

  template <const auto& class_v>
  constexpr std::size_t IdxOfClass() const {
    return kClassNotInLoaderSetIdx;
  }

  template <const auto& possibly_supported_class>
  constexpr std::size_t IdxOfAncestor(std::size_t cur_idx = 0) const {
    return kClassNotInLoaderSetIdx;
  }

  template <typename T>
  bool constexpr operator==(const T& rhs) const {
    return false;
  }
  bool constexpr operator==(const DefaultClassLoader&) const { return true; }

  template <typename T>
  bool constexpr operator!=(const T& rhs) const {
    return !(*this == rhs);
  }
};

// Class loader that cannot supply any classes. This should be the root loader
// for most user defined classes.
class NullClassLoader {
 public:
  const char* name_ = "__JNI_BIND_NULL_CLASS_LOADER__";

  template <const auto&, bool ignore_default_loader = false>
  constexpr bool SupportedDirectlyOrIndirectly() const {
    return false;
  }

  template <const auto& class_v>
  constexpr std::size_t IdxOfClass() const {
    return kNoIdx;
  }

  template <const auto& possibly_supported_class>
  constexpr std::size_t IdxOfAncestor(std::size_t cur_idx = 0) const {
    return kClassNotInLoaderSetIdx;
  }

  template <typename T>
  bool constexpr operator==(const T& rhs) const {
    return false;
  }
  bool constexpr operator==(const NullClassLoader&) const { return true; }

  template <typename T>
  bool constexpr operator!=(const T& rhs) const {
    return !(*this == rhs);
  }
};

static constexpr NullClassLoader kNullClassLoader;
static constexpr DefaultClassLoader kDefaultClassLoader;

// DO NOT USE: This obviates a compiler bug for value based enablement on ctor.
static constexpr auto kShadowNullClassLoader = kNullClassLoader;

// DO NOT USE: This obviates a compiler bug for value based enablement on ctor.
static constexpr auto kShadowDefaultClassLoader = kDefaultClassLoader;

}  // namespace jni


namespace jni {

// clang-format off

inline constexpr Class kJavaLangClass{
  "java/lang/Class",
  Method{"getClassLoader", Return{ Class { "java/lang/ClassLoader" } }},
};

inline constexpr Class kJavaLangObject{
  "java/lang/Object",
  Method{"getClass", Return{kJavaLangClass}},
};

inline constexpr Class kJavaLangClassLoader{
  "java/lang/ClassLoader",
  Method{"loadClass", Return{kJavaLangClass}, Params<jstring>{}},
  Method{"toString", Return{jstring{}}, Params<>{}},
};

static constexpr Class kJavaLangString{
  "java/lang/String",

  Constructor{jstring{}},
  Constructor{Array{jbyte{}}},

  Method{"toString", Return{jstring{}}, Params<>{}},
};
// clang-format on

}  // namespace jni

#include <array>
#include <string_view>

namespace jni::metaprogramming {

struct StringConcatenate {
  template <std::string_view const&... Vs>
  struct Helper {
    static constexpr auto BuildConcatenation() noexcept {
      constexpr std::size_t len = (Vs.size() + ... + 0);
      std::array<char, len + 1> arr{};
      auto append_single_string =
          [i = 0, &arr](auto const& string_to_concatenate) mutable {
            for (auto c : string_to_concatenate) arr[i++] = c;
          };
      (append_single_string(Vs), ...);
      arr[len] = 0;

      return arr;
    }

    static constexpr auto arr = BuildConcatenation();
    static constexpr std::string_view value{arr.data(), arr.size() - 1};
  };

  template <std::string_view const&... Vs>
  static constexpr std::string_view value = Helper<Vs...>::value;
};

template <std::string_view const&... Vs>
static constexpr auto StringConcatenate_v = StringConcatenate::value<Vs...>;

}  // namespace jni::metaprogramming

#include <utility>

namespace jni {

enum class LifecycleType {
  LOCAL,
  GLOBAL,
  // WEAK, // not implemented yet.
};

template <typename Span, LifecycleType lifecycle_type>
struct LifecycleHelper;

// Shared implementation for local jobjects (jobject, jstring).
template <typename Span>
struct LifecycleLocalBase {
  static inline void Delete(Span object) {
    Trace(metaprogramming::LambdaToStr(STR("DeleteLocalRef")), object);

#ifdef DRY_RUN
#else
    JniEnv::GetEnv()->DeleteLocalRef(object);
#endif  // DRY_RUN
  }

  static inline Span NewReference(Span object) {
    Trace(metaprogramming::LambdaToStr(STR("NewLocalRef")), object);

#ifdef DRY_RUN
    return Fake<Span>();
#else
    return static_cast<Span>(JniEnv::GetEnv()->NewLocalRef(object));
#endif  // DRY_RUN
  }
};

template <typename Span>
struct LifecycleHelper<Span, LifecycleType::LOCAL>
    : public LifecycleLocalBase<Span> {
  using Base = LifecycleLocalBase<Span>;
  using Base::Base;
};

// Shared implementation for global jobjects (jobject, jstring).
template <typename Span>
struct LifecycleGlobalBase {
  static inline Span Promote(Span object) {
    Trace(metaprogramming::LambdaToStr(STR("NewGlobalRef")), object);

#ifdef DRY_RUN
    jobject ret = Fake<jobject>();
#else
    jobject ret = JniEnv::GetEnv()->NewGlobalRef(object);
#endif  // DRY_RUN

    Trace(metaprogramming::LambdaToStr(STR("DeleteLocalRef")), object);

#ifdef DRY_RUN
#else
    JniEnv::GetEnv()->DeleteLocalRef(object);
#endif  // DRY_RUN

    return static_cast<Span>(ret);
  }

  static inline void Delete(Span object) {
    Trace(metaprogramming::LambdaToStr(STR("DeleteGlobalRef")), object);

#ifdef DRY_RUN
#else
    JniEnv::GetEnv()->DeleteGlobalRef(object);
#endif  // DRY_RUN
  }

  static inline Span NewReference(Span object) {
    Trace(metaprogramming::LambdaToStr(STR("NewGlobalRef")), object);

#ifdef DRY_RUN
    return Fake<Span>();
#else
    return static_cast<Span>(JniEnv::GetEnv()->NewGlobalRef(object));
#endif  // DRY_RUN
  }
};

template <typename Span>
struct LifecycleHelper<Span, LifecycleType::GLOBAL>
    : public LifecycleLocalBase<Span> {
  using Base = LifecycleGlobalBase<Span>;
  using Base::Base;
};

}  // namespace jni


#include <tuple>
#include <type_traits>
#include <utility>

namespace jni {

inline constexpr struct NoClassLoader {
} kNoClassLoaderSpecified;

// This is just the list of classes we expect to be loadable from a class loader
// and its parent loader.
//
// Classes from different loaders are typically incompatible, but Class loaders
// delegate classes that they cannot directly load to their parent loaders, so
// classes attached to two different class loaders will still be compatible if
// they were loaded by a shared parent loader.
//
// To annotate a class in a function or field declaration, use `LoadedBy`.
template <typename ParentLoader_, typename... SupportedClasses_>
class ClassLoader : public Object {
 public:
  const ParentLoader_ parent_loader_;
  const std::tuple<SupportedClasses_...> supported_classes_;

  explicit constexpr ClassLoader(const char* class_loader_name)
      : Object(class_loader_name) {}

  explicit constexpr ClassLoader(
      ParentLoader_ parent_loader,
      SupportedClassSet<SupportedClasses_...> supported_class_set)
      : Object("__JNI_NO_NAME_GIVEN__"),
        parent_loader_(parent_loader),
        supported_classes_(supported_class_set.supported_classes_) {}

  // TODO(b/143908983): Loaders should not be able to supply classes that their
  //  parents do.
  explicit constexpr ClassLoader(
      const char* class_loader_name, ParentLoader_ parent_loader,
      SupportedClassSet<SupportedClasses_...> supported_class_set)

      : Object(class_loader_name),
        parent_loader_(parent_loader),
        supported_classes_(supported_class_set.supported_classes_) {}

  bool constexpr operator==(
      const ClassLoader<ParentLoader_, SupportedClasses_...>& rhs) const {
    return (*this).parent_loader_ == rhs.parent_loader_ &&
           (*this).supported_classes_ == rhs.supported_classes_;
  }
  template <typename T>
  bool constexpr operator==(const T& rhs) const {
    return false;
  }
  template <typename T>
  bool constexpr operator!=(const T& rhs) const {
    return !(*this == rhs);
  }

  template <const auto& class_v, std::size_t... Is>
  constexpr std::size_t IdxOfClassHelper(
      std::integer_sequence<std::size_t, Is...>) const {
    // std::max appears to be missing the initializer list overload in Bazel's
    // implementation of clang.  This should simply be std::max.
    return metaprogramming::ModifiedMax(
        {((std::get<Is>(supported_classes_) == class_v)
              ? std::size_t{Is}
              : metaprogramming::kNegativeOne)...,
         metaprogramming::kNegativeOne});
  }

  // Returns the index for a given class within this set (any given class ref is
  // defined by this index).
  template <const auto& class_v>
  constexpr std::size_t IdxOfClass() const {
    return IdxOfClassHelper<class_v>(
        std::make_integer_sequence<std::size_t,
                                   sizeof...(SupportedClasses_)>());
  }

  // Tests if a class is supported by this class loader (not ancestors).
  template <const auto& possibly_supported_class>
  constexpr bool SupportedByThisSet() const {
    return IdxOfClass<possibly_supported_class>() != kClassNotInLoaderSetIdx;
  }

  // Tests if a class is supported by this class loader (ancestors included).
  template <const auto& possibly_supported_class,
            bool ignore_default_loader = false>
  constexpr bool SupportedDirectlyOrIndirectly() const {
    return parent_loader_.template SupportedDirectlyOrIndirectly<
               possibly_supported_class>() ||
           SupportedByThisSet<possibly_supported_class>();
  }

  // Returns the index for parent-most ancestor that supports a given class.
  template <const auto& possibly_supported_class>
  constexpr std::size_t IdxOfAncestor(std::size_t cur_idx = 0) const {
    if (SupportedByThisSet<possibly_supported_class>() &&
        !parent_loader_.template SupportedDirectlyOrIndirectly<
            possibly_supported_class, true>()) {
      return cur_idx;
    }
    return parent_loader_.template IdxOfAncestor<possibly_supported_class>(
        cur_idx + 1);
  }
};

// Note: Null is chosen, not default, because LoadedBy requires a syntax like
// LoadedBy{ClassLoader{"kClass"}} (using the CTAD loader type below), but
// we want to prevent explicit usage of a default loader (as it makes no sense).
ClassLoader(const char*)
    -> ClassLoader<std::decay_t<decltype(kNullClassLoader)>>;

template <typename ParentLoader_, typename... SupportedClasses_>
ClassLoader(ParentLoader_ parent_loader,
            SupportedClassSet<SupportedClasses_...> supported_classes)
    -> ClassLoader<ParentLoader_, SupportedClasses_...>;

template <typename... SupportedClasses_>
ClassLoader(SupportedClassSet<SupportedClasses_...>)
    -> ClassLoader<DefaultClassLoader, SupportedClasses_...>;

template <typename T, std::size_t I>
constexpr auto& GetAncestor(const T& loader) {
  if constexpr (I == 0) {
    return loader;
  } else {
    return GetAncestor<decltype(loader.parent_loader_), I - 1>(
        loader.parent_loader_);
  }
}

template <const auto& loader, const auto& possibly_supported_class>
constexpr const auto& ParentLoaderForClass() {
  if constexpr (!loader.template SupportedDirectlyOrIndirectly<
                    possibly_supported_class>()) {
    return kNullClassLoader;
  } else if constexpr (loader.template IdxOfAncestor<
                           possibly_supported_class>() !=
                       kClassNotInLoaderSetIdx) {
    return GetAncestor<
        decltype(loader),
        loader.template IdxOfAncestor<possibly_supported_class>()>(loader);
  } else {
    return kDefaultClassLoader;
  }
}

}  // namespace jni


namespace jni::metaprogramming {

template <std::size_t repeat_cnt>
struct RepeatString {
  template <const std::string_view& val_to_repeat>
  static constexpr std::string_view val = metaprogramming::StringConcatenate_v<
      val_to_repeat, RepeatString<repeat_cnt - 1>::template val<val_to_repeat>>;
};

template <>
struct RepeatString<0> {
  template <const std::string_view& val_to_repeat>
  static constexpr std::string_view val{""};
};

template <std::size_t repeat_cnt, const std::string_view& val_to_repeat>
static constexpr auto RepeatString_v{
    RepeatString<repeat_cnt>::template val<val_to_repeat>};

}  // namespace jni::metaprogramming

#include <string_view>

namespace jni::metaprogramming {

struct Constants {
  static constexpr std::string_view new_line = "\n";

  static constexpr std::string_view left_bracket = "[";
  static constexpr std::string_view right_bracket = "]";

  static constexpr std::string_view left_parenthesis = "(";
  static constexpr std::string_view right_parenthesis = ")";

  static constexpr std::string_view hash = "#";
  static constexpr std::string_view comma = ",";
  static constexpr std::string_view semi_colon = ";";

  static constexpr std::string_view a = "a";
  static constexpr std::string_view b = "b";
  static constexpr std::string_view c = "c";
  static constexpr std::string_view d = "d";
  static constexpr std::string_view e = "e";
  static constexpr std::string_view f = "f";
  static constexpr std::string_view g = "g";
  static constexpr std::string_view h = "h";
  static constexpr std::string_view i = "i";
  static constexpr std::string_view j = "j";
  static constexpr std::string_view k = "k";
  static constexpr std::string_view l = "l";
  static constexpr std::string_view m = "m";
  static constexpr std::string_view n = "n";
  static constexpr std::string_view o = "o";
  static constexpr std::string_view p = "p";
  static constexpr std::string_view q = "q";
  static constexpr std::string_view r = "r";
  static constexpr std::string_view s = "s";
  static constexpr std::string_view t = "t";
  static constexpr std::string_view u = "u";
  static constexpr std::string_view v = "v";
  static constexpr std::string_view w = "w";
  static constexpr std::string_view x = "x";
  static constexpr std::string_view y = "y";
  static constexpr std::string_view z = "z";

  static constexpr std::string_view A = "A";
  static constexpr std::string_view B = "B";
  static constexpr std::string_view C = "C";
  static constexpr std::string_view D = "D";
  static constexpr std::string_view E = "E";
  static constexpr std::string_view F = "F";
  static constexpr std::string_view G = "G";
  static constexpr std::string_view H = "H";
  static constexpr std::string_view I = "I";
  static constexpr std::string_view J = "J";
  static constexpr std::string_view K = "K";
  static constexpr std::string_view L = "L";
  static constexpr std::string_view M = "M";
  static constexpr std::string_view N = "N";
  static constexpr std::string_view O = "O";
  static constexpr std::string_view P = "P";
  static constexpr std::string_view Q = "Q";
  static constexpr std::string_view R = "R";
  static constexpr std::string_view S = "S";
  static constexpr std::string_view T = "T";
  static constexpr std::string_view U = "U";
  static constexpr std::string_view V = "V";
  static constexpr std::string_view W = "W";
  static constexpr std::string_view X = "X";
  static constexpr std::string_view Y = "Y";
  static constexpr std::string_view Z = "Z";
};

}  // namespace jni::metaprogramming


namespace jni {

// jobject.
template <>
struct LifecycleHelper<jobject, LifecycleType::LOCAL>
    : public LifecycleLocalBase<jobject> {
  template <typename... CtorArgs>
  static inline jobject Construct(jclass clazz, jmethodID ctor_method,
                                  CtorArgs&&... ctor_args) {
    Trace(metaprogramming::LambdaToStr(STR("NewObject")), clazz, ctor_method,
          ctor_args...);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return JniEnv::GetEnv()->NewObject(clazz, ctor_method, ctor_args...);
#endif  // DRY_RUN
  }
};

template <>
struct LifecycleHelper<jobject, LifecycleType::GLOBAL>
    : public LifecycleGlobalBase<jobject> {
  template <typename... CtorArgs>
  static inline jobject Construct(jclass clazz, jmethodID ctor_method,
                                  CtorArgs&&... ctor_args) {
    using Local = LifecycleHelper<jobject, LifecycleType::LOCAL>;

    jobject local_object = Local::Construct(
        clazz, ctor_method, std::forward<CtorArgs&&>(ctor_args)...);
    jobject global_object = Promote(local_object);
    Local::Delete(local_object);

    return global_object;
  }
};

// jclass.
template <>
struct LifecycleHelper<jclass, LifecycleType::LOCAL>
    : public LifecycleLocalBase<jclass> {};

template <>
struct LifecycleHelper<jclass, LifecycleType::GLOBAL>
    : public LifecycleGlobalBase<jclass> {};

}  // namespace jni

#include <string_view>

namespace jni {

// Translates a single JNI term (e.g. jint -> "I") as if it were being used as a
// parameter to a method.
//
// Note, the context a parameter is used on occasion will alter a signature,
// e.g. void in a return is explicit, whereas when used as a parameter, it is
// represented as the omission of any value.
//
// Additionally, Android will obnoxiously fail to compile the standard looking:
//    static constexpr char kStr[] = "SomeString";
//
// Because it is against style to import a using declaration header wide,
// but these are also template definitions, they must remain in this header, and
// so there are goofy looking "using literal" declarations throughout.
//
// https://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/types.html
//
// TODO:  Rename to JavaPrimitiveTypeToString
template <typename JavaType>
constexpr std::string_view JavaTypeToString();

template <>
constexpr std::string_view JavaTypeToString<void>() {
  // Note:  This only applies when used as a return, not as a parameter.  This
  // could be enforced through type system, but maybe feels excessive to do so.
  // For now, enforcing this is unnecesssary, as this function is only called
  // for each Param, which, in the case of no params, is 0 times.
  using namespace std::literals;
  return "V"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jboolean>() {
  using namespace std::literals;
  return "Z"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jbyte>() {
  using namespace std::literals;
  return "B"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jchar>() {
  using namespace std::literals;
  return "C"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jshort>() {
  using namespace std::literals;
  return "S"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jint>() {
  using namespace std::literals;
  return "I"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jlong>() {
  using namespace std::literals;
  return "J"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jfloat>() {
  using namespace std::literals;
  return "F"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jdouble>() {
  using namespace std::literals;
  return "D"sv;
}

template <>
constexpr std::string_view JavaTypeToString<jstring>() {
  using namespace std::literals;
  return "Ljava/lang/String;"sv;
}

}  // namespace jni

namespace jni {

// Tag to indicate you are referring to the enclosing class.
// Useful for builder patterns where the decorated object returned is identical.
struct Self {};

}  // namespace jni

#include <string_view>
#include <type_traits>

namespace jni {

// Metafunction that returns either "" if a member called |name_| isn't
// present, or a constexpr std::string_view of the name if it is.
template <const auto&, typename Enable = void>
struct NameOrNothing {
  static constexpr std::string_view val{""};
};

template <const auto& val_>
struct NameOrNothing<val_, std::void_t<decltype(val_.name_)>> {
  static constexpr std::string_view val{val_.name_};
};

template <const auto& val>
static constexpr auto NameOrNothing_v = NameOrNothing<val>::val;

static constexpr std::string_view kInit{"<init>"};

}  // namespace jni

#include <tuple>
#include <type_traits>
#include <utility>

namespace jni {

template <typename... ClassLoaderTs>
class Jvm {
 public:
  const std::tuple<ClassLoaderTs...> class_loaders_;

  constexpr Jvm(ClassLoaderTs... class_loaders)
      : class_loaders_(class_loaders...) {}

  template <const auto& class_loader_v, std::size_t... Is>
  constexpr size_t IdxOfClassLoaderHelper(
      std::integer_sequence<std::size_t, Is...>) const {
    return metaprogramming::ModifiedMax(
        {((std::get<Is>(class_loaders_) == class_loader_v) ? Is : -1)...});
  }

  // Returns the index for a given classloader within this set (any given class
  // ref is defined by this index).
  template <const auto& class_loader_v>
  constexpr size_t IdxOfClassLoader() const {
    return IdxOfClassLoaderHelper<class_loader_v>(
        std::make_integer_sequence<std::size_t, sizeof...(ClassLoaderTs)>());
  }

  template <typename T>
  bool constexpr operator==(const T& rhs) const {
    return false;
  }
  bool constexpr operator==(const Jvm&) const { return true; }

  template <typename T>
  bool constexpr operator!=(const T& rhs) const {
    return !(*this == rhs);
  }
};

template <typename... ClassLoaderTs>
Jvm(ClassLoaderTs...) -> Jvm<ClassLoaderTs...>;

// Convenience Jvm definition.
// Compatible with default class loader or specified loaders.
inline constexpr Jvm kDefaultJvm{kDefaultClassLoader};

}  // namespace jni

namespace jni {

enum class IdType {
  CLASS,
  STATIC_OVERLOAD_SET,
  STATIC_OVERLOAD,
  STATIC_OVERLOAD_PARAM,
  OVERLOAD_SET,
  OVERLOAD,
  OVERLOAD_PARAM,
  STATIC_FIELD,
  FIELD,
};

}  // namespace jni

#include <array>
#include <cstddef>
#include <utility>

namespace jni::metaprogramming {

// Wrapper to convert a sequence of values into a type.
template <const auto val_>
struct Val {
  using type = decltype(val_);
  static constexpr type val = val_;
};

template <const auto val>
using Val_t = Val<val>;

// Wrapper to convert a sequence of values into a type.
template <auto... Vs>
struct Vals {
  static constexpr std::array val{Vs...};
};

// Wrapper to convert a sequence of const values into a type.
template <const auto... Vs>
struct ValsConst {
  static constexpr std::array val{Vs...};
};

// Wrapper to convert a sequence of ref values into a type.
template <auto&... Vs>
struct ValsRef {
  static constexpr std::array val{Vs...};
};

// Wrapper to convert a sequence of const ref values into a type.
template <const auto&... Vs>
struct ValsConstRef {
  static constexpr std::array val{Vs...};
};

}  // namespace jni::metaprogramming

#include <tuple>
#include <utility>

namespace jni::metaprogramming {

// Metafunction to take a sequence of indices, and produce the corresponding
// elements in sequences of tuples.
template <std::size_t... sizes>
struct TypeIndexMask {
  template <typename, typename... Ts>
  struct GetHelper {};

  template <std::size_t... Is, typename... Ts>
  struct GetHelper<std::index_sequence<Is...>, Ts...> {
    using type = std::tuple<TypeOfNthTupleElement_t<Is, Ts>...>;
  };

  template <typename... Ts>
  using Get = typename GetHelper<std::index_sequence<sizes...>, Ts...>::type;

  template <typename>
  struct SequenceGenerator {};

  template <typename... Ts>
  struct SequenceGenerator<std::tuple<Ts...>> {
    using type = Get<Ts...>;
  };

  template <typename Tup>
  using type = typename SequenceGenerator<Tup>::type;
};

template <typename TupOfTypes, std::size_t... sizes>
using TypeTupFromIndexMask_t =
    typename TypeIndexMask<sizes...>::template type<TupOfTypes>;

}  // namespace jni::metaprogramming

#include <algorithm>
#include <utility>

namespace jni::metaprogramming {

// Represents a value that can be [0, max], and an overflow bit.
// When incremented, the type will rollover, and set its overflow bit.
template <size_t cur_value, size_t max = 1, bool overflow_bit = false>
struct NBit {
  static constexpr size_t value_ = cur_value;
  static constexpr size_t overflow_bit_ = overflow_bit;

  // Only unary values will ever need the clamp.
  using Increment =
      NBit<(value_ == max ? 0 : std::clamp(value_ + 1, size_t{0}, max)), max,
           value_ == max>;

  using ResetOverflow = NBit<cur_value, max, false>;
};

}  // namespace jni::metaprogramming


#include <type_traits>
#include <utility>

namespace jni::metaprogramming {

// Metafunction to increment types a certain number of times.
// Types must export a type alias "Increment" which increments the type once.
template <std::size_t I>
struct Increment {
  template <typename T, std::size_t Idx>
  struct IncrementCountHelper {
    using type = typename Increment<Idx - 1>::template type<T>::Increment;
  };

  template <typename T>
  struct IncrementCountHelper<T, 0> {
    using type = T;
  };

  template <typename T>
  using type = typename IncrementCountHelper<T, I>::type;
};

template <typename T, std::size_t I = 1>
using Increment_t = typename Increment<I>::template type<T>;

}  // namespace jni::metaprogramming

#include <string_view>

namespace jni {

template <bool useParent, typename T>
struct ParentIfSelf {
  using type = T;
};

template <typename T>
struct ParentIfSelf<true, T> {
  using type = typename T::template ChangeIdType<IdType::CLASS>;
};

// Helper to generate full signature information for a "selected" value, and
// possibly some container information.  Here, |Selector| is |MethodSelection|,
// |FieldSelection|, etc.
//
// Unfortunately, the type system does not permit passing subobjects, however,
// types can be used that represent a specific selection upon an object.  This
// consolidates all signature info.
//
// |Selector| a type alias |RawValT| (e.g. jint, Class{...}, etc...).
template <typename SelectorIn>
struct SelectorStaticInfo {
  static constexpr inline bool kIsSelf =
      std::is_same_v<Self, typename SelectorIn::RawValT>;
  using Selector = typename ParentIfSelf<kIsSelf, SelectorIn>::type;

  template <std::size_t I>
  struct IthRawTypeMember {
    template <typename T>
    static constexpr const auto& Val(const T& val) {
      return IthRawTypeMember<I - 1>::Val(val.raw_);
    }
  };

  template <>
  struct IthRawTypeMember<0> {
    template <typename T>
    static constexpr const auto& Val(const T& val) {
      return val;
    }
  };

  // Strangely, the compiler refuses to peer through Val and loses the
  // constexpr-ness (i.e std::decay_t<decltype(Val())>; is not a constant
  // expression).
  static constexpr inline auto Val() {
    if constexpr (Selector::kRank == 0) {
      return Selector::Val();
    } else {
      return Selector::Val().raw_;
    }
  }

  using RawValT = typename Selector::RawValT;

  static constexpr inline bool kIsObject = std::is_base_of_v<Object, RawValT>;
  static constexpr inline bool kIsVoid = std::is_same_v<Void, RawValT>;
  static constexpr std::size_t kRank = Selector::kRank;

  static constexpr std::string_view TypeNameOrNothingIfNotAnObject() {
    if constexpr (kIsObject) {
      return Val().name_;
    } else {
      return "";
    }
  }

  static constexpr std::string_view kTypeNameOrNothingIfNotAnObject =
      TypeNameOrNothingIfNotAnObject();

  static constexpr std::string_view kEmptyStr = "";
  static constexpr std::string_view kModifierStr =
      (kRank == 0) ? ""
                   : metaprogramming::RepeatString_v<
                         kRank, metaprogramming::Constants::left_bracket>;

  static constexpr std::string_view UndecoratedTypeName() {
    if constexpr (kIsObject) {
      return metaprogramming::StringConcatenate_v<
          metaprogramming::Constants::L, kTypeNameOrNothingIfNotAnObject,
          metaprogramming::Constants::semi_colon>;
    } else if constexpr (kIsSelf) {
      return metaprogramming::StringConcatenate_v<
          metaprogramming::Constants::L, kTypeNameOrNothingIfNotAnObject,
          metaprogramming::Constants::semi_colon>;
    } else if constexpr (kIsVoid) {
      return JavaTypeToString<void>();
    } else {
      return JavaTypeToString<RawValT>();
    }
  }

  static constexpr std::string_view kUndecoratedTypeName =
      UndecoratedTypeName();

  static constexpr std::string_view TypeName() {
    return metaprogramming::StringConcatenate_v<kModifierStr,
                                                kUndecoratedTypeName>;
  }

  static constexpr std::string_view kTypeName = TypeName();
};

}  // namespace jni

#include <type_traits>

namespace jni {

template <typename TUndecayed>
struct ProxyHelper;

// See jni::Proxy.
template <typename T>
using Proxy_t = typename ProxyHelper<T>::Proxy_t;

template <typename T>
using Index_t = typename ProxyHelper<T>::Index;

template <typename T>
using CDecl_t = typename ProxyHelper<T>::CDecl;

template <typename T, typename Id>
using Return_t = typename ProxyHelper<T>::template AsReturn_t<Id>;

template <typename T, typename ParamSelection>
using Arg_t = typename ProxyHelper<T>::template AsArg_t<ParamSelection>;

template <typename T>
using AsDecl_t = typename ProxyHelper<T>::AsDecl_t;

// Instead of directly searching for the type, convertible types are sought.
// E.g. A string like "Foo" the type will be const char[4] not const char*.
template <typename Query>
struct IsConvertibleKey {
  template <typename T>
  static constexpr bool value =
      std::is_same_v<Query, std::decay_t<T>> ||
      std::is_base_of_v<std::decay_t<T>, std::decay_t<Query>> ||
      std::is_base_of_v<std::decay_t<Query>, std::decay_t<T>>;
};

template <typename Query, typename T>
static constexpr bool IsConvertibleKey_v =
    IsConvertibleKey<Query>::template value<T>;

}  // namespace jni

namespace jni {

// Use these pass through macros to avoid clang-tidy warnings.
#define JNI_BIND_EXPORT JNIEXPORT
#define JNI_BIND_CALL JNICALL

// Helper macro for defining native JNI entrypoints.
#define JNI_BIND_C_ENTRYPOINT(class_name, return_type, method_name, ...) \
  return_type class_name_##method_name(JNIEnv*, jclass, ##__VA_ARGS__)

// Provide this base tag to UserDefined to enable custom types.
struct JniUserDefinedCorpusTag {};

// ArrayView Helper.
template <typename T>
struct ArrayViewHelper;

// Id.
template <typename JniT_, IdType kIdType_, std::size_t idx,
          std::size_t secondary_idx, std::size_t tertiary_idx>
struct Id;

// Scope.
template <LifecycleType lifecycleType, typename JniT, typename... ViableSpans>
struct Scoped;

// Objects.
template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class LocalObject;

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class GlobalObject;

// Strings.
class LocalString;
class GlobalString;

// Arrays.
template <typename SpanType, std::size_t kRank_, const auto& class_v_,
          const auto& class_loader_v_, const auto& jvm_v_>
class LocalArray;

// Classloaders.
template <LifecycleType lifecycleType, const auto& jvm_v_,
          const auto& class_loader_v_>
class ClassLoaderRef;

// Jvm.
template <const auto& jvm_v_>
class JvmRef;

// Thread Guards.
class ThreadGuard;
class ThreadLocalGuardDestructor;

// ValidatorProxy.
template <typename JniT>
struct ValidatorProxy;

}  // namespace jni

#include <cstddef>
#include <utility>

namespace jni::metaprogramming {

// This class is a diminished form of `ValsEqual` that treats everything as an
//  auto const&. Unfortunately, MSVC appears to have a bug that makes compiling
// `ValsEqual` impossible, however, JNI Bind only ever needs auto const&.
template <typename T>
struct ValsEqualDiminished {
  template <typename T1, typename T2>
  struct Helper {
    static constexpr bool val = std::is_same_v<T1, T2>;
  };

  template <template <const auto&...> class Container1, const auto&... V1s_,
            template <const auto&...> class Container2, const auto&... V2s_>
  struct Helper<Container1<V1s_...>, Container2<V2s_...>> {
    static constexpr bool val = ((V1s_ == V2s_) && ...);
  };

  template <typename T2>
  static constexpr bool val = Helper<T, T2>::val;
};

template <typename T1, typename T2>
static constexpr bool ValsEqualDiminished_v =
    ValsEqualDiminished<T1>::template val<T2>;

template <auto V1, auto V2>
static constexpr bool ValsEqualDiminished_v_v =
    ValsEqualDiminished<Vals<V1>>::template val<Vals<V2>>;

template <auto& V1, auto& V2>
static constexpr bool ValsEqualDiminished_r_v =
    ValsEqualDiminished<ValsRef<V1>>::template val<ValsRef<V2>>;

template <const auto& V1, const auto& V2>
static constexpr bool ValsEqualDiminished_cr_v =
    ValsEqualDiminished<ValsConstRef<V1>>::template val<ValsConstRef<V2>>;

}  // namespace jni::metaprogramming

#include <tuple>

namespace jni::metaprogramming {

struct Reverse {
  template <typename... Ts>
  struct Helper {
    using type = std::tuple<>;
  };

  template <typename... Ts>
  using Helper_t = typename Helper<std::tuple<Ts...>>::type;

  template <typename T1, typename... Ts>
  struct Helper<std::tuple<T1, Ts...>> {
    using type = ConcatenateTup_t<Helper_t<Ts...>, std::tuple<T1>>;
  };

  template <typename... Ts>
  using type = Helper_t<Ts...>;
};

template <typename... Ts>
using Reverse_t = typename Reverse::template type<Ts...>;

template <typename Tup>
using Reverse_Tup = TupleUnroller_t<Reverse, Tup>;

}  // namespace jni::metaprogramming

#include <string_view>
#include <utility>

namespace jni::metaprogramming {

template <char sought_char, char new_char>
struct Replace {
  template <const std::string_view& str, typename IndexSequence>
  struct Helper;

  template <const std::string_view& str, std::size_t... Is>
  struct Helper<str, std::index_sequence<Is...>> {
    static constexpr std::string_view val = StringAsType<(
        str[Is] == sought_char ? new_char : str[Is])...>::chars_as_sv;
  };

  template <const std::string_view& str>
  static constexpr std::string_view val =
      Helper<str, std::make_index_sequence<str.length()>>::val;
};

template <const std::string_view& str, char sought_char, char new_char>
static constexpr auto Replace_v =
    Replace<sought_char, new_char>::template val<str>;

}  // namespace jni::metaprogramming

#include <algorithm>
#include <tuple>
#include <utility>

namespace jni::metaprogramming {

template <typename...>
struct NBitSequence {};

template <>
struct NBitSequence<> {
  using Increment = NBitSequence<>;
};

// Represents a sequence of "NBits".  This can be useful for generating
// selections of pools of objects of various sizes.
//
// For a sequence with N values, a tuple of size N that contains tuples whose
// sizes match those of max_values can be selected from using TypeMask.
//
// In other words, a sequence of N sets of sizes K0, K1, K2...KN can represent a
// single selection with an NBitSequence where sizeof...(values) == N and
// sizeof...(max_values) == N where Ki == max_values[i].
template <size_t... values, size_t... max_values, bool... overflows>
struct NBitSequence<NBit<values, max_values, overflows>...> {
  static constexpr size_t sequence_size_ = sizeof...(values);

  using Sequence = std::tuple<NBit<values, max_values, overflows>...>;
  using TypeMask = TypeIndexMask<values...>;

  template <size_t I>
  using GetBit = TypeOfNthTupleElement_t<I, Sequence>;

  // Helper method to compare values of all NBits.
  template <size_t... Bits>
  static constexpr bool Same(std::index_sequence<Bits...> = {}) {
    static_assert(sizeof...(Bits) == sizeof...(values));
    return std::is_same_v<std::index_sequence<values...>,
                          std::index_sequence<Bits...>>;
  }

  static constexpr size_t max_representable_size_ = ((max_values + 1) * ...);

  template <size_t... Bits>
  static constexpr bool same_ = Same(std::index_sequence<Bits...>{});

  // If the next bit is overflowing (i.e. it just rolled over), this current
  // value increments. Bits only overflow for a single cycle, otherwise higher
  // order bits flip when no rollover is happening.
  template <size_t I>
  struct IncrementedBit {
    using type = std::conditional_t<IncrementedBit<I + 1>::type::overflow_bit_,
                                    typename GetBit<I>::Increment,
                                    typename GetBit<I>::ResetOverflow>;
  };

  template <>
  struct IncrementedBit<sequence_size_ - 1> {
    using type = typename GetBit<sequence_size_ - 1>::Increment;
  };

  template <typename index_sequence>
  struct IncrementSequenceHelper {};

  template <size_t... Is>
  struct IncrementSequenceHelper<std::index_sequence<Is...>> {
    using type = NBitSequence<typename IncrementedBit<Is>::type...>;
  };

  using Increment = typename IncrementSequenceHelper<
      std::make_index_sequence<sequence_size_>>::type;
};

}  // namespace jni::metaprogramming

#include <atomic>
#include <functional>
#include <mutex>

namespace jni::metaprogramming {

// This class is intended to mimic a function local static except that it
// supports being reset. The object is constructed with a lambda and all
// subsequent loads will be thread safe, invoking the lambda once (and only
// once) if the currently stored value is T{0}.
//
// This class is thread-safe.  Loads will be cheap (after a potentially
// expensive initial init), stores are expensive.
template <typename T_>
class DoubleLockedValue {
 public:
  template <typename Lambda>
  inline T_ LoadAndMaybeInit(Lambda lambda) {
    // Typical case, value already initialised, perform cheap load and return.
    T_ return_value = value_.load(std::memory_order_acquire);

    if (return_value != T_{0}) {
      return return_value;
    }

    // Value was nil (uninitialised), perform heavy-weight lock.
    std::lock_guard<std::mutex> lock_guard{lock_};

    // Check another thread didn't race to lock before.
    return_value = value_.load(std::memory_order_acquire);
    if (return_value != T_{}) {
      return return_value;
    }

    // Perform the potentially expensive initialisation and return.
    return_value = lambda();
    value_.store(return_value, std::memory_order_release);
    return return_value;
  }

  // Sets the value to {0}.
  inline void Reset() {
    std::lock_guard<std::mutex> lock_guard{lock_};
    value_.store(0, std::memory_order_release);
  }

  // Sets the value to {0} and iff the value was not {0} prior to being torn
  // down, the teardown lambda will be invoked with this value.
  template <typename TeardownLambda>
  inline void Reset(TeardownLambda lambda) {
    std::lock_guard<std::mutex> lock_guard{lock_};
    auto val = value_.load();
    if (val != 0) {
      lambda(val);
      value_.store(0, std::memory_order_release);
    }
  }

 private:
  std::atomic<T_> value_ = {0};
  std::mutex lock_;
};

}  // namespace jni::metaprogramming

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// Metafunction that detects a partial specialisation for a Container.
template <template <typename> class Container>
struct Detect {
  template <typename T, typename Enable = void>
  struct Helper {
    static constexpr bool val = false;
    using type = std::tuple<>;
  };

  template <typename T>
  struct Helper<T, std::void_t<typename Container<T>::type>> {
    static constexpr bool val = true;
    using type = typename Container<T>::type;
  };

  template <typename T>
  using type = typename Helper<T>::type;

  template <typename T>
  static constexpr bool val = Helper<T>::val;
};

template <template <typename> class Container, typename T>
using Detect_t = typename Detect<Container>::template type<T>;

template <template <typename> class Container, typename T>
static constexpr bool Detect_v = Detect<Container>::template val<T>;

}  // namespace jni::metaprogramming

namespace jni::metaprogramming {

// Provide a partial specialization to this class to provide custom types.
template <typename T>
struct UserDefined;

}  // namespace jni::metaprogramming

#include <type_traits>

namespace jni {

// Takes an Id and produces its signature.
template <typename>
struct Signature {};

template <typename JniT_, IdType kIdType_, std::size_t idx,
          std::size_t secondary_idx, std::size_t tertiary_idx>
struct Signature<Id<JniT_, kIdType_, idx, secondary_idx, tertiary_idx>> {
  using IdT = Id<JniT_, kIdType_, idx, secondary_idx, tertiary_idx>;

  static constexpr IdType kChildIdType = kIdType_ == IdType::OVERLOAD
                                             ? IdType::OVERLOAD_PARAM
                                             : IdType::STATIC_OVERLOAD_PARAM;

  template <typename IdxPack>
  struct Helper;

  template <std::size_t... Is>
  struct Helper<std::index_sequence<Is...>> {
    template <std::size_t I>
    struct Val {
      static constexpr std::string_view val =
          Signature<Id<JniT_, kChildIdType, idx, secondary_idx, I>>::val;
    };

    static constexpr std::string_view val =
        metaprogramming::StringConcatenate_v<Val<Is>::val...>;
  };

  struct ReturnHelper {
    static constexpr std::string_view val =
        Signature<Id<JniT_, kChildIdType, idx, secondary_idx, kNoIdx>>::val;
  };

  // For methods and ctors generates the signature, e.g. "(II)LClass1;".
  // For parameters, emits just a type name.
  static constexpr std::string_view Val() {
    if constexpr (kIdType_ == IdType::CLASS) {
      return SelectorStaticInfo<IdT>::TypeName();
    } else if constexpr (kIdType_ == IdType::FIELD ||
                         kIdType_ == IdType::STATIC_FIELD) {
      return SelectorStaticInfo<IdT>::TypeName();
    } else if constexpr (kIdType_ == IdType::OVERLOAD_SET) {
      return "NOT_IMPLEMENTED";
    } else if constexpr (kIdType_ == IdType::OVERLOAD ||
                         kIdType_ == IdType::STATIC_OVERLOAD) {
      using Idxs = std::make_index_sequence<IdT::NumParams()>;
      if constexpr (IdT::kIsConstructor) {
        return metaprogramming::StringConcatenate_v<
            metaprogramming::Constants::left_parenthesis, Helper<Idxs>::val,
            metaprogramming::Constants::right_parenthesis,
            metaprogramming::Constants::V>;
      } else {
        return metaprogramming::StringConcatenate_v<
            metaprogramming::Constants::left_parenthesis, Helper<Idxs>::val,
            metaprogramming::Constants::right_parenthesis, ReturnHelper::val>;
      }
    } else if constexpr (kIdType_ == IdType::OVERLOAD_PARAM ||
                         kIdType_ == IdType::STATIC_OVERLOAD_PARAM) {
      return SelectorStaticInfo<IdT>::TypeName();
    }

    return "NOT_IMPLEMENTED";
  }

  static constexpr std::string_view val{Val()};
};

template <typename T>
static constexpr auto Signature_v = Signature<T>::val;

}  // namespace jni

namespace jni {

struct LoaderTag {};

// Annotation for use in function and field declarations. When used as argument
// the underlying object must come from the same class loader.
template <typename ClassLoaderT, typename ClassT>
struct LoadedBy : LoaderTag {
  const ClassLoaderT class_loader_;
  const ClassT class_;

  static_assert(
      !std::is_same_v<ClassLoaderT, ClassLoader<DefaultClassLoader>>,
      "LoadedBy is not required for the default loader (it's implicit).");

  constexpr LoadedBy(ClassLoaderT class_loader, ClassT clazz)
      : class_loader_(class_loader), class_(clazz) {}
};

template <typename ClassLoaderT, typename ClassT>
LoadedBy(ClassLoaderT, ClassT) -> LoadedBy<ClassLoaderT, ClassT>;

template <typename T>
struct IsLoadedBy {
  static constexpr bool val = false;
};

template <typename ClassLoaderT, typename ClassT>
struct IsLoadedBy<LoadedBy<ClassLoaderT, ClassT>> {
  static constexpr bool val = true;
};

template <typename T>
static constexpr bool IsLoadedBy_v = IsLoadedBy<T>::val;

template <typename T>
constexpr auto StripClassFromLoadedBy(T val) {
  if constexpr (IsLoadedBy_v<T>) {
    return val.class_;
  } else {
    return val;
  }
}

template <typename T>
constexpr auto StripClassLoaderFromLoadedBy(T val) {
  if constexpr (IsLoadedBy_v<T>) {
    return val.class_loader_;
  } else {
    return val;
  }
}

}  // namespace jni


namespace jni::metaprogramming {

// Builds a stable (i.e. first unique type is preserved) set of unique types
// from a set.
struct UniqueSet {
  template <typename... Ts>
  struct Helper {
    using type = std::tuple<>;
  };

  template <typename... Ts>
  using Helper_t = typename Helper<Ts...>::type;

  // Strips the first element from the first set if it's not unique.
  template <typename T, typename... Ts>
  struct Helper<T, Ts...> {
    using type =
        std::conditional_t<!Contains_v<T, Ts...>,
                           ConcatenateTup_t<std::tuple<T>, Helper_t<Ts...>>,
                           Helper_t<Ts...>>;
  };

  template <typename... Ts>
  using InputSetReversed = Reverse_t<Ts...>;

  template <typename... Ts>
  using ReverseSetStrippedOfDuplicates =
      typename TupleToType_t<InputSetReversed<Ts...>, Helper>::type;

  template <typename... Ts>
  using type = Reverse_Tup<ReverseSetStrippedOfDuplicates<Ts...>>;
};

template <typename... Ts>
using UniqueSet_t = typename UniqueSet::template type<Ts...>;

template <typename Tup>
using UniqueSet_Tup = TupleUnroller_t<UniqueSet, Tup>;

}  // namespace jni::metaprogramming


#include <tuple>

namespace jni::metaprogramming {

// Metafunction that takes sets of tuples and reduces them to a single tuple.
// No element in the result set will be a tuple, any and all nested tuples will
// be flattened.
struct Flatten {
  template <typename... Ts>
  struct FlattenHelper {
    using type = std::tuple<>;
  };

  template <typename... TupleContents, typename... Ts>
  struct FlattenHelper<std::tuple<TupleContents...>, Ts...> {
    // If the type itself is a tuple, it may itself contain nested tuples.
    using type =
        ConcatenateTup_t<typename FlattenHelper<TupleContents...>::type,
                         typename FlattenHelper<Ts...>::type>;
  };

  template <typename NonTupleType, typename... Ts>
  struct FlattenHelper<NonTupleType, Ts...> {
    // The first type is for certain not a tuple or the alternate specialisation
    // would have been chosen. The value is wrapped back into a tuple, in order
    // to exploit ConcatenateTup_t and potentially flatten any remaining args.
    using type = ConcatenateTup_t<std::tuple<NonTupleType>,
                                  typename FlattenHelper<Ts...>::type>;
  };

  template <typename... Ts>
  using type = typename FlattenHelper<Ts...>::type;
};

template <typename... Ts>
using Flatten_t = typename Flatten::template type<Ts...>;

}  // namespace jni::metaprogramming

#include <tuple>

namespace jni::metaprogramming {

// Provides the universe of keys, including user defined types if any.
// Users define custom types by partially specialising UserDefined (see test).
template <typename... Defaults>
struct Corpus {
  template <typename T, bool = Detect_v<UserDefined, T>>
  struct Helper {
    using type =
        ConcatenateTup_t<Detect_t<UserDefined, T>, std::tuple<Defaults...>>;
  };

  template <typename T>
  struct Helper<T, false> {
    using type = std::tuple<Defaults...>;
  };

  template <typename T>
  using type = typename Helper<T>::type;
};

template <typename T, typename... Defaults>
using Corpus_t = typename Corpus<Defaults...>::template type<T>;

}  // namespace jni::metaprogramming

#include <tuple>
#include <type_traits>
#include <utility>

namespace jni::metaprogramming {

// Metafunction that takes a list of tuples, and creates a counter where each
// value is the size of the corresponding tuple.  This can be used to create
// every possible permutation of the input tuples.
struct GenerateBitSequenceFromTupSets {
  template <typename... Tups>
  struct Helper {
    using ListOfTups = std::tuple<Tups...>;

    template <typename>
    struct SequenceGenerator {};

    template <size_t... Is>
    struct SequenceGenerator<std::index_sequence<Is...>> {
      // Generates count to max size inclusive, so -1 on the size.
      // e.g. std::tuple<int> has only 1 viable selection but would be
      // represented by a unary NBit (i.e NBit<0, 0, false>);
      using type =
          NBitSequence<NBit<0,
                            std::tuple_size_v<TypeOfNthTupleElement_t<
                                Is, typename Helper<Tups...>::ListOfTups>> -
                                1,
                            false>...>;
    };
  };

  template <typename... Tups>
  using type = typename Helper<Tups...>::template SequenceGenerator<
      std::make_index_sequence<sizeof...(Tups)>>::type;
};

struct GenerateBitSequenceFromTupSetsFunc {
  template <typename... Tups>
  using type = typename GenerateBitSequenceFromTupSets::template type<Tups...>;
};

template <typename... Tups>
using GenerateBitSequenceFromTupSets_t =
    typename GenerateBitSequenceFromTupSets::template type<Tups...>;

struct CartesianProduct {
  template <typename... Tups>
  struct CartesianProductHelper {
    using NBitSequenceForTups = GenerateBitSequenceFromTupSets_t<Tups...>;
    using AllTupsAsList = std::tuple<Tups...>;

    template <typename>
    struct SequenceGenerator {};

    template <size_t... Is>
    struct SequenceGenerator<std::index_sequence<Is...>> {
      using type = std::tuple<typename Increment_t<
          NBitSequenceForTups, Is>::TypeMask::template type<AllTupsAsList>...>;
    };

    using type = typename SequenceGenerator<std::make_index_sequence<
        NBitSequenceForTups::max_representable_size_>>::type;
  };

  template <>
  struct CartesianProductHelper<> {
    using type = std::tuple<>;
  };

  template <typename... Tups>
  using type = typename CartesianProductHelper<Tups...>::type;
};

template <typename... Tups>
using CartesianProduct_t = typename CartesianProduct::template type<Tups...>;

}  // namespace jni::metaprogramming

#include <vector>

namespace jni {

// Used as shared storage of lists for IDs like jclass, jMethod, etc.
// Only applicable for Jvms not fully specified (i.e. default classloader).
// See JvmRef::~JvmRef.
template <typename T>
static std::vector<metaprogramming::DoubleLockedValue<T>*>& DefaultRefs() {
  static auto* ret_val =
      new std::vector<metaprogramming::DoubleLockedValue<T>*>{};
  return *ret_val;
}

// Provides a static inline `DoubleLockedValue<T>` val against a `UniqueID`.
template <typename UniqueID, typename T>
struct StaticDoubleLock {
  static inline metaprogramming::DoubleLockedValue<T> val;
};

// Takes a GetLambda and only invokes it for the first time on equal values of
// `SignatureLambda`. This is useful for putting `const char*` into type IDs.
template <typename GetLambda, typename SignatureLambda>
struct RefStorage {
  // Return of `GetLambda`.
  using ReturnT = decltype(std::declval<GetLambda>()(nullptr));

  // Compile-time unique ID.
  static constexpr auto kSignature = []() {
    return SignatureLambda::TypeName().data();
  };
  using Signature = metaprogramming::LambdaStringToType<decltype(kSignature)>;

  // Common ID-wide double locked value.
  using Storage = StaticDoubleLock<Signature, ReturnT>;

  // Retrieves the guarded value, possibly invoking the expensive lambda.
  static ReturnT Get(GetLambda lambda) {
    return StaticDoubleLock<Signature, ReturnT>::val.LoadAndMaybeInit(
        std::bind(lambda, &Storage::val));
  }
};

}  // namespace jni

#include <optional>

namespace jni {

struct RefBaseBase {};

// Used to detect RefBase in type proxying.
// This is useful, e.g. when you want to say "an object that might be passed"
// but the object's type (i.e. full name + loader information) is unknown.
template <typename StorageType>
class RefBaseTag : public RefBaseBase {
 public:
  template <typename Base, LifecycleType lifecycleType, typename JniT,
            typename ViableSpan>
  friend struct EntryBase;

  RefBaseTag(StorageType object) : object_ref_(object) {}

  RefBaseTag(const RefBaseTag& rhs) = delete;

  RefBaseTag(RefBaseTag&& rhs) {
    object_ref_ = rhs.object_ref_;
    rhs.object_ref_ = nullptr;
  }

  template <typename T,
            typename = std::enable_if_t<std::is_same_v<T, StorageType>>>
  RefBaseTag(RefBaseTag<T>&& rhs) : object_ref_(rhs.Release()) {}

  StorageType Release() {
    StorageType return_value = object_ref_;
    object_ref_ = nullptr;

    return return_value;
  }

  explicit operator StorageType() const { return object_ref_; }

 protected:
  StorageType object_ref_ = nullptr;
};

// Represents a runtime object with only Name information.  It is ephemeral and
// can be used a return value which is immediately wrapped in a rich object type
// (such as LocalObject, LocalString) which has the complete class definition.
//
// This can also be used as a temporary when passed into a function that accepts
// objects.  This ensures type correctness (names must match) but doesn't
// require the full class description be used when describing the function.
template <typename JniT>
class RefBase : public RefBaseTag<typename JniT::StorageType> {
 public:
  using StorageType = typename JniT::StorageType;
  using RefBaseTag<StorageType>::RefBaseTag;
  using RefBaseTag<StorageType>::operator typename JniT::StorageType;

  static inline const char* name_ = JniT::class_v.name_;

  RefBase(const RefBase& rhs) = delete;

  RefBase(RefBaseTag<StorageType>&& rhs)
      : RefBaseTag<StorageType>(std::move(rhs)) {}
};

//==============================================================================
template <typename T>
using RefBaseT_t = typename T::RefBaseT;

}  // namespace jni

#include <type_traits>
#include <variant>

namespace jni {

template <typename SpanType_, const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm, std::size_t kRank_ = 0,
          std::size_t class_idx_ = kNoIdx,
          std::size_t class_loader_idx_ = kNoIdx>
struct JniT {
  static constexpr std::size_t kRank = kRank_;
  static_assert(kRank != -1);

  // Same as this type, except uses rank-1.
  using RankLess1 = JniT<SpanType_, class_v_, class_loader_v_, jvm_v_,
                         kRank_ - 1, class_idx_, class_loader_idx_>;

  static constexpr bool MinimallySpecified() {
    return (class_v_ == kNoClassSpecified
                ? true
                : (class_idx_ == kNoIdx ? true : false)) &&
           (class_loader_v_ == kDefaultClassLoader
                ? true
                : (class_loader_idx_ == kNoIdx ? true : false));
  }

  static constexpr auto GetClass() {
    if constexpr (class_idx_ != kNoIdx) {
      return StripClassFromLoadedBy(
          std::get<class_idx_>(GetClassLoader().supported_classes_));
    } else {
      return StripClassFromLoadedBy(class_v_);
    }
  }

  static constexpr auto GetClassLoader() {
    if constexpr (class_loader_idx_ != kNoIdx) {
      return StripClassLoaderFromLoadedBy(
          std::get<class_loader_idx_>(jvm_v_.class_loaders_));
    } else {
      return StripClassLoaderFromLoadedBy(class_loader_v_);
    }
  }

  static constexpr std::string_view kName{GetClass().name_};
  static constexpr std::string_view kNameWithDots{
      metaprogramming::Replace_v<kName, '/', '.'>};

  static constexpr const auto& GetJvm() { return jvm_v_; }

  static constexpr auto GetStatic() {
    return FullArrayStripV(GetClass()).static_;
  }

  static constexpr decltype(GetClass()) class_v = GetClass();
  static constexpr decltype(GetClassLoader()) class_loader_v = GetClassLoader();
  static constexpr decltype(jvm_v_) jvm_v = jvm_v_;
  static constexpr decltype(GetStatic()) static_v = GetStatic();

  using SpanType = SpanType_;
  using StorageType = typename StorageHelper<SpanType_, kRank>::type;

  using ClassT = std::decay_t<ArrayStrip_t<std::decay_t<decltype(GetClass())>>>;
  using ClassLoaderT = std::decay_t<decltype(GetClassLoader())>;
  using JvmT = std::decay_t<decltype(jvm_v)>;
  using StaticT = std::decay_t<decltype(GetStatic())>;

  static constexpr ClassT stripped_class_v{FullArrayStripV(GetClass())};

  ////////////////////////////////////////////////////////////////////////////
  // Minimal Spanning type (allows perfect caching for fully specified jvm).
  ////////////////////////////////////////////////////////////////////////////

  // kNoIdx for default loader, then provided idx, then calculated idx.
  static constexpr std::size_t MinimalClassIdx() {
    if constexpr (class_idx_ != kNoIdx) {
      // Fully specified (already).
      return class_idx_;
    } else if constexpr (class_loader_v_ == kDefaultClassLoader) {
      // Partially specified.
      return kNoIdx;
    } else {
      if constexpr (class_loader_v_.template IdxOfClass<class_v>() !=
                    kClassNotInLoaderSetIdx) {
        return class_loader_v_.template IdxOfClass<class_v>();
      } else {
        return kNoIdx;
      }
    }
  }

  // kNoIdx for default jvm, then provided idx, then calculated idx.
  static constexpr std::size_t MinimalClassLoaderIdx() {
    // Fully specified (already).
    if constexpr (class_loader_idx_ != kNoIdx) {
      return class_loader_idx_;
    } else if constexpr (jvm_v_ == kDefaultJvm) {
      // Partially specified.
      return kNoIdx;
    } else {
      // Fully specified (derived).
      return jvm_v_.template IdxOfClassLoader<class_loader_v>();
    }
  }

  static constexpr auto& MinimalSpanningClass() {
    if constexpr (MinimalClassIdx() == kNoIdx) {
      // If idx uncomputable, use provided.
      return class_v_;
    } else {
      return kNoClassSpecified;
    }
  }

  static constexpr auto& MinimalSpanningClassLoader() {
    if constexpr (MinimalClassLoaderIdx() == kNoIdx) {
      // If idx uncomputable, use provided.
      return class_loader_v_;
    } else {
      return kNoClassLoaderSpecified;
    }
  }

  // Some compilers seem to only be OK if this indirection is used.
  static constexpr auto& kMinimalSpanningClass = MinimalSpanningClass();
  static constexpr auto& kMinimalSpanningClassLoader =
      MinimalSpanningClassLoader();

  using MinimallySpanningType =
      JniT<SpanType_, kMinimalSpanningClass, kMinimalSpanningClassLoader,
           jvm_v_, kRank, MinimalClassIdx(), MinimalClassLoaderIdx()>;
};

template <typename T1, typename T2>
struct JniTEqual {
  static constexpr bool val = false;
};

template <
    typename SpanType1, const auto& class_v_1, const auto& class_loader_v_1,
    const auto& jvm_v_1, std::size_t kRank_1, std::size_t class_idx_1,
    std::size_t class_loader_idx_1,

    typename SpanType2, const auto& class_v_2, const auto& class_loader_v_2,
    const auto& jvm_v_2, std::size_t kRank_2, std::size_t class_idx_2,
    std::size_t class_loader_idx_2>
struct JniTEqual<JniT<SpanType1, class_v_1, class_loader_v_1, jvm_v_1, kRank_1,
                      class_idx_1, class_loader_idx_1>,
                 JniT<SpanType2, class_v_2, class_loader_v_2, jvm_v_2, kRank_2,
                      class_idx_2, class_loader_idx_2>> {
  static constexpr bool val =
      std::is_same_v<SpanType1, SpanType2> &&
      metaprogramming::ValsEqualDiminished_cr_v<class_v_1, class_v_2> &&
      metaprogramming::ValsEqualDiminished_cr_v<class_loader_v_1,
                                                class_loader_v_2> &&
      metaprogramming::ValsEqualDiminished_cr_v<jvm_v_1, jvm_v_2> &&
      kRank_1 == kRank_2 && class_idx_1 == class_idx_2 &&
      class_loader_idx_1 == class_loader_idx_2;
};

template <typename T1, typename T2>
constexpr bool JniTEqual_v = JniTEqual<T1, T2>::val;

template <typename JniT_, typename SpanT, int kLessRank>
struct RawProxy {
  using RawValT = SpanT;
  static constexpr std::size_t kRank = JniT_::kRank;
  const RawValT raw_{0};
};

template <typename JniT, int kLessRank>
struct RawProxy<JniT, jobject, kLessRank> {
  using RawValT = decltype(JniT::GetClass());

  const RawValT raw_ = JniT::GetClass();
  const char* name_ = JniT::GetClass().name_;

  static constexpr std::size_t kRank = JniT::kRank + kLessRank;
};

// Helper to generate signatures for objects at rank-1 but span types at rank.
// Used in static selection signature generation (for types like LocalArray).
template <typename JniT, int kLessRank>
struct JniTSelector {
  using RawProxyT = RawProxy<JniT, typename JniT::SpanType, kLessRank>;
  using RawValT = typename RawProxyT::RawValT;

  static constexpr std::size_t kRank = RawProxyT::kRank;

  static constexpr auto Val() { return RawProxyT{}; }
};

}  // namespace jni


#include <type_traits>
#include <utility>

namespace jni::metaprogramming {

template <class T, std::size_t>
using T_ = T;

template <class DefaultType, std::size_t... Is>
auto TupleFromSize(std::index_sequence<Is...>) {
  return std::tuple<T_<DefaultType, Is>...>{};
}

// Takes a type and returns a std::tuple of DefaultValues.
template <class DefaultType, std::size_t N>
auto TupleFromSize() {
  return TupleFromSize<DefaultType>(std::make_index_sequence<N>{});
}

template <class DefaultType, std::size_t N>
using TupleFromSize_t = decltype(TupleFromSize<DefaultType, N>());

}  // namespace jni::metaprogramming

#include <tuple>
#include <type_traits>
#include <utility>

namespace jni::metaprogramming {

// Returns a null pointer of the type of the two input tuples interleaved.
template <class Tuple1, class Tuple2, std::size_t... indices>
auto Interleave(std::integer_sequence<std::size_t, indices...>)
    -> decltype(std::tuple_cat(
        std::make_tuple(std::get<indices>(std::declval<Tuple1>()),
                        std::get<indices>(std::declval<Tuple2>()))...))* {
  // This interleave is for *types only*, all values within the tuples are
  // completely incidental.  In the event there is no default constructor, it
  // won't be possible to return a value, so, instead, return a pointer (which
  // won't be used) and infer the type by stripping the pointer.
  return nullptr;
}

template <class Tuple1, class Tuple2>
auto Interleave() {
  return Interleave<Tuple1, Tuple2>(
      std::make_index_sequence<std::tuple_size<Tuple1>::value>());
}

template <typename T0, typename T1>
struct Interleaved;

template <typename... T0, typename... T1>
struct Interleaved<std::tuple<T0...>, std::tuple<T1...>> {
  using type = std::remove_pointer_t<
      decltype(Interleave<std::tuple<T0...>, std::tuple<T1...>>())>;
};

}  // namespace jni::metaprogramming


namespace jni {

template <>
struct LifecycleHelper<jstring, LifecycleType::LOCAL>
    : public LifecycleLocalBase<jstring> {
  static inline jstring Construct(const char* chars) {
    Trace(metaprogramming::LambdaToStr(STR("NewStringUTF")), chars);

#ifdef DRY_RUN
    return Fake<jstring>();
#else
    return jni::JniEnv::GetEnv()->NewStringUTF(chars);
#endif  // DRY_RUN
  }
};

template <>
struct LifecycleHelper<jstring, LifecycleType::GLOBAL>
    : public LifecycleGlobalBase<jstring> {
  template <typename... CtorArgs>
  static inline jstring Construct(const char* chars) {
    using Local = LifecycleHelper<jstring, LifecycleType::LOCAL>;

    jstring local_string = Local::Construct(chars);
    jstring global_string = Promote(local_string);
    Local::Delete(local_string);

    return global_string;
  }
};

}  // namespace jni

#include <utility>

namespace jni {

template <typename ReturnType, std::size_t kRank, bool kStatic>
class InvokeHelper {};

////////////////////////////////////////////////////////////////////////////////
// Rank 0 type: void
//    void is special, but for symmetry it uses rank 0 with primitives.
////////////////////////////////////////////////////////////////////////////////
template <>
struct InvokeHelper<void, 0, false> {
  template <typename... Ts>
  static void Invoke(jobject object, jclass clazz, jmethodID method_id,
                     Ts&&... ts) {
#ifdef DRY_RUN
#else
    Trace(metaprogramming::LambdaToStr(STR("CallVoidMethod")), object, clazz,
          method_id, ts...);

    jni::JniEnv::GetEnv()->CallVoidMethod(object, method_id,
                                          std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 0 types, i.e. the primitive type itself (e.g. int).
////////////////////////////////////////////////////////////////////////////////
template <>
struct InvokeHelper<jboolean, 0, false> {
  template <typename... Ts>
  static jboolean Invoke(jobject object, jclass clazz, jmethodID method_id,
                         Ts&&... ts) {
#ifdef DRY_RUN
    return Fake<jboolean>();
#else
    Trace(metaprogramming::LambdaToStr(STR("CallBooleanMethod")), object, clazz,
          method_id, ts...);

    return jni::JniEnv::GetEnv()->CallBooleanMethod(object, method_id,
                                                    std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jint, 0, false> {
  template <typename... Ts>
  static jint Invoke(jobject object, jclass clazz, jmethodID method_id,
                     Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallIntMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jint>();
#else
    return jni::JniEnv::GetEnv()->CallIntMethod(object, method_id,
                                                std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jlong, 0, false> {
  template <typename... Ts>
  static jlong Invoke(jobject object, jclass clazz, jmethodID method_id,
                      Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallLongMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jlong>();
#else
    return jni::JniEnv::GetEnv()->CallLongMethod(object, method_id,
                                                 std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jfloat, 0, false> {
  template <typename... Ts>
  static jfloat Invoke(jobject object, jclass clazz, jmethodID method_id,
                       Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallFloatMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    //    return Fake<jfloat>();
    return 123.f;
#else
    return jni::JniEnv::GetEnv()->CallFloatMethod(object, method_id,
                                                  std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jdouble, 0, false> {
  template <typename... Ts>
  static jdouble Invoke(jobject object, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallDoubleMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    // return Fake<jdouble>();
    return 123.f;
#else
    return jni::JniEnv::GetEnv()->CallDoubleMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jobject, 0, false> {
  // This always returns a local reference which should be embedded in type
  // information wherever this is used.
  template <typename... Ts>
  static jobject Invoke(jobject object, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallObjectMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jstring, 0, false> {
  template <typename... Ts>
  static jobject Invoke(jobject object, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallObjectMethod")), object, clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jstring>();
#else
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 1 types, i.e. single dimension arrays (e.g. int[]).
////////////////////////////////////////////////////////////////////////////////
template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jboolean>, kRank, false> {
  template <typename... Ts>
  static jbooleanArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                              Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jbooleanArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jbooleanArray>();
#else
    return static_cast<jbooleanArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jbyte>, kRank, false> {
  template <typename... Ts>
  static jbyteArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jbyteArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jbyteArray>();
#else
    return static_cast<jbyteArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jchar>, kRank, false> {
  template <typename... Ts>
  static jcharArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jcharArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jcharArray>();
#else
    return static_cast<jcharArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jshort>, kRank, false> {
  template <typename... Ts>
  static jshortArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                            Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jshortArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jshortArray>();
#else
    return static_cast<jshortArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jint>, kRank, false> {
  template <typename... Ts>
  static jintArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                          Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jintArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jintArray>();
#else
    return static_cast<jintArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jlong>, kRank, false> {
  template <typename... Ts>
  static jlongArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jlongArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jlongArray>();
#else
    return static_cast<jlongArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jfloat>, kRank, false> {
  template <typename... Ts>
  static jfloatArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                            Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jfloatArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jfloatArray>();
#else
    return static_cast<jfloatArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jdouble>, kRank, false> {
  template <typename... Ts>
  static jdoubleArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jdoubleArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jdoubleArray>();
#else
    return static_cast<jdoubleArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jarray>, kRank, false> {
  // Arrays of arrays (which this invoke represents) return object arrays
  // (arrays themselves are objects, ergo object arrays).
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jobject>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank 1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 2+ types, i.e. multi-dimension arrays (e.g. int[][], int[][][]).
////////////////////////////////////////////////////////////////////////////////
template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jboolean>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jbyte>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jchar>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jshort>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jint>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jfloat>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);
#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jdouble>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jlong>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jarray>, kRank, false> {
  // Arrays of arrays (which this invoke represents) return object arrays
  // (arrays themselves are objects, ergo object arrays).
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jobject>, kRank, false> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallObjectMethod (jobjectArray), Rank >1")),
          object, clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
#endif
  }
};

}  // namespace jni


#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace jni {

template <typename t1, typename t2 = void>
struct Proxy;

// CDecls for all declarable types (these index into proxy definitions).
using AllKeys = metaprogramming::Corpus_t<
    JniUserDefinedCorpusTag, void, jboolean, jbyte, jshort, jint, jfloat, jlong,
    jchar, jdouble, jstring, jobject, Self, jarray, jobjectArray, jintArray,
    jbooleanArray, jbyteArray, jcharArray, jshortArray, jdoubleArray,
    jfloatArray, jlongArray>;

template <typename TUndecayed>
struct ProxyHelper {
  using T = std::decay_t<TUndecayed>;

  // Metafunction that builds a list of a passable type to all it's possible
  // passable types, which may not be the same.  E.g. jint => jint, but
  // jstring => jstring, const char*, std::string_view, std::string, etc.
  struct IndexToKey {
    // Proxies can be indexed by their |AsArg|s or their |AsDecl|.
    template <typename CDecl>
    using type = metaprogramming::CartesianProduct_t<
        std::tuple<CDecl>,
        metaprogramming::UniqueSet_Tup<metaprogramming::ConcatenateTup_t<
            typename Proxy<CDecl>::AsArg, typename Proxy<CDecl>::AsDecl>>>;
  };

  // Build a list of two element tuples (in preparation to build a map).  e.g.
  // { {jint, int}, {jstring, const char*}, {jstring, std::string}, etc. }.
  // Note that types may map to 1 or more types, such as jstring above.
  using IndexToKeyAsTuples = metaprogramming::Reduce_t<
      metaprogramming::Combine,
      metaprogramming::InvokePerTupArg_t<IndexToKey, AllKeys>>;

  // Collapse this list into a set of keys and values consumable by
  // TypeToTypeMap.
  using IndexToKeyMap = metaprogramming::TypeToTypeMapFromKeyValuesTup_t<
      metaprogramming::Flatten_t<IndexToKeyAsTuples>>;

  // When flipped, a type passed can be reverse indexed to select the same
  // Proxy partial specialisation.
  using KeyToIndex = metaprogramming::TypeToTypeMap_Invert<IndexToKeyMap>;

  using Index =
      metaprogramming::TypeToTypeMapQueryWithComparator_t<KeyToIndex,
                                                          IsConvertibleKey<T>>;

  using Proxy_t = Proxy<Index>;

  using CDecl = typename Proxy_t::CDecl;

  template <typename Id>
  using AsReturn_t = typename Proxy_t::template AsReturn<Id>;

  template <typename ParamSelection>
  using AsArg_t = typename Proxy_t::AsArg;

  using AsDecl_t = typename Proxy_t::AsDecl;
};

// Default Proxy, all types and values are pure passthrough.
template <typename Key_>
struct ProxyBase {
  using Key = Key_;

  using CDecl = Key_;

  template <typename>
  using AsReturn = Key_;

  using AsArg = std::tuple<Key_>;
  using AsDecl = std::tuple<Key_>;

  template <typename T>
  static auto ProxyAsArg(T&& t) {
    return std::forward<T>(t);
  }

  template <typename InputParamSelectionT, typename T>
  static constexpr bool kViable = IsConvertibleKey_v<Key_, T>;
};

}  // namespace jni


#include <cstddef>
#include <limits>
#include <string_view>
#include <utility>

namespace jni {

template <typename JniT_, IdType kIdType_, std::size_t idx = kNoIdx,
          std::size_t secondary_idx = kNoIdx, std::size_t tertiary_idx = kNoIdx>
struct Id {
  using JniT = JniT_;
  static constexpr IdType kIdType = kIdType_;

  static constexpr auto Class() { return JniT::GetClass(); }

  static constexpr std::size_t kIdx = idx;
  static constexpr std::size_t kSecondaryIdx = secondary_idx;
  static constexpr std::size_t kTertiaryIdx = tertiary_idx;

  static constexpr bool kIsConstructor =
      (kIdType == IdType::OVERLOAD || kIdType == IdType::OVERLOAD_PARAM ||
       kIdType == IdType::OVERLOAD_SET) &&
      (kIdx == kNoIdx);

  static constexpr bool kIsStatic = kIdType == IdType::STATIC_OVERLOAD_SET ||
                                    kIdType == IdType::STATIC_OVERLOAD ||
                                    kIdType == IdType::STATIC_OVERLOAD_PARAM ||
                                    kIdType == IdType::STATIC_FIELD;

  template <IdType new_id_type>
  using ChangeIdType = Id<JniT, new_id_type, idx, secondary_idx, tertiary_idx>;

  template <std::size_t kIdxToChange, std::size_t kNewValue>
  using ChangeIdx = Id<JniT, kIdType, (kIdxToChange == 0 ? kNewValue : idx),
                       (kIdxToChange == 1 ? kNewValue : secondary_idx),
                       (kIdxToChange == 2 ? kNewValue : tertiary_idx)>;

  static constexpr auto Val() {
    if constexpr (kIdType == IdType::CLASS) {
      return Class();
    } else if constexpr (kIdType == IdType::STATIC_FIELD) {
      static_assert(idx != kNoIdx);
      return std::get<idx>(Class().static_.fields_).raw_;
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD_SET) {
      // Overload (no such thing as static constructor).
      static_assert(idx != kNoIdx);
      return std::get<idx>(Class().static_.methods_);
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD) {
      // Overload (no such thing as static constructor).
      static_assert(idx != kNoIdx);
      return std::get<secondary_idx>(
          std::get<idx>(Class().static_.methods_).invocations_);
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD_PARAM) {
      // Overload.
      if constexpr (tertiary_idx == kNoIdx) {
        // Return.
        return std::get<secondary_idx>(
                   std::get<idx>(Class().static_.methods_).invocations_)
            .return_.raw_;
      } else {
        return std::get<tertiary_idx>(
            std::get<secondary_idx>(
                std::get<idx>(Class().static_.methods_).invocations_)
                .params_.values_);
      }
    } else if constexpr (kIdType == IdType::OVERLOAD_SET) {
      if constexpr (idx == kNoIdx) {
        // Constructor.
        return Class().constructors_;
      } else {
        // Overload.
        return std::get<idx>(Class().methods_);
      }
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      if constexpr (idx == kNoIdx) {
        // Constructor.
        return std::get<secondary_idx>(Class().constructors_);
      } else {
        // Overload.
        return std::get<secondary_idx>(
            std::get<idx>(Class().methods_).invocations_);
      }
    } else if constexpr (kIdType == IdType::OVERLOAD_PARAM) {
      if constexpr (idx == kNoIdx) {
        // Constructor.
        if constexpr (tertiary_idx == kNoIdx) {
          // Return.
          return Class();
        } else {
          // Overload return.
          return std::get<tertiary_idx>(
              std::get<secondary_idx>(Class().constructors_).params_.values_);
        }
      } else {
        // Overload.
        if constexpr (tertiary_idx == kNoIdx) {
          // Return.
          return std::get<secondary_idx>(
                     std::get<idx>(Class().methods_).invocations_)
              .return_.raw_;
        } else {
          return std::get<tertiary_idx>(
              std::get<secondary_idx>(
                  std::get<idx>(Class().methods_).invocations_)
                  .params_.values_);
        }
      }
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD_SET) {
      // Overload (no static_assert).
      static_assert(idx != kNoIdx);

      return std::get<secondary_idx>(
          std::get<idx>(Class().static_methods_).invocations_);
    } else if constexpr (kIdType == IdType::FIELD) {
      static_assert(idx != kNoIdx);
      return std::get<idx>(Class().fields_).raw_;
    }
  }

  // Returns root for constructor, else return's "raw_" member.
  static constexpr auto Materialize() {
    if constexpr (kIdType == IdType::STATIC_OVERLOAD) {
      return Val().return_.raw_;
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      if constexpr (kIdx == kNoIdx) {
        // Constructor.
        return Class();
      } else {
        // Overload return value.
        return Val().return_.raw_;
      }
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD_SET ||
                         kIdType == IdType::OVERLOAD_SET) {
      // Not implemented.
      return Void{};
    } else {
      return Val();
    }
  }

  using MaterializeT = std::decay_t<decltype(Materialize())>;
  using RawMaterializeT = ArrayStrip_t<MaterializeT>;
  static constexpr std::size_t kMaterializedRank =
      Rankifier::Rank(Materialize());
  using MaterializeCDeclT = std::decay_t<VoidIfVoid_t<decltype(Materialize())>>;

  using RawValT = ArrayStrip_t<std::decay_t<decltype(Val())>>;
  using UnstrippedRawVal = std::decay_t<decltype(Val())>;
  using CDecl = CDecl_t<VoidIfVoid_t<MaterializeT>>;

  static constexpr std::size_t kRank = Rankifier::Rank(Val());
  static constexpr bool kIsSelf = std::is_same_v<RawValT, Self>;

  static constexpr const char* Name() {
    if constexpr (kIdType == IdType::CLASS) {
      return Class().name_;
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD_SET) {
      return Val().name_;
    } else if constexpr (kIdType == IdType::STATIC_OVERLOAD) {
      return Id<JniT, IdType::STATIC_OVERLOAD_SET, idx, secondary_idx>::Name();
    } else if constexpr (kIdType == IdType::STATIC_FIELD) {
      return std::get<idx>(Class().static_.fields_).name_;
    } else if constexpr (kIdType == IdType::OVERLOAD_SET && idx == kNoIdx) {
      return "<init>";
    } else if constexpr (kIdType == IdType::OVERLOAD_SET) {
      return Val().name_;
    } else if constexpr (kIdType == IdType::OVERLOAD) {
      return Id<JniT, IdType::OVERLOAD_SET, idx, secondary_idx>::Name();
    } else if constexpr (kIdType == IdType::FIELD) {
      return std::get<idx>(Class().fields_).name_;
    } else {
      return "NO_NAME";
    }
  }
  static constexpr std::string_view kName = Name();
  static constexpr std::string_view kNameUsingDots =
      metaprogramming::Replace_v<kName, '/', '.'>;

  static constexpr std::size_t NumParams() {
    if constexpr (kIdType == IdType::OVERLOAD ||
                  kIdType == IdType::STATIC_OVERLOAD) {
      return std::tuple_size_v<decltype(Val().params_.values_)>;
    } else if constexpr (kIdType == IdType::OVERLOAD_SET ||
                         kIdType == IdType::STATIC_OVERLOAD_SET) {
      if constexpr (idx == kNoIdx) {
        // Constructor.
        return std::tuple_size_v<std::decay_t<decltype(Val())>>;
      } else {
        return std::tuple_size_v<std::decay_t<decltype(Val().invocations_)>>;
      }
    } else {
      // Other types don't have meaningful use of this.
      return 1;
    }
  }

  static constexpr std::size_t kNumParams = NumParams();
};

}  // namespace jni


#include <optional>
#include <tuple>

namespace jni::metaprogramming {

template <typename>
struct OptionalTup {};

// Takes a _tuple_ of types and returns a tuple with the same types except
// wrapped in std::optional.
template <typename... Ts>
struct OptionalTup<std::tuple<Ts...>> {
  using type = std::tuple<std::optional<Ts>...>;
};

template <typename... Ts>
using OptionalTup_t = typename OptionalTup<Ts...>::type;

}  // namespace jni::metaprogramming

#include <string_view>
#include <tuple>
#include <utility>

namespace jni::metaprogramming {

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::*nameable_member>
class InvocableMap;

// This is an interface that can be inherited from to expose an operator(...).
// It provides compile time string index lookup with no macros although it is
// dependent on a clang extension.
//
// To use this API, inherit from this class using template types as follows:
//
// |CrtpBase|: The name of the class inheriting from the map.  This class
//   will inherit an operator().  It must implement this exact signature:
//
//    template <std::size_t I, typename... Args>
//    auto InvocableMapCall(const char* key, Args&&... args);
//
//   If i is the index where |tup_container_v.*nameable_member|.name_ == key,
//     then InvocablemapCall will forward the args from operator() with the
//     same args.  Static memory can be used in this function call and it will
//     be unique because of the I non-type template parameter.
//
// |tup_container_v| is a static instance of an object whose |nameable_member|
//   contains a public field called name_.  It might seem strange not to
//   directly pass a const auto&, however, this prevents accessing subobjects.
//
// The motivation for using inheritance as opposed to a simple member is that
// the the const char cannot be propagated without losing its constexpr-ness,
// and so the clang extension can no longer restrict function candidates.
template <typename CrtpBase, const auto& tup_container_v,
          const auto std::decay_t<decltype(tup_container_v)>::*nameable_member>
using InvocableMap_t =
    InvocableMap<CrtpBase, tup_container_v,
                 std::decay_t<decltype(tup_container_v)>, nameable_member>;

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::*nameable_member,
          std::size_t I>
class InvocableMapEntry;

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::*nameable_member,
          typename IndexSequenceType>
class InvocableMapBase {};

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::*nameable_member,
          std::size_t... idxs>
class InvocableMapBase<CrtpBase, tup_container_v, TupContainerT,
                       nameable_member, std::index_sequence<idxs...>>
    : public InvocableMapEntry<CrtpBase, tup_container_v, TupContainerT,
                               nameable_member, idxs>... {
 public:
  using InvocableMapEntry<CrtpBase, tup_container_v, TupContainerT,
                          nameable_member, idxs>::operator()...;
};

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::*nameable_member,
          std::size_t I>
class InvocableMapEntry {
 public:
#if __clang__
  // This function blurs the distinction between type and value space.  The
  // clang extension allows the key to be wrapped in a constexpr way.  This
  // allows for string to string comparison based on the static value the class
  // is templated by.
  //
  // The reason the TypeMap interface requires inheritance as opposed to simply
  // holding an instance of this map (like you would with a regular hash map) is
  // the constexpr-ness of the string can't be propagated.  This essentially
  // means you get one shot at defining the function.
  template <typename... Args>
  constexpr auto operator()(const char* key, Args&&... args) __attribute__((
      enable_if(std::string_view(key) ==
                    std::get<I>(tup_container_v.*nameable_member).name_,
                ""))) {
    static_assert(std::is_base_of_v<InvocableMapEntry, CrtpBase>,
                  "You must derive from the invocable map.");

    return (*static_cast<CrtpBase*>(this))
        .template InvocableMapCall<I, Args...>(key,
                                               std::forward<Args>(args)...);
  }
#else
  static_assert(false,
                "This container requires clang for compile time strings.");
#endif
};

//==============================================================================
template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::*nameable_member>
class InvocableMap
    : public InvocableMapBase<
          CrtpBase, tup_container_v, TupContainerT, nameable_member,
          std::make_index_sequence<std::tuple_size_v<
              std::decay_t<decltype(tup_container_v.*nameable_member)>>>> {};

}  // namespace jni::metaprogramming


#include <cstddef>
#include <utility>

namespace jni {

////////////////////////////////////////////////////////////////////////////////
// Rank 0 type (aka void).
////////////////////////////////////////////////////////////////////////////////
template <>
struct InvokeHelper<void, 0, true> {
  template <typename... Ts>
  static void Invoke(jobject, jclass clazz, jmethodID method_id, Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticVoidMethod")), clazz,
          method_id, ts...);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->CallStaticVoidMethod(clazz, method_id,
                                                std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 0 types, i.e. the primitive type itself (e.g. int).
////////////////////////////////////////////////////////////////////////////////
template <>
struct InvokeHelper<jboolean, 0, true> {
  template <typename... Ts>
  static jboolean Invoke(jobject, jclass clazz, jmethodID method_id,
                         Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticBooleanMethod")), clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jboolean>();
#else
    return jni::JniEnv::GetEnv()->CallStaticBooleanMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jbyte, 0, true> {
  template <typename... Ts>
  static jboolean Invoke(jobject, jclass clazz, jmethodID method_id,
                         Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticByteMethod")), clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jboolean>();
#else
    return jni::JniEnv::GetEnv()->CallStaticByteMethod(clazz, method_id,
                                                       std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jchar, 0, true> {
  template <typename... Ts>
  static jboolean Invoke(jobject, jclass clazz, jmethodID method_id,
                         Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticCharMethod")), clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jboolean>();
#else
    return jni::JniEnv::GetEnv()->CallStaticCharMethod(clazz, method_id,
                                                       std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jshort, 0, true> {
  template <typename... Ts>
  static jboolean Invoke(jobject, jclass clazz, jmethodID method_id,
                         Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticShortMethod")), clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jboolean>();
#else
    return jni::JniEnv::GetEnv()->CallStaticShortMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jint, 0, true> {
  template <typename... Ts>
  static jint Invoke(jobject, jclass clazz, jmethodID method_id, Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticIntMethod")), clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jint>();
#else
    return jni::JniEnv::GetEnv()->CallStaticIntMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jlong, 0, true> {
  template <typename... Ts>
  static jlong Invoke(jobject, jclass clazz, jmethodID method_id, Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticLongMethod")), clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jlong>();
#else
    return jni::JniEnv::GetEnv()->CallStaticLongMethod(clazz, method_id,
                                                       std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jfloat, 0, true> {
  template <typename... Ts>
  static jfloat Invoke(jobject, jclass clazz, jmethodID method_id, Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticFloatMethod")), clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return 123.f;
#else
    return jni::JniEnv::GetEnv()->CallStaticFloatMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jdouble, 0, true> {
  template <typename... Ts>
  static jdouble Invoke(jobject, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticDoubleMethod")), clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return 123.;
#else
    return jni::JniEnv::GetEnv()->CallStaticDoubleMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jobject, 0, true> {
  // This always returns a local reference which should be embedded in type
  // information wherever this is used.
  template <typename... Ts>
  static jobject Invoke(jobject, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod")), clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return jni::JniEnv::GetEnv()->CallStaticObjectMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

template <>
struct InvokeHelper<jstring, 0, true> {
  template <typename... Ts>
  static jobject Invoke(jobject, jclass clazz, jmethodID method_id,
                        Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod")), clazz,
          method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return jni::JniEnv::GetEnv()->CallStaticObjectMethod(
        clazz, method_id, std::forward<Ts>(ts)...);
#endif  // DRY_RUN
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 1 types, i.e. single dimension arrays (e.g. int[]).
////////////////////////////////////////////////////////////////////////////////
template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jboolean>, kRank, true> {
  template <typename... Ts>
  static jbooleanArray Invoke(jobject, jclass clazz, jmethodID method_id,
                              Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod, Rank 1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jbooleanArray>();
#else
    return static_cast<jbooleanArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jbyte>, kRank, true> {
  template <typename... Ts>
  static jbyteArray Invoke(jobject, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod, Rank 1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jbyteArray>();
#else
    return static_cast<jbyteArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jchar>, kRank, true> {
  template <typename... Ts>
  static jcharArray Invoke(jobject, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod, Rank 1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jcharArray>();
#else
    return static_cast<jcharArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jshort>, kRank, true> {
  template <typename... Ts>
  static jshortArray Invoke(jobject, jclass clazz, jmethodID method_id,
                            Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod, Rank 1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jshortArray>();
#else
    return static_cast<jshortArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jint>, kRank, true> {
  template <typename... Ts>
  static jintArray Invoke(jobject, jclass clazz, jmethodID method_id,
                          Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod, Rank 1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jintArray>();
#else
    return static_cast<jintArray>(jni::JniEnv::GetEnv()->CallStaticObjectMethod(
        clazz, method_id, std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jfloat>, kRank, true> {
  template <typename... Ts>
  static jfloatArray Invoke(jobject, jclass clazz, jmethodID method_id,
                            Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod, Rank 1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jfloatArray>();
#else
    return static_cast<jfloatArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jdouble>, kRank, true> {
  template <typename... Ts>
  static jdoubleArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod, Rank 1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jdoubleArray>();
#else
    return static_cast<jdoubleArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jlong>, kRank, true> {
  template <typename... Ts>
  static jlongArray Invoke(jobject, jclass clazz, jmethodID method_id,
                           Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod, Rank 1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jlongArray>();
#else
    return static_cast<jlongArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jarray>, kRank, true> {
  // Arrays of arrays (which this invoke represents) return object arrays
  // (arrays themselves are objects, ergo object arrays).
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod, Rank 1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank == 1), jobject>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(STR("CallStaticObjectMethod, Rank 1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 2+ types, i.e. multi-dimension arrays (e.g. int[][], int[][][]).
////////////////////////////////////////////////////////////////////////////////
template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jboolean>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallStaticObjectMethod (jboolean), Rank >1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jbyte>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallStaticObjectMethod (jbyte), Rank >1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jchar>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallStaticObjectMethod (jchar), Rank >1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jshort>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallStaticObjectMethod (jshort), Rank >1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jint>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallStaticObjectMethod (jint), Rank >1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jfloat>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallStaticObjectMethod (jfloat), Rank >1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jdouble>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallStaticObjectMethod (jdouble), Rank >1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jlong>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallStaticObjectMethod (jlong), Rank >1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jarray>, kRank, true> {
  // Arrays of arrays (which this invoke represents) return object arrays
  // (arrays themselves are objects, ergo object arrays).
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallStaticObjectMethod (jarray), Rank >1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct InvokeHelper<std::enable_if_t<(kRank > 1), jobject>, kRank, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject, jclass clazz, jmethodID method_id,
                             Ts&&... ts) {
    Trace(metaprogramming::LambdaToStr(
              STR("CallStaticObjectMethod (jobject), Rank >1")),
          clazz, method_id, ts...);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->CallStaticObjectMethod(clazz, method_id,
                                                      std::forward<Ts>(ts)...));
#endif  // DRY_RUN
  }
};

}  // namespace jni

#include <utility>

namespace jni {

template <typename Raw, std::size_t kRank = 0, bool kStatic = false,
          typename Enable = void>
struct FieldHelper {
  static Raw GetValue(jobject object_ref, jfieldID field_ref_);

  static void SetValue(jobject object_ref, jfieldID field_ref_, Raw&& value);
};

////////////////////////////////////////////////////////////////////////////////
// Rank 0: Primitive types (e.g. int).
////////////////////////////////////////////////////////////////////////////////
template <>
struct FieldHelper<jboolean, 0, false, void> {
  static inline jboolean GetValue(const jobject object_ref,
                                  const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetBooleanValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jboolean>();
#else
    return jni::JniEnv::GetEnv()->GetBooleanField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jboolean&& value) {
    Trace(metaprogramming::LambdaToStr(STR("GetBooleanValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetBooleanField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jbyte, 0, false, void> {
  static inline jbyte GetValue(const jobject object_ref,
                               const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetByteValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jbyte>();
#else
    return jni::JniEnv::GetEnv()->GetByteField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jbyte&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetByteValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetByteField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jchar, 0, false, void> {
  static inline jchar GetValue(const jobject object_ref,
                               const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetCharValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jchar>();
#else
    return jni::JniEnv::GetEnv()->GetCharField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jchar&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetCharValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetCharField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jshort, 0, false, void> {
  static inline jshort GetValue(const jobject object_ref,
                                const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetShortValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jshort>();
#else
    return jni::JniEnv::GetEnv()->GetShortField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jshort&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetShortValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetShortField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jint, 0, false, void> {
  static inline jint GetValue(const jobject object_ref,
                              const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetIntValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jint>();
#else
    return jni::JniEnv::GetEnv()->GetIntField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jint&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetIntValue")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetIntField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jlong, 0, false, void> {
  static inline jlong GetValue(const jobject object_ref,
                               const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetLongField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jlong>();
#else
    return jni::JniEnv::GetEnv()->GetLongField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jlong&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetLongField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetLongField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jfloat, 0, false, void> {
  static inline jfloat GetValue(const jobject object_ref,
                                const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetFloatField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return 123.f;
#else
    return jni::JniEnv::GetEnv()->GetFloatField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jfloat&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetFloatField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetFloatField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jdouble, 0, false, void> {
  static inline jdouble GetValue(const jobject object_ref,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetDoubleField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return 123.;
#else
    return jni::JniEnv::GetEnv()->GetDoubleField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jdouble&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetDoubleField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetDoubleField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jobject, 0, false, void> {
  static inline jobject GetValue(const jobject object_ref,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jobject&& new_value) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, new_value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jstring, 0, false, void> {
  static inline jstring GetValue(const jobject object_ref,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jstring>();
#else
    return reinterpret_cast<jstring>(
        jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_));
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jstring&& new_value) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, new_value);
#endif  // DRY_RUN
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 1: Single dimension arrays (e.g. int[]).
////////////////////////////////////////////////////////////////////////////////
template <typename ArrayType>
struct BaseFieldArrayHelper {
  static inline ArrayType GetValue(const jobject object_ref,
                                   const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectField, Rank 1")),
          object_ref, field_ref_);

#ifdef DRY_RUN
    return Fake<ArrayType>();
#else
    return static_cast<ArrayType>(
        jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_));
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, ArrayType&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jboolean>, kRank, false, void>
    : BaseFieldArrayHelper<jbooleanArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jbyte>, kRank, false, void>
    : BaseFieldArrayHelper<jbyteArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jchar>, kRank, false, void>
    : BaseFieldArrayHelper<jcharArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jshort>, kRank, false, void>
    : BaseFieldArrayHelper<jshortArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jint>, kRank, false, void>
    : BaseFieldArrayHelper<jintArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jlong>, kRank, false, void>
    : BaseFieldArrayHelper<jlongArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jfloat>, kRank, false, void>
    : BaseFieldArrayHelper<jfloatArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jdouble>, kRank, false, void>
    : BaseFieldArrayHelper<jdoubleArray> {};

////////////////////////////////////////////////////////////////////////////////
// Rank 1: jobjects & jstrings.
// Rank 2+: Multi-dimension arrays (e.g. int[][], int[][][]).
////////////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t kRank>
struct FieldHelper<
    T, kRank, false,
    std::enable_if_t<(std::is_same_v<jobject, T> ||
                      std::is_same_v<jstring, T> || (kRank > 1))>> {
  static inline jobjectArray GetValue(const jobject object_ref,
                                      const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectField, Rank >1")),
          object_ref, field_ref_);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_));
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, jobjectArray&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectField, Rank >1")),
          object_ref, field_ref_);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

}  // namespace jni

#include <iterator>
#include <string>
#include <type_traits>

namespace jni {

template <typename JString>
struct Proxy<JString,
             typename std::enable_if_t<std::is_same_v<JString, jstring>>>
    : public ProxyBase<JString> {
  template <typename Id, std::size_t kRank>
  struct Helper {
    using type = LocalArray<jstring, kRank, kJavaLangString,
                            kDefaultClassLoader, kDefaultJvm>;
  };

  template <typename Id>
  struct Helper<Id, 0> {
    using type = LocalString;
  };

  using AsArg = std::tuple<std::string, jstring, char*, const char*,
                           std::string_view, RefBaseTag<jstring>>;

  template <typename Id>
  using AsReturn = typename Helper<Id, Id::kRank>::type;

  template <typename OverloadSelection, typename T>
  static constexpr bool kViable =
      IsConvertibleKey<T>::template value<std::string> ||
      IsConvertibleKey<T>::template value<jstring> ||
      IsConvertibleKey<T>::template value<char*> ||
      IsConvertibleKey<T>::template value<const char*> ||
      IsConvertibleKey<T>::template value<std::string_view> ||
      std::is_same_v<T, LocalString> || std::is_same_v<T, GlobalString>;

  // These leak local instances of strings.  Usually, RAII mechanisms would
  // correctly release local instances, but here we are stripping that so it can
  // be used in a method.  This could be obviated by wrapping the calling scope
  // in a local stack frame.
  static jstring ProxyAsArg(jstring s) { return s; }

  template <typename T,
            typename = std::enable_if_t<std::is_same_v<T, const char*> ||
                                        std::is_same_v<T, std::string> ||
                                        std::is_same_v<T, std::string_view>>>
  static jstring ProxyAsArg(T s) {
    if constexpr (std::is_same_v<T, const char*>) {
      return LifecycleHelper<jstring, LifecycleType::LOCAL>::Construct(s);
    } else {
      return LifecycleHelper<jstring, LifecycleType::LOCAL>::Construct(
          s.data());
    }
  }

  template <typename T,
            typename = std::enable_if_t<std::is_same_v<T, GlobalString> ||
                                        std::is_same_v<T, LocalString>>>
  static jstring ProxyAsArg(T& t) {
    return jstring{t};
  }

  template <typename T,
            typename = std::enable_if_t<std::is_same_v<T, GlobalString> ||
                                        std::is_same_v<T, LocalString>>>
  static jstring ProxyAsArg(T&& t) {
    return t.Release();
  }
};

}  // namespace jni

namespace jni {

template <typename Overload>
struct ArrayHelper;

template <typename SpanType, std::size_t kRank, const auto& class_v_,
          const auto& class_loader_v_, const auto& jvm_v_>
class LocalArray;

template <typename JArrayType>
struct Proxy<JArrayType, typename std::enable_if_t<
                             std::is_convertible_v<JArrayType, jarray>>>
    : public ProxyBase<JArrayType> {
  // Non-array primitive type (e.g. jintArray => jint).
  using CDecl = ArrayToRegularTypeMap_t<JArrayType>;

  // Primitive Array Types (e.g. if JArrayType is jintarray and T is too).
  template <typename ParamSelection, typename T, typename Enable = void>
  struct Helper {
    static constexpr bool val =
        (std::is_same_v<T, JArrayType> && ParamSelection::kRank == 1) ||
        (std::is_same_v<T, jobjectArray> && ParamSelection::kRank >= 2);
  };

  // LocalArray.
  template <typename ParamSelection, typename SpanType, std::size_t kRank,
            const auto& class_v_, const auto& class_loader_v_,
            const auto& jvm_v_>
  struct Helper<ParamSelection, LocalArray<SpanType, kRank, class_v_,
                                           class_loader_v_, jvm_v_>> {
    static constexpr auto param_copy = FullArrayStripV(ParamSelection::Val());

    static constexpr bool val =
        (kRank == ParamSelection::kRank) &&
        (std::is_same_v<SpanType, typename ParamSelection::RawValT> ||
         (std::is_same_v<SpanType, jobjectArray> &&
          ParamSelection::kRank >= 2) ||
         (std::string_view{class_v_.name_} == NameOrNothing_v<param_copy>));
  };

  template <typename ParamSelection, typename T>
  static constexpr bool kViable = Helper<ParamSelection, T>::val;

  using AsDecl = std::tuple<ArrayTag<JArrayType>>;
  using AsArg =
      std::tuple<JArrayType, RefBaseTag<JArrayType>, ArrayTag<JArrayType>>;

  template <typename Id>
  using AsReturn = typename ArrayHelper<Id>::AsReturn;

  static JArrayType ProxyAsArg(JArrayType arr) { return arr; };

  template <typename T>
  static JArrayType ProxyAsArg(const T& t) {
    return JArrayType{t};
  };

  template <typename T, typename = std::enable_if_t<
                            std::is_base_of_v<RefBaseTag<JArrayType>, T>>>
  static JArrayType ProxyAsArg(T&& t) {
    return t.Release();
  };
};

// This must be defined outside of Proxy so implicit definition doesn't occur.
template <typename IdT>
struct ArrayHelper {
  template <const auto& t>
  struct Helper {
    static constexpr auto val = FullArrayStripV(t.raw_);

    using StrippedCDecl = CDecl_t<std::decay_t<decltype(val)>>;
    using ConvertedCDecl = RegularToArrayTypeMap_t<StrippedCDecl>;
  };

  static constexpr auto kVal{IdT::Materialize()};

  static constexpr auto LocalArrayBuildFromArray() {
    using RawT = typename IdT::RawMaterializeT;
    constexpr std::size_t kRank = IdT::kMaterializedRank;

    if constexpr (!std::is_same_v<CDecl_t<RawT>, jobject>) {
      return LocalArray<RawT, kRank, kNoClassSpecified, kDefaultClassLoader,
                        kDefaultJvm>{1};
    } else {
      return LocalArray<jobject, kRank, Helper<kVal>::val, kDefaultClassLoader,
                        kDefaultJvm>{jobjectArray{nullptr}};
    }
  }

  using StrippedCDecl = typename Helper<kVal>::StrippedCDecl;
  using ConvertedCDecl = typename Helper<kVal>::ConvertedCDecl;

  using AsReturn = decltype(LocalArrayBuildFromArray());
};

}  // namespace jni

#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace jni {

template <typename TUndecayed>
struct ProxyHelper;

// Proxy is a metafunction that gives useful conversions from
// types and forwards to a corresponding type that's viable as input.
//
// Note, given the context, different types present differently.  E.g. a |jint|
// is always a jint, but a |jobject| is declared as a |jni::Class|, passed as a
// |jni::RefBase&| and then converted to a |jobject| to cross the C API.
//
// |Proxy_t| will select the correct proxy for any of the above types.  To be
// specific, |Proxy_t| of any type in |Arg| or |AsDecl| will return
// the parent Proxy.
//
// Each proxy exports aliases for a given |CDecl|.
//  |Index|: A uniquely identifying Key for proxy lookup.  This is usually the
//    CDecl (e.g. jint => jint), but rich types may differ (Object =>jobject).
//  |CDecl|: This is both the unique ID for a given proxy, as well as the
//    distinct type (of which there is only one) that is usable when invoking a
//    JNI call through the C API (e.g. |jint|, |jobject|).
//  |AsArg|: All valid passable types.
//  |AsDecl|: The type to be used in a function declaration, either as
//   return or as a declared argument. If is templated by |class_v| and
//   |class_loader_v| which can allow for additional decoration.
template <typename CDecl, typename Enable>
struct Proxy : public ProxyBase<CDecl> {};

template <typename VoidType>
struct Proxy<VoidType,
             typename std::enable_if_t<std::is_same_v<VoidType, void>>>
    : public ProxyBase<void> {};

template <typename CharType>
struct Proxy<CharType,
             typename std::enable_if_t<std::is_same_v<CharType, jchar>>>
    : public ProxyBase<jchar> {
  using AsArg = std::tuple<char, jchar>;
  using AsDecl = std::tuple<char, jchar>;

  template <typename OverloadSelection, typename T>
  static constexpr bool kViable = IsConvertibleKey<T>::template value<char> ||
                                  IsConvertibleKey<T>::template value<jchar>;
};

template <typename BooleanType>
struct Proxy<BooleanType,
             typename std::enable_if_t<std::is_same_v<BooleanType, jboolean>>>
    : public ProxyBase<jboolean> {
  using AsArg = std::tuple<jboolean, bool>;
  using AsDecl = std::tuple<jboolean, bool>;

  template <typename OverloadSelection, typename T>
  static constexpr bool kViable =
      IsConvertibleKey<T>::template value<jboolean> ||
      IsConvertibleKey<T>::template value<bool>;
};

template <typename LongType>
struct Proxy<LongType,
             typename std::enable_if_t<std::is_same_v<LongType, jlong>>>
    : public ProxyBase<jlong> {
  using AsArg = std::tuple<long, jlong>;
  using AsDecl = std::tuple<long, jlong>;

  template <typename OverloadSelection, typename T>
  static constexpr bool kViable = IsConvertibleKey<T>::template value<long> ||
                                  IsConvertibleKey<T>::template value<jlong>;

  static jlong ProxyAsArg(jlong val) { return val; }

  // jlong is a smaller type on ARM than x86.
  // When jlong is not equivalent, we upcast to the wider type.
  template <typename T,
            typename = std::enable_if_t<std::is_same_v<T, long> &&
                                        !std::is_same_v<jlong, long>>>
  static jlong ProxyAsArg(T val) {
    return jlong{val};
  }
};

////////////////////////////////////////////////////////////////////////////////
// Object Proxy Definitions.
////////////////////////////////////////////////////////////////////////////////
template <typename JObject>
struct Proxy<JObject,
             typename std::enable_if_t<std::is_same_v<JObject, jobject>>>
    : public ProxyBase<jobject> {
  using AsDecl = std::tuple<Object>;
  using AsArg = std::tuple<jobject, RefBaseTag<jobject>, LoaderTag>;

  template <typename InputParamSelectionT, typename T>
  struct ContextualViabilityHelper {
    // TODO(b/143908983): This is overly permissive, see method_selection_test.
    static constexpr bool kViable = std::is_same_v<T, jobject>;
  };

  // Old "LocalObject" form.
  template <typename IdT,
            template <const auto&, const auto&, const auto&> class Container,
            const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  struct ContextualViabilityHelper<IdT,
                                   Container<class_v, class_loader_v, jvm_v>> {
    static constexpr bool kViable =
        std::string_view{class_v.name_} == std::string_view{IdT::Val().name_};
  };

  // New "LocalObject" form.
  template <typename IdT, LifecycleType lifecycleType, const auto& class_v,
            const auto& class_loader_v, const auto& jvm_v>
  struct ContextualViabilityHelper<
      IdT, Scoped<lifecycleType, void,
                  JniT<jobject, class_v, class_loader_v, jvm_v>, jobject>> {
    static constexpr bool kViable =
        std::string_view{class_v.name_} == std::string_view{IdT::Val().name_};
  };

  template <typename IdT, typename T>
  static constexpr bool kViable = ContextualViabilityHelper<IdT, T>::kViable;

  template <typename Id>
  struct Helper {
    static constexpr auto kClass{Id::Val()};
    static constexpr auto kClassLoader{Id::JniT::GetClassLoader()};

    // TODO(b/174272629): Class loaders should also be enforced.
    using type = LocalObject<kClass, kClassLoader, kDefaultJvm>;
  };

  template <typename Id>
  using AsReturn = typename Helper<Id>::type;

  static jobject ProxyAsArg(jobject obj) { return obj; };

  // Applies for both local and global.
  template <typename T>
  static jobject ProxyAsArg(T& t) {
    return jobject{t};
  };

  // Applies for both local and global.
  template <typename T>
  static jobject ProxyAsArg(T&& t) {
    return t.Release();
  };
};

////////////////////////////////////////////////////////////////////////////////
// Self Proxy Definitions.
////////////////////////////////////////////////////////////////////////////////
template <typename SelfType>
struct Proxy<SelfType,
             typename std::enable_if_t<std::is_same_v<SelfType, Self>>>
    : public ProxyBase<jobject> {
  using AsDecl = std::tuple<Self>;
  using AsArg = std::tuple<Self>;

  template <typename IdT>
  using SelfIdT_t = typename IdT::template ChangeIdType<IdType::CLASS>;

  template <typename Id>
  struct Helper {
    static constexpr auto kClass{Id::Val()};
    static constexpr auto kClassLoader{Id::JniT::GetClassLoader()};

    // TODO(b/174272629): Class loaders should also be enforced.
    using type = LocalObject<kClass, kClassLoader, kDefaultJvm>;
  };

  template <typename Id>
  using AsReturn = typename Helper<Id>::type;

  template <typename IdT, typename T>
  static constexpr bool kViable =
      Proxy<jobject>::template kViable<SelfIdT_t<IdT>, T>;

  // Applies for both local and global.
  template <typename T>
  static jobject ProxyAsArg(T& t) {
    return jobject{t};
  };

  // Applies for both local and global.
  template <typename T>
  static jobject ProxyAsArg(T&& t) {
    return t.Release();
  };
};

}  // namespace jni

namespace jni {

// Adopts a local.
struct AdoptLocal {};

// Creates an additional reference to the underlying object.
// When used for local, presumes local, for global, presumes global.
struct NewRef {};

// This tag allows the constructor to promote underlying jobject for you.
struct PromoteToGlobal {};

// CAUTION: This tag assume the underlying jobject has been pinned as a global.
// This is atypical when solely using JNI Bind, use with caution.
struct AdoptGlobal {};

}  // namespace jni


#include <type_traits>
#include <vector>

namespace jni {

static inline jclass LoadClassFromObject(const char* name, jobject object_ref);

// Represents a a jclass instance for a specific class. 4 flavours exist:
//   1) Default JVM, default class loader.
//   2) Non-default JVM, default class loader.
//   3) Default JVM, non-default class loader.
//   4) Non-default JVM, non default class loader (i.e. fully specified).
//
// Use |ClassRef_t| to provide |JniT| in its minimal form.
template <typename JniT>
class ClassRef {
 public:
  static_assert(std::is_same_v<JniT, typename JniT::MinimallySpanningType>,
                "JniT must be in its minimal form for best caching.");

  template <typename Lambda>
  static void PrimeJClassFromClassLoader(Lambda lambda) {
    class_ref_.LoadAndMaybeInit(lambda);
  }

  static jclass GetAndMaybeLoadClassRef(
      jobject optional_object_to_build_loader_from) {
    // For the default classloader, storage in uniquely IDed struct static.
    if constexpr (JniT::GetClassLoader() == kDefaultClassLoader) {
      static auto get_lambda =
          [](metaprogramming::DoubleLockedValue<jclass>* storage) {
            DefaultRefs<jclass>().push_back(storage);

            // FindClass uses plain name (e.g. "kClass") for rank 0, qualified
            // class names when used in arrays (e.g. "[LkClass;"). This doesn't
            // come up in the API until rank 2.
            if constexpr (JniT::kRank <= 1) {
              return static_cast<jclass>(
                  LifecycleHelper<jobject, LifecycleType::GLOBAL>::Promote(
                      JniHelper::FindClass(JniT::kName.data())));
            } else {
              // Primitive types drop their rank by 1 because of how their
              // signatures get derived in array_ref.h.
              using JniTForLifecycle = std::conditional_t<
                  std::is_same_v<jobject, typename JniT::SpanType>, JniT,
                  typename JniT::RankLess1>;

              return static_cast<jclass>(
                  LifecycleHelper<jobject, LifecycleType::GLOBAL>::Promote(
                      JniHelper::FindClass(
                          SelectorStaticInfo<
                              JniTSelector<JniTForLifecycle, -1>>::TypeName()
                              .data())));
            }
          };

      return RefStorage<
          decltype(get_lambda),
          SelectorStaticInfo<JniTSelector<JniT, 0>>>::Get(get_lambda);
    } else {
      // For non default classloader, storage in class member.
      return class_ref_.LoadAndMaybeInit([=]() {
        return LoadClassFromObject(JniT::kNameWithDots.data(),
                                   optional_object_to_build_loader_from);
      });
    }
  }

  static jclass GetAlreadyLoadedClassRef() {
    return class_ref_.LoadAndMaybeInit([]() { return jclass{0}; });
  }

  static void MaybeReleaseClassRef() {
    class_ref_.Reset([](jclass maybe_loaded_class) {
      LifecycleHelper<jclass, LifecycleType::GLOBAL>::Delete(
          maybe_loaded_class);
    });
  }

 private:
  // A global reference to a jclass object that is returned from FindClass.
  // The variable has static storage because ClassIDs are static to the lifetime
  // of a JVM.  See GetAndMaybeLoadClassRef and MaybeReleaseClassRef.
  static inline metaprogramming::DoubleLockedValue<jclass> class_ref_;
};

// When we get an object_ref_ as a return value from a Java method, it may be
// an instance of a subclass of ClassRefT. In this case, if we directly used
// the object_ref_'s class, then we might incorrectly get member information
// for the subclass instead of the original class. However, the original class
// should still be loadable from the subclass's class loader, so we load the
// ClassRef explicitly by class name.
static inline jclass LoadClassFromObject(const char* name, jobject object_ref) {
  // We cannot refer to the wrapper MethodRefs here, so we just manually use
  // the class loader through JNI.

  // Gets the ClassLoader of java/lang/class (the primordial loader).
  // Note, these aren't static methods, they're member methods to be invoked
  // on the object's class itself.  The class may not have been loaded yet,
  // and all you have is a jobject
  jclass java_lang_class_jclass =
      ClassRef<JniT<jobject, kJavaLangClass>>::GetAndMaybeLoadClassRef(nullptr);

  jclass java_lang_class_loader_jclass =
      ClassRef<JniT<jobject, kJavaLangClassLoader>>::GetAndMaybeLoadClassRef(
          nullptr);

  jclass class_of_object_jclass = JniHelper::GetObjectClass(object_ref);

  jmethodID get_class_loader_jmethod = JniHelper::GetMethodID(
      java_lang_class_jclass, "getClassLoader", "()Ljava/lang/ClassLoader;");

  jobject object_ref_class_loader_jobject =
      InvokeHelper<jobject, 1, false>::Invoke(class_of_object_jclass, nullptr,
                                              get_class_loader_jmethod);

  jmethodID load_class_jmethod =
      JniHelper::GetMethodID(java_lang_class_loader_jclass, "loadClass",
                             "(Ljava/lang/String;)Ljava/lang/Class;");

  jstring name_string =
      LifecycleHelper<jstring, LifecycleType::LOCAL>::Construct(name);
  jobject local_jclass_of_correct_loader =
      InvokeHelper<jobject, 1, false>::Invoke(object_ref_class_loader_jobject,
                                              nullptr, load_class_jmethod,
                                              name_string);
  jobject promote_jclass_of_correct_loader =
      LifecycleHelper<jobject, LifecycleType::GLOBAL>::Promote(
          local_jclass_of_correct_loader);

  LifecycleHelper<jobject, LifecycleType::LOCAL>::Delete(
      object_ref_class_loader_jobject);

  return static_cast<jclass>(promote_jclass_of_correct_loader);
}

template <typename JniT>
using ClassRef_t = ClassRef<typename JniT::MinimallySpanningType>;

}  // namespace jni

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// "Unfurls" an index sequence onto a container, and provides convenience
// helpers
//
// |type| is a variadic metafunction that generates a tuple for each type
//  where each type is Container<Idx, Ts...> for [0, Is...).
//    i.e. std::tuple<Container<0, Ts...>, Container<1, Ts...>, ...>
//
// |val| is a variadic data member that is the fold of the val member of each
//   element of |type|. If |use_conjunction_fold_on_val| is true, the fold
//   operator will be &&, otherwise it will be ||.
template <template <std::size_t, typename...> class Container, std::size_t Is,
          bool use_conjunction_fold_on_val = true>
struct Unfurl {
  template <typename Ts, typename Indexes>
  struct Helper;

  template <typename... Ts, std::size_t... Indices>
  struct Helper<std::tuple<Ts...>, std::index_sequence<Indices...>> {
    using type = std::tuple<Container<Indices, Ts...>...>;

    static constexpr bool val = use_conjunction_fold_on_val
                                    ? (Container<Indices, Ts...>::val && ...)
                                    : (Container<Indices, Ts...>::val || ...);
  };

  template <typename... Ts>
  using type =
      typename Helper<std::tuple<Ts...>, std::make_index_sequence<Is>>::type;

  template <typename... Ts>
  static constexpr bool val =
      Helper<std::tuple<Ts...>, std::make_index_sequence<Is>>::val;
};

template <std::size_t Is, template <std::size_t, typename...> class Container,
          typename... Ts>
using Unfurl_t = typename Unfurl<Container, Is>::template type<Ts...>;

template <std::size_t Is, template <std::size_t, typename...> class Container,
          typename... Ts>
static constexpr bool UnfurlConjunction_v =
    Unfurl<Container, Is, true>::template val<Ts...>;

template <std::size_t Is, template <std::size_t, typename...> class Container,
          typename... Ts>
static constexpr bool UnfurlDisjunction_v =
    Unfurl<Container, Is, false>::template val<Ts...>;

}  // namespace jni::metaprogramming

#include <string_view>
#include <tuple>
#include <utility>

namespace jni::metaprogramming {

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::*nameable_member,
          typename IndexSequenceType>
class QueryableMapBase {};

// This is an interface that can be inherited from to expose an
// operator["name"]. It provides compile time string index lookup with no macros
// although it is dependent on a clang extension.
//
// To use this API, inherit from this class using template types as follows:
//
// |CrtpBase|: The name of the class inheriting from the map.  This class
//   will inherit an operator[].  It must implement this exact signature:
//
//    template <std::size_t I>
//    auto QueryableMapCall(const char* key);
//
// |tup_container_v| is a static instance of an object whose |nameable_member|
//   contains a public field called name_.  It might seem strange not to
//   directly pass a const auto&, however, this prevents accessing subobjects.
//
// The motivation for using inheritance as opposed to a simple member is that
// the the const char cannot be propagated without losing its constexpr-ness,
// and so the clang extension can no longer restrict function candidates.
template <typename CrtpBase, const auto& tup_container_v,
          std::size_t container_size, typename TupContainerT,
          const auto TupContainerT::*nameable_member>
class QueryableMap
    : public QueryableMapBase<CrtpBase, tup_container_v, TupContainerT,
                              nameable_member,
                              std::make_index_sequence<container_size>> {};

template <typename CrtpBase, const auto& tup_container_v,
          const auto std::decay_t<decltype(tup_container_v)>::*nameable_member>
using QueryableMap_t =
    QueryableMap<CrtpBase, tup_container_v,
                 std::tuple_size_v<std::decay_t<decltype((tup_container_v.*
                                                          nameable_member))>>,
                 std::decay_t<decltype(tup_container_v)>, nameable_member>;

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::*nameable_member,
          std::size_t I>
class QueryableMapEntry;

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::*nameable_member,
          std::size_t... idxs>
class QueryableMapBase<CrtpBase, tup_container_v, TupContainerT,
                       nameable_member, std::index_sequence<idxs...>>
    : public QueryableMapEntry<CrtpBase, tup_container_v, TupContainerT,
                               nameable_member, idxs>... {
 public:
  using QueryableMapEntry<CrtpBase, tup_container_v, TupContainerT,
                          nameable_member, idxs>::operator[]...;

  using QueryableMapEntry<CrtpBase, tup_container_v, TupContainerT,
                          nameable_member, idxs>::Contains...;

  // Will select subclass specialisations if present.
  constexpr bool Contains(const char* key) { return false; }
};

template <typename CrtpBase, const auto& tup_container_v,
          typename TupContainerT, const auto TupContainerT::*nameable_member,
          std::size_t I>
class QueryableMapEntry {
 public:
#if __clang__
  // This function blurs the distinction between type and value space.  The
  // clang extension allows the key to be wrapped in a constexpr way.  This
  // allows for string to string comparison based on the static value the class
  // is templated by.
  //
  // The reason the TypeMap interface requires inheritance as opposed to simply
  // holding an instance of this map (like you would with a regular hash map) is
  // the constexpr-ness of the string can't be propagated.  This essentially
  // means you get one shot at defining the function.
  constexpr auto operator[](const char* key) __attribute__((
      enable_if(std::string_view(key) ==
                    std::get<I>(tup_container_v.*nameable_member).name_,
                ""))) {
    static_assert(std::is_base_of_v<QueryableMapEntry, CrtpBase>,
                  "You must derive from the invocable map.");

    return (*static_cast<CrtpBase*>(this)).template QueryableMapCall<I>(key);
  }

  constexpr bool Contains(const char* key) __attribute__((
      enable_if(std::string_view(key) ==
                    std::get<I>(tup_container_v.*nameable_member).name_,
                ""))) {
    return true;
  }
#else
  static_assert(false,
                "This container requires clang for compile time strings.");
#endif
};

}  // namespace jni::metaprogramming

namespace jni::metaprogramming {

struct Max {
  template <typename T1, typename T2>
  struct Helper;

  template <const auto v1, const auto v2>
  struct Helper<Val<v1>, Val<v2>> {
    using type = std::conditional_t<(v1 > v2), Val<v1>, Val<v2>>;
  };

  template <typename T1, typename T2>
  using type = typename Helper<T1, T2>::type;
};

template <typename T1, typename T2>
using Max_t = typename Max::type<T1, T2>;

template <typename T1, typename T2>
static constexpr auto Max_v = Max_t<T1, T2>::val;

struct Min {
  template <typename T1, typename T2>
  struct Helper;

  template <const auto v1, const auto v2>
  struct Helper<Val<v1>, Val<v2>> {
    using type = std::conditional_t<(v1 < v2), Val<v1>, Val<v2>>;
  };

  template <typename T1, typename T2>
  using type = typename Helper<T1, T2>::type;
};

template <typename T1, typename T2>
using Min_t = typename Min::type<T1, T2>;

template <typename T1, typename T2>
static constexpr auto Min_v = Min_t<T1, T2>::val;

}  // namespace jni::metaprogramming

#include <tuple>

namespace jni::metaprogramming {

// Call takes a tuple of arguments and returns a tuple of elements
// dereferenced once (with no arguments). Eventually, this should have
// the pack discriminated forwarding (to optionally pass args).
struct Call {
  template <typename T>
  struct Helper;

  template <typename... Ts>
  struct Helper<std::tuple<Ts...>> {
    using type = std::tuple<typename Ts::type...>;
  };

  template <typename T>
  using type = typename Helper<T>::type;
};

template <typename T>
using Call_t = typename Call::type<T>;

}  // namespace jni::metaprogramming

#include <utility>

namespace jni {

////////////////////////////////////////////////////////////////////////////////
// Rank 0: Static primitive types (e.g. int).
////////////////////////////////////////////////////////////////////////////////
template <>
struct FieldHelper<jboolean, 0, true, void> {
  static inline jboolean GetValue(const jclass clazz,
                                  const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticBooleanField")), clazz,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jboolean>();
#else
    return jni::JniEnv::GetEnv()->GetStaticBooleanField(clazz, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jboolean&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticBooleanField")), clazz,
          field_ref_, value);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetStaticBooleanField(clazz, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jbyte, 0, true, void> {
  static inline jbyte GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticByteField")), clazz,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jbyte>();
#else
    return jni::JniEnv::GetEnv()->GetStaticByteField(clazz, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jbyte&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticByteField")), clazz,
          field_ref_, value);

#ifdef DRY_RUN
#else
    return jni::JniEnv::GetEnv()->SetStaticByteField(clazz, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jchar, 0, true, void> {
  static inline jchar GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticCharField")), clazz,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jchar>();
#else
    return jni::JniEnv::GetEnv()->GetStaticCharField(clazz, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jchar&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticCharField")), clazz,
          field_ref_, value);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetStaticCharField(clazz, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jshort, 0, true, void> {
  static inline jshort GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticShortField")), clazz,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jshort>();
#else
    return jni::JniEnv::GetEnv()->GetStaticShortField(clazz, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jshort&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticShortField")), clazz,
          field_ref_, value);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetStaticShortField(clazz, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jint, 0, true, void> {
  static inline jint GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticIntField")), clazz,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jint>();
#else
    return jni::JniEnv::GetEnv()->GetStaticIntField(clazz, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jint&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticIntField")), clazz,
          field_ref_, value);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetStaticIntField(clazz, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jlong, 0, true, void> {
  static inline jlong GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticLongField")), clazz,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jlong>();
#else
    return jni::JniEnv::GetEnv()->GetStaticLongField(clazz, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jlong&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticLongField")), clazz,
          field_ref_, value);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetStaticLongField(clazz, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jfloat, 0, true, void> {
  static inline jfloat GetValue(const jclass clazz, const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticFloatField")), clazz,
          field_ref_);

#ifdef DRY_RUN
    return 123.f;
#else
    return jni::JniEnv::GetEnv()->GetStaticFloatField(clazz, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jfloat&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticFloatField")), clazz,
          field_ref_, value);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetStaticFloatField(clazz, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jdouble, 0, true, void> {
  static inline jdouble GetValue(const jclass clazz,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticDoubleField")), clazz,
          field_ref_);

#ifdef DRY_RUN
    return 123.;
#else
    return jni::JniEnv::GetEnv()->GetStaticDoubleField(clazz, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jdouble&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticDoubleField")), clazz,
          field_ref_, value);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetStaticDoubleField(clazz, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jobject, 0, true, void> {
  static inline jobject GetValue(const jclass clazz,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticObjectField")), clazz,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return jni::JniEnv::GetEnv()->GetStaticObjectField(clazz, field_ref_);
#endif  // DRY_RUN
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jobject&& new_value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticObjectField")), clazz,
          field_ref_, new_value);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetStaticObjectField(clazz, field_ref_, new_value);
#endif  // DRY_RUN
  }
};

template <>
struct FieldHelper<jstring, 0, true, void> {
  static inline jstring GetValue(const jclass clazz,
                                 const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticObjectField")), clazz,
          field_ref_);

#ifdef DRY_RUN
    return Fake<jstring>();
#else
    return reinterpret_cast<jstring>(
        jni::JniEnv::GetEnv()->GetStaticObjectField(clazz, field_ref_));
#endif  // DRY_RUN
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jstring&& new_value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticObjectField")), clazz,
          field_ref_, new_value);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetStaticObjectField(clazz, field_ref_, new_value);
#endif  // DRY_RUN
  }
};

////////////////////////////////////////////////////////////////////////////////
// Rank 1: Static single dimension arrays (e.g. int[]).
////////////////////////////////////////////////////////////////////////////////
template <typename ArrayType>
struct StaticBaseFieldArrayHelper {
  static inline ArrayType GetValue(const jobject object_ref,
                                   const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectField")), object_ref,
          field_ref_);

#ifdef DRY_RUN
    return Fake<ArrayType>();
#else
    return static_cast<ArrayType>(
        jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_));
#endif  // DRY_RUN
  }

  static inline void SetValue(const jobject object_ref,
                              const jfieldID field_ref_, ArrayType&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectField")), object_ref,
          field_ref_, value);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, value);
#endif  // DRY_RUN
  }
};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jboolean>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jbooleanArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jbyte>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jbyteArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jchar>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jcharArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jshort>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jshortArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jint>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jintArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jlong>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jlongArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jfloat>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jfloatArray> {};

template <std::size_t kRank>
struct FieldHelper<std::enable_if_t<(kRank == 1), jdouble>, kRank, true, void>
    : StaticBaseFieldArrayHelper<jdoubleArray> {};

////////////////////////////////////////////////////////////////////////////////
// Rank 1: Static jobjects.
// Rank 2+: Static multi-dimension arrays (e.g. int[][], int[][][]).
////////////////////////////////////////////////////////////////////////////////
template <typename T, std::size_t kRank>
struct FieldHelper<
    T, kRank, true,
    std::enable_if_t<(std::is_same_v<jobject, T> || (kRank > 1))>> {
  static inline jobjectArray GetValue(const jclass clazz,
                                      const jfieldID field_ref_) {
    Trace(metaprogramming::LambdaToStr(STR("GetStaticObjectField, Rank 1+")),
          clazz, field_ref_);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return static_cast<jobjectArray>(
        jni::JniEnv::GetEnv()->GetStaticObjectField(clazz, field_ref_));
#endif  // DRY_RUN
  }

  static inline void SetValue(const jclass clazz, const jfieldID field_ref_,
                              jobjectArray&& value) {
    Trace(metaprogramming::LambdaToStr(STR("SetStaticObjectField, Rank 1+")),
          clazz, field_ref_, value);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetStaticObjectField(clazz, field_ref_, value);
#endif  // DRY_RUN
  }
};

}  // namespace jni

#include <string_view>
#include <type_traits>
#include <utility>

namespace jni {

// Transforms a OverloadRef IdT into a fully qualified ID. Storage is keyed
// against these IDs to reduce excess MethodID lookups.
template <typename IdT>
struct OverloadRefUniqueId {
  static constexpr std::string_view kDash = "#";
  static constexpr std::string_view kClassQualifier{IdT::Class().name_};
  static constexpr std::string_view kOverloadName{IdT::Name()};

  // IdT::Name will be the overload name (e.g. "Foo").
  // Dashes are solely for readability in debugging.
  static constexpr std::string_view TypeName() {
    return metaprogramming::StringConcatenate_v<
        kClassQualifier, kDash, kOverloadName, kDash, Signature_v<IdT>>;
  }
};

template <typename IdT_, IdType kReturnIDType>
struct OverloadRef {
  using IdT = IdT_;

  using ReturnIdT = typename IdT::template ChangeIdType<kReturnIDType>;
  using SelfIdT = typename IdT::template ChangeIdType<IdType::CLASS>;

  using ReturnProxied = std::conditional_t<
      ReturnIdT::kIsSelf,
      Return_t<typename SelfIdT::MaterializeCDeclT, SelfIdT>,
      Return_t<typename ReturnIdT::MaterializeCDeclT, ReturnIdT>>;

  static jmethodID GetMethodID(jclass clazz) {
    static auto get_lambda =
        [clazz](metaprogramming::DoubleLockedValue<jmethodID>* storage) {
          DefaultRefs<jmethodID>().push_back(storage);

          if constexpr (IdT::kIsStatic) {
            return jni::JniHelper::GetStaticMethodID(clazz, IdT::Name(),
                                                     Signature_v<IdT>.data());
          } else {
            return jni::JniHelper::GetMethodID(clazz, IdT::Name(),
                                               Signature_v<IdT>.data());
          }
        };

    return RefStorage<decltype(get_lambda), OverloadRefUniqueId<IdT>>::Get(
        get_lambda);
  }

  template <typename... Params>
  static ReturnProxied Invoke(jclass clazz, jobject object,
                              Params&&... params) {
    constexpr std::size_t kRank = ReturnIdT::kRank;
    constexpr bool kStatic = ReturnIdT::kIsStatic;
    const jmethodID mthd = OverloadRef::GetMethodID(clazz);

    if constexpr (std::is_same_v<ReturnProxied, void>) {
      return InvokeHelper<void, kRank, kStatic>::Invoke(
          object, clazz, mthd,
          Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...);
    } else if constexpr (IdT::kIsConstructor) {
      return ReturnProxied{
          AdoptLocal{},
          LifecycleHelper<jobject, LifecycleType::LOCAL>::Construct(
              clazz, mthd,
              Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...)};
    } else {
      if constexpr (std::is_base_of_v<RefBaseBase, ReturnProxied>) {
        return ReturnProxied{
            AdoptLocal{},
            InvokeHelper<typename ReturnIdT::CDecl, kRank, kStatic>::Invoke(
                object, clazz, mthd,
                Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...)};
      } else {
        return static_cast<ReturnProxied>(
            InvokeHelper<typename ReturnIdT::CDecl, kRank, kStatic>::Invoke(
                object, clazz, mthd,
                Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...));
      }
    }
  }
};

}  // namespace jni


namespace jni {

template <typename JniT, size_t field_idx_>
struct FieldSelection {
  using IdT = Id<JniT, IdType::FIELD, field_idx_>;
  static constexpr IdType kRetTypeId = IdType::FIELD;
};

}  // namespace jni

namespace jni::metaprogramming {

template <char sought>
struct StringContains {
  template <const std::string_view& str, typename IndexSequence>
  struct Helper;

  template <const std::string_view& str, std::size_t... Is>
  struct Helper<str, std::index_sequence<Is...>> {
    static constexpr bool val = ((str[Is] == sought) || ...);
  };

  template <const std::string_view& str>
  static constexpr bool val =
      Helper<str, std::make_index_sequence<str.length()>>::val;
};

template <const std::string_view& str, char sought>
static constexpr bool StringContains_v =
    StringContains<sought>::template val<str>;

}  // namespace jni::metaprogramming

namespace jni::metaprogramming {

enum class PackType {
  NOT_CONTAINER,
  TYPES,
  AUTO,
  AUTO_REF,
  CONST_AUTO_REF,
};

// Metafunction to discrimate the underlying pack type of a Container.
// Note: This interface is subject to change as the auto partial specialisations
// cannot discriminate on void.
struct PackDiscrimator {
  template <typename T>
  struct Helper {
    static constexpr PackType val = PackType::NOT_CONTAINER;
  };

  template <template <typename...> class Container, typename... Ts>
  struct Helper<Container<Ts...>> {
    static constexpr PackType val = PackType::TYPES;
  };

  template <template <auto...> class Container, auto... Vs>
  struct Helper<Container<Vs...>> {
    static constexpr PackType val = PackType::AUTO;
  };

  template <template <auto&...> class Container, auto&... Vs>
  struct Helper<Container<Vs...>> {
    static constexpr PackType val = PackType::AUTO_REF;
  };

  template <template <const auto&...> class Container, const auto&... Vs>
  struct Helper<Container<Vs...>> {
    static constexpr PackType val = PackType::CONST_AUTO_REF;
  };

  template <typename T>
  static constexpr PackType val = Helper<T>::val;
};

template <typename T>
static constexpr PackType PackDiscriminator_e =
    PackDiscrimator::template val<T>;

// Metafunction to forward a containerized pack to a compatible container.
template <template <template <typename...> class> class TypesContainer,
          template <template <auto...> class> class AutoContainer,
          template <template <const auto&...> class>
          class ConstAutoRefContainer>
struct PackDiscriminatedForward {
  template <typename T>
  struct Helper;

  template <template <typename...> class Container, typename... Ts>
  struct Helper<Container<Ts...>> {
    using type =
        typename TypesContainer<Container>::template type<Container<Ts...>>;
  };

  template <template <auto...> class Container, auto... vs>
  struct Helper<Container<vs...>> {
    using type =
        typename AutoContainer<Container>::template type<Container<vs...>>;
  };

  template <template <const auto&...> class Container, const auto&... vs>
  struct Helper<Container<vs...>> {
    using type = typename ConstAutoRefContainer<Container>::template type<
        Container<vs...>>;
  };

  template <typename T>
  using type = typename Helper<T>::type;
};

}  // namespace jni::metaprogramming

#include <limits>
#include <type_traits>
#include <utility>

namespace jni {

// Viablility helper for an exact parameter.
template <std::size_t I, typename IdT, typename ValkReturnIDType,
          typename... Ts>
struct Viable {
  static constexpr IdType kReturnIDType = ValkReturnIDType::val;
  using IdTmp = typename IdT::template ChangeIdType<kReturnIDType>;
  using IdTParamType = typename IdTmp::template ChangeIdx<2, I>;

  static constexpr bool val =
      Proxy_t<typename IdTParamType::UnstrippedRawVal>::template kViable<
          IdTParamType,
          metaprogramming::TypeOfNthElement_t<I, std::decay_t<Ts>...>>;
};

template <typename OverloadId, IdType kReturnIDType>
struct ArgumentValidate {
  // Helper to prevents instantiating mismatching size unrolls.
  template <typename... Ts>
  static constexpr bool ViableHelper() {
    if constexpr (sizeof...(Ts) == OverloadId::kNumParams) {
      return metaprogramming::UnfurlConjunction_v<
          OverloadId::kNumParams, Viable, OverloadId,
          metaprogramming::Val_t<kReturnIDType>, Ts...>;
    } else {
      return false;
    }
  }

  template <typename... Ts>
  static constexpr bool kValid = ViableHelper<Ts...>();
};

template <typename IdT_, IdType kReturnIDType>
struct OverloadSelection {
  using IdT = IdT_;

  template <typename... Ts>
  static constexpr bool OverloadViable() {
    return ArgumentValidate<IdT, kReturnIDType>::template kValid<Ts...>;
  }

  template <typename... Ts>
  static constexpr size_t OverloadIdxIfViable() {
    return OverloadViable<Ts...>() ? IdT::kSecondaryIdx : kNoIdx;
  }
};

template <typename IdT_, IdType kIDType = IdType::OVERLOAD,
          IdType kReturnIDType = IdType::OVERLOAD_PARAM>
struct MethodSelection {
  using IdT = IdT_;
  using JniT = typename IdT::JniT;

  template <std::size_t I, typename... Ts>
  struct Helper {
    using type = metaprogramming::Val_t<OverloadSelection<
        Id<JniT, kIDType, IdT::kIdx, I>,
        kReturnIDType>::template OverloadIdxIfViable<Ts...>()>;
  };

  template <typename... Ts>
  static constexpr std::size_t kIdxForTs = metaprogramming::ReduceAsPack_t<
      metaprogramming::Min, metaprogramming::Call_t<metaprogramming::Unfurl_t<
                                IdT::NumParams(), Helper, Ts...>>>::val;

  template <typename... Ts>
  using FindOverloadSelection =
      OverloadSelection<Id<JniT, kIDType, IdT::kIdx, kIdxForTs<Ts...>>,
                        kReturnIDType>;

  template <typename... Ts>
  static constexpr bool ArgSetViable() {
    return kIdxForTs<Ts...> != kNoIdx;
  }
};

template <typename IdT, IdType kIDType, IdType kReturnIDType, typename... Args>
struct OverloadSelector {
  using OverloadSelectionForArgs = typename MethodSelection<
      IdT, kIDType, kReturnIDType>::template FindOverloadSelection<Args...>;

  using OverloadRef =
      OverloadRef<Id<typename IdT::JniT, kIDType, IdT::kIdx,
                     OverloadSelectionForArgs::IdT::kSecondaryIdx>,
                  kReturnIDType>;

  static constexpr bool kIsValidArgSet =
      MethodSelection<IdT, kIDType,
                      kReturnIDType>::template ArgSetViable<Args...>();
};

}  // namespace jni

#include <mutex>
#include <string_view>
#include <tuple>
#include <utility>

namespace jni {

// See JvmRef::~JvmRef.
static inline auto& GetDefaultLoadedFieldList() {
  static auto* ret_val =
      new std::vector<metaprogramming::DoubleLockedValue<jfieldID>*>{};
  return *ret_val;
}

// Represents a live instance of Field I's definition.
//
// Note, this class performs no cleanup on destruction.  jFieldIDs are static
// throughout the duration of a JVM's life, see JvmRef for teardown.
template <typename JniT, IdType field_type, std::size_t I>
class FieldRef {
 public:
  using IdT = Id<JniT, field_type, I>;
  using FieldSelectionT = FieldSelection<JniT, I>;

  explicit FieldRef(jclass class_ref, jobject object_ref)
      : class_ref_(class_ref), object_ref_(object_ref) {}

  FieldRef(const FieldRef&) = delete;
  FieldRef(const FieldRef&&) = delete;
  void operator=(const FieldRef&) = delete;

  // This method is thread safe.
  static jfieldID GetFieldID(jclass clazz) {
    static jni::metaprogramming::DoubleLockedValue<jfieldID> return_value;

    return return_value.LoadAndMaybeInit([=]() {
      if constexpr (JniT::class_loader_v == kDefaultClassLoader) {
        GetDefaultLoadedFieldList().push_back(&return_value);
      }

      if constexpr (IdT::kIsStatic) {
        return jni::JniHelper::GetStaticFieldID(clazz, IdT::Name(),
                                                Signature_v<IdT>.data());
      } else {
        return jni::JniHelper::GetFieldID(clazz, IdT::Name(),
                                          Signature_v<IdT>.data());
      }
    });
  }

  using ReturnProxied = Return_t<typename IdT::MaterializeCDeclT, IdT>;

  const auto& SelfVal() {
    if constexpr (IdT::kIsStatic) {
      return class_ref_;
    } else {
      return object_ref_;
    }
  }

  ReturnProxied Get() {
    if constexpr (std::is_base_of_v<RefBaseBase, ReturnProxied>) {
      return {AdoptLocal{},
              FieldHelper<CDecl_t<typename IdT::RawValT>, IdT::kRank,
                          IdT::kIsStatic>::GetValue(SelfVal(),
                                                    GetFieldID(class_ref_))};
    } else {
      return {FieldHelper<CDecl_t<typename IdT::RawValT>, IdT::kRank,
                          IdT::kIsStatic>::GetValue(SelfVal(),
                                                    GetFieldID(class_ref_))};
    }
  }

  template <typename T>
  void Set(T&& value) {
    FieldHelper<CDecl_t<typename IdT::RawValT>, IdT::kRank,
                IdT::kIsStatic>::SetValue(SelfVal(), GetFieldID(class_ref_),
                                          Proxy_t<T>::ProxyAsArg(
                                              std::forward<T>(value)));
  }

 private:
  const jclass class_ref_;
  const jobject object_ref_;
};

}  // namespace jni

#include <type_traits>

namespace jni::metaprogramming {

template <typename T1>
struct DeepEqualDiminished;

template <typename T1, typename T2>
static constexpr bool DeepEqualDiminished_v =
    DeepEqualDiminished<T1>::template val<T2>;

template <typename Container1>
struct TypesEqualDiminished;

template <typename Container1_>
struct TypesEqualDiminished {
  template <typename Container1, typename Container2>
  struct Helper;

  template <template <typename...> class Container1,
            template <typename...> class Container2, typename... T1s,
            typename... T2s>
  struct Helper<Container1<T1s...>, Container2<T2s...>> {
    static constexpr bool Val() {
      if constexpr (sizeof...(T1s) == sizeof...(T2s)) {
        // Containers are interchangeable (i.e. the same).
        if constexpr (std::is_same_v<Container1<T1s...>, Container2<T1s...>>) {
          return (DeepEqualDiminished_v<T1s, T2s> && ...);
        } else {
          return false;
        }
      } else {
        return false;
      }
    }

    static constexpr bool val = Val();
  };

  template <typename Container2>
  static constexpr bool val = Helper<Container1_, Container2>::val;
};

// Identical to `DeepEqual` except using diminished value comparison.
// This is to obviate an MSVC compiler bug (see `ValsEqualDiminished`).
template <typename T1>
struct DeepEqualDiminished {
  template <PackType pack_type_1, PackType pack_type_2>
  static constexpr bool Compatible() {
    if constexpr (pack_type_1 == PackType::NOT_CONTAINER ||
                  pack_type_2 == PackType::NOT_CONTAINER) {
      // Non-containers only with non-containers.
      return pack_type_1 == PackType::NOT_CONTAINER &&
             pack_type_2 == PackType::NOT_CONTAINER;
    } else if constexpr (pack_type_1 == PackType::TYPES ||
                         pack_type_2 == PackType::TYPES) {
      // Type packs only with other type packs.
      return pack_type_1 == PackType::TYPES && pack_type_2 == PackType::TYPES;
    } else {
      // All the auto pack types.
      return true;
    }
  }

  template <typename T2>
  static constexpr bool Val() {
    const PackType pack_type_1 = PackDiscriminator_e<T1>;
    const PackType pack_type_2 = PackDiscriminator_e<T2>;

    if constexpr (!Compatible<pack_type_1, pack_type_2>()) {
      return false;
    } else if constexpr (PackDiscriminator_e<T1> == PackType::NOT_CONTAINER) {
      return std::is_same_v<T1, T2>;
    } else if constexpr (PackDiscriminator_e<T1> == PackType::TYPES) {
      return TypesEqualDiminished<T1>::template val<T2>;
    } else {
      return ValsEqualDiminished_v<T1, T2>;
    }

    return true;
  }

  template <typename T2>
  static constexpr bool val = Val<T2>();
};

}  // namespace jni::metaprogramming

#include <cstddef>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>

namespace jni {

// Represents a runtime instance of a JNI Object.  Instead of using this class
// directly, instead the more specialised types such as LocalObject,
// GlobalObject, etc.
//
// To call methods on the object, use the  operator(), to access fields, use
// operator[].
template <typename JniT>
class ObjectRef
    : public metaprogramming::InvocableMap<
          ObjectRef<JniT>, JniT::stripped_class_v, typename JniT::ClassT,
          &JniT::ClassT::methods_>,
      public metaprogramming::QueryableMap_t<
          ObjectRef<JniT>, JniT::stripped_class_v, &JniT::ClassT::fields_>,
      public RefBase<JniT> {
 protected:
  static_assert(
      JniT::class_loader_v
          .template SupportedDirectlyOrIndirectly<JniT::class_v>(),
      "This class is not directly or indirectly supported by this loader.");
  static_assert(!metaprogramming::StringContains_v<JniT::kName, '.'>,
                "Use '/', not '.' in class names (for maximum) portability.");

  using RefBase = RefBase<JniT>;

  ObjectRef() = delete;
  ObjectRef& operator=(const ObjectRef& rhs) = delete;

  jclass GetJClass() const {
    // jobject != jstring but both derive from this class.
    // Args are passed to recover class ref from class loader, and nullptr is
    // always safe. `GetAndMaybeLoadClassRef` requires jobject, so using
    // `RefBase::object_ref_` below obiates ubsan failures.
    if constexpr (std::is_same_v<typename JniT::SpanType, jobject>) {
      return ClassRef_t<JniT>::GetAndMaybeLoadClassRef(RefBase::object_ref_);
    } else {
      return ClassRef_t<JniT>::GetAndMaybeLoadClassRef(nullptr);
    }
  }

 public:
  ObjectRef(std::nullptr_t) : RefBase(nullptr) {}

  explicit ObjectRef(RefBaseTag<typename JniT::StorageType>&& rhs)
      : RefBase(std::move(rhs)) {}

  // Invoked through CRTP from InvocableMap.
  template <size_t I, typename... Args>
  auto InvocableMapCall(const char* key, Args&&... args) const {
    using IdT = Id<JniT, IdType::OVERLOAD_SET, I>;
    using MethodSelectionForArgs =
        OverloadSelector<IdT, IdType::OVERLOAD, IdType::OVERLOAD_PARAM,
                         Args...>;

    static_assert(MethodSelectionForArgs::kIsValidArgSet,
                  "JNI Error: Invalid argument set.");

    return MethodSelectionForArgs::OverloadRef::Invoke(
        GetJClass(), RefBase::object_ref_, std::forward<Args>(args)...);
  }

  // Invoked through CRTP from QueryableMap.
  template <size_t I>
  auto QueryableMapCall(const char* key) const {
    return FieldRef<JniT, IdType::FIELD, I>{GetJClass(), RefBase::object_ref_};
  }
};

// Imbues constructors for ObjectRefs and handles calling the correct
// intermediate constructors.  Access to this class is constrainted for non
// default classloaders (see |ValidatorProxy|).
template <typename JniT>
class ConstructorValidator : public ObjectRef<JniT> {
 public:
  using Base = ObjectRef<JniT>;
  using Base::Base;

  // Objects can still be wrapped.  This could happen if a classloaded object
  // is built in Java and then passed through to JNI.
  ConstructorValidator(typename JniT::StorageType obj) : Base(obj) {}

  template <LifecycleType, const auto& jvm_v, const auto& class_loader_v>
  friend class ClassLoaderRef;

  static constexpr std::size_t kNumConstructors =
      std::tuple_size_v<decltype(JniT::class_v.constructors_)>;

  template <typename... Args>
  struct Helper {
    using IdT = Id<JniT, IdType::OVERLOAD_SET, kNoIdx>;

    // 0 is (always) used to represent the constructor.
    using type = OverloadSelector<IdT, IdType::OVERLOAD, IdType::OVERLOAD_PARAM,
                                  Args...>;
  };

  template <typename... Args>
  using Permutation_t = typename Helper<Args...>::type;

  template <typename... Args,
            typename std::enable_if<(sizeof...(Args) != 0 && JniT::kRank == 0),
                                    int>::type = 0>
  ConstructorValidator(Args&&... args)
      : Base(static_cast<typename JniT::StorageType>(
            Permutation_t<Args...>::OverloadRef::Invoke(
                Base::GetJClass(), Base::object_ref_,
                std::forward<Args>(args)...)
                .Release())) {
    static_assert(Permutation_t<Args...>::kIsValidArgSet,
                  "You have passed invalid arguments to construct this type.");
  }

  ConstructorValidator()
      : Base(Permutation_t<>::OverloadRef::Invoke(Base::GetJClass(),
                                                  Base::object_ref_)
                 .Release()) {}
};

// Forward declaration for constructor validator (ctor augmentations).
template <typename JniT>
struct ValidatorProxy : public ConstructorValidator<JniT> {
  using Base = ConstructorValidator<JniT>;
  using Base::Base;
};

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
using ObjectRefBuilder_t =
    ValidatorProxy<JniT<jobject, class_v_, class_loader_v_, jvm_v_>>;

}  // namespace jni

#include <type_traits>

namespace jni {

// Marks the end of `ScopeEntry` daisy chain.
struct ScopedTerminalTag {};

// Shared implementation common to all *local* `Entry`.
template <typename Base, LifecycleType lifecycleType, typename JniT,
          typename ViableSpan>
struct EntryBase : public Base {
  using Base::Base;
  using Span = typename JniT::SpanType;

  // `RefBaseTag` move constructor for object of same span type.
  template <typename T,
            typename = std::enable_if_t<
                (::jni::metaprogramming::DeepEqualDiminished_v<EntryBase, T> ||
                 std::is_base_of_v<RefBaseTag<Span>, T>)>>
  EntryBase(T&& rhs) : Base(rhs.Release()) {}
  EntryBase(AdoptLocal, ViableSpan object) : Base(object) {}

  // "Copy" constructor: Additional reference to object will be created.
  EntryBase(NewRef, ViableSpan object)
      : EntryBase(AdoptLocal{},
                  object
                      ? static_cast<Span>(
                            LifecycleHelper<Span, lifecycleType>::NewReference(
                                static_cast<Span>(object)))
                      : nullptr) {}

  // Comparison operator for pinned Scoped type (not deep equality).
  template <typename T, typename = std::enable_if_t<
                            (std::is_base_of_v<RefBaseTag<Span>, T> ||
                             std::is_same_v<T, ViableSpan>)>>
  bool operator==(const T& rhs) const {
    if constexpr (std::is_base_of_v<RefBaseTag<Span>, T>) {
      return static_cast<Span>(rhs.object_ref_) == Base::object_ref_;
    } else if constexpr (std::is_same_v<T, ViableSpan>) {
      return rhs == Base::object_ref_;
    }
  }

  // Comparison inequality operator for pinned Scoped type (not deep equality).
  template <typename T, typename = std::enable_if_t<
                            (std::is_base_of_v<RefBaseTag<Span>, T> ||
                             std::is_same_v<T, ViableSpan>)>>
  bool operator!=(const T& rhs) const {
    return !(*this == rhs);
  }
};

// Local scoped entry augmentation.
template <LifecycleType lifecycleType, typename JniT, typename ViableSpan,
          typename... ViableSpans>
struct Entry
    : public EntryBase<Entry<LifecycleType::LOCAL, JniT, ViableSpans...>,
                       LifecycleType::LOCAL, JniT, ViableSpan> {
  using Base = EntryBase<Entry<LifecycleType::LOCAL, JniT, ViableSpans...>,
                         LifecycleType::LOCAL, JniT, ViableSpan>;
  using Base::Base;

  // "Wrap" constructor: Newly created object released at end of scope.
  // Wrap constructors automatically create a new local because objects passed
  // into JNI should not be released, and LocalObject<KClass>(jni_arg) is
  // common.
  Entry(ViableSpan object)
      : Base(AdoptLocal{},
             object ? LifecycleHelper<typename JniT::StorageType,
                                      LifecycleType::LOCAL>::
                          NewReference(
                              static_cast<typename JniT::StorageType>(object))
                    : nullptr) {}

  Entry(AdoptLocal, ViableSpan object)
      : Base(AdoptLocal{}, static_cast<typename JniT::StorageType>(object)) {}
};

// Shared implementation common to all *global* `Entry`.
template <typename Base, typename JniT, typename ViableSpan>
struct EntryBase<Base, LifecycleType::GLOBAL, JniT, ViableSpan> : public Base {
  using Base::Base;
  using Span = typename JniT::SpanType;

  // `RefBaseTag` move constructor for object of same span type.
  template <typename T,
            typename = std::enable_if_t<
                (::jni::metaprogramming::DeepEqualDiminished_v<EntryBase, T> ||
                 std::is_base_of_v<RefBaseTag<Span>, T>)>>
  EntryBase(T&& rhs)
      : Base(LifecycleHelper<typename JniT::StorageType,
                             LifecycleType::GLOBAL>::Promote(rhs.Release())) {}

  // "Copy" constructor: Additional reference to object will be created.
  EntryBase(NewRef, ViableSpan object)
      : Base(static_cast<Span>(
            LifecycleHelper<Span, LifecycleType::GLOBAL>::NewReference(
                static_cast<Span>(object)))) {}
};

// Global scoped entry augmentation.
template <typename JniT, typename ViableSpan, typename... ViableSpans>
struct Entry<LifecycleType::GLOBAL, JniT, ViableSpan, ViableSpans...>
    : public EntryBase<Entry<LifecycleType::GLOBAL, JniT, ViableSpans...>,
                       LifecycleType::GLOBAL, JniT, ViableSpan> {
  using Base = EntryBase<Entry<LifecycleType::GLOBAL, JniT, ViableSpans...>,
                         LifecycleType::GLOBAL, JniT, ViableSpan>;
  using Base::Base;

  // "Promote" constructor: Creates new global, frees |obj| (standard).
  explicit Entry(PromoteToGlobal, ViableSpan obj)
      : Base(LifecycleHelper<typename JniT::StorageType,
                             LifecycleType::GLOBAL>::Promote(obj)) {}

  // "Adopts" a global (non-standard).
  explicit Entry(AdoptGlobal, ViableSpan obj) : Base(obj) {}

 protected:
  // Causes failure for illegal "wrap" like construction.
  explicit Entry(ViableSpan object)
      : Base(reinterpret_cast<typename JniT::SpanType>(object)) {}
};

// Terminal Entry (ends daisy chain).
template <typename JniT>
struct Entry<LifecycleType::LOCAL, JniT, ScopedTerminalTag>
    : public ValidatorProxy<JniT> {
  using Base = ValidatorProxy<JniT>;
  using Base::Base;
};

template <typename JniT>
struct Entry<LifecycleType::GLOBAL, JniT, ScopedTerminalTag>
    : public ValidatorProxy<JniT> {
  using Base = ValidatorProxy<JniT>;
  using Base::Base;
};

// Local augmentation.
template <LifecycleType lifecycleType, typename JniT, typename... ViableSpans>
struct Scoped
    : public Entry<lifecycleType, JniT, ViableSpans..., ScopedTerminalTag> {
  using Base = Entry<lifecycleType, JniT, ViableSpans..., ScopedTerminalTag>;
  using Base::Base;

 protected:
  void MaybeReleaseUnderlyingObject() {
    if (Base::object_ref_) {
      LifecycleHelper<typename JniT::StorageType, lifecycleType>::Delete(
          Base::object_ref_);
    }
  }

 public:
  ~Scoped() { MaybeReleaseUnderlyingObject(); }
};

template <LifecycleType lifecycleType, typename JniT, typename... ViableSpans>
Scoped(Scoped<lifecycleType, JniT, ViableSpans...>)
    -> Scoped<lifecycleType, JniT, ViableSpans...>;

}  // namespace jni

#include <optional>
#include <string>
#include <string_view>

namespace jni {

template <typename CrtpBase>
class StringRefBase
    : public ConstructorValidator<
          JniT<jstring, kJavaLangString, kDefaultClassLoader, kDefaultJvm>> {
 public:
  using ValidT = ConstructorValidator<
      JniT<jstring, kJavaLangString, kDefaultClassLoader, kDefaultJvm>>;
  using ValidT::ValidT;

  StringRefBase(std::nullptr_t) : ValidT(jstring{nullptr}) {}
  StringRefBase(jstring object) : ValidT(object) {}

  ~StringRefBase() {
    if (object_ref_) {
      static_cast<CrtpBase&>(*this).ClassSpecificDeleteObjectRef(object_ref_);
    }
  }
};

// Represents a UTF view into a jstring (see jni::String::Pin()).
//
// This class will immediately pin memory associated with the jstring, and
// release on leaving scope. Note, this class will *always* make an expensive
// copy, as strings are natively represented in Java as Unicode.
//
// (C++20 will offer a compatible std::string_view but C++17 does not).
class UtfStringView {
 public:
  UtfStringView(jstring java_string)
      : java_string_(java_string),
        chars_(java_string_ ? JniHelper::GetStringUTFChars(java_string)
                            : nullptr) {}

  ~UtfStringView() {
    if (chars_) {
      JniHelper::ReleaseStringUTFChars(java_string_, chars_);
    }
  }

  UtfStringView(UtfStringView&&) = delete;
  UtfStringView(const UtfStringView&) = delete;

  // Returns a view into the pinned character string.
  // Warning: std::string_view{nullptr} is undefined behaviour and may crash.
  const std::string_view ToString() const { return std::string_view{chars_}; }

 private:
  const jstring java_string_;
  const char* chars_;
};

}  // namespace jni

#include <type_traits>

namespace jni {

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
using LocalObjectImpl =
    Scoped<LifecycleType::LOCAL,
           JniT<jobject, class_v_, class_loader_v_, jvm_v_>, jobject>;

template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class LocalObject : public LocalObjectImpl<class_v_, class_loader_v_, jvm_v_> {
 public:
  using Base = LocalObjectImpl<class_v_, class_loader_v_, jvm_v_>;
  using Base::Base;

  template <typename T>
  LocalObject(ArrayViewHelper<T> array_view_helper)
      : LocalObject(AdoptLocal{}, array_view_helper.val_) {}

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  LocalObject(LocalObject<class_v, class_loader_v, jvm_v>&& obj)
      : Base(AdoptLocal{}, obj.Release()) {}
};

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
LocalObject(LocalObject<class_v_, class_loader_v_, jvm_v_>&&)
    -> LocalObject<class_v_, class_loader_v_, jvm_v_>;

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
bool operator==(const jobject& lhs,
                const LocalObject<class_v_, class_loader_v_, jvm_v_>& rhs) {
  return lhs == static_cast<jobject>(rhs);
}

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
bool operator!=(const jobject& lhs,
                const LocalObject<class_v_, class_loader_v_, jvm_v_>& rhs) {
  return !(lhs == rhs);
}

}  // namespace jni

#include <type_traits>

namespace jni {

struct JniArrayHelperBase {
  static inline std::size_t GetLength(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayLength")), array);

#ifdef DRY_RUN
    return Fake<std::size_t>();
#else
    return jni::JniEnv::GetEnv()->GetArrayLength(array);
#endif  // DRY_RUN
  }
};

// Rank 2+ arrays all behave like object arrays.
template <typename SpannedType, std::size_t kRank>
struct JniArrayHelper : public JniArrayHelperBase {
  using AsArrayType = jobjectArray;

  static inline jobjectArray NewArray(std::size_t size,
                                      jclass class_id = nullptr,
                                      jobject initial_element = nullptr) {
    Trace(metaprogramming::LambdaToStr(STR("NewObjectArray")), size, class_id,
          initial_element);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return jni::JniEnv::GetEnv()->NewObjectArray(size, class_id,
                                                 initial_element);
#endif  // DRY_RUN
  }

  // The API of fetching objects only permits accessing one object at a time.
  static inline jobject GetArrayElement(jobjectArray array, std::size_t idx) {
    Trace(metaprogramming::LambdaToStr(STR("GetObjectArrayElement")), array,
          idx);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return jni::JniEnv::GetEnv()->GetObjectArrayElement(array, idx);
#endif  // DRY_RUN
  };

  // The API of fetching objects only permits accessing one object at a time.
  static inline void SetArrayElement(jobjectArray array, std::size_t idx,
                                     SpannedType obj) {
    Trace(metaprogramming::LambdaToStr(STR("SetObjectArrayElement")), array,
          idx, obj);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectArrayElement(array, idx, obj);
#endif  // DRY_RUN
  };
};

template <>
struct JniArrayHelper<jboolean, 1> : public JniArrayHelperBase {
  using AsArrayType = jbooleanArray;

  static inline jbooleanArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewBooleanArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jbooleanArray>();
#else
    return jni::JniEnv::GetEnv()->NewBooleanArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jboolean> GetArrayElements(
      jarray array) {
    Trace(
        metaprogramming::LambdaToStr(STR("GetArrayElements, jboolean, Rank 1")),
        array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jboolean>{};
#else
    GetArrayElementsResult<jboolean> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetBooleanArrayElements(
        static_cast<jbooleanArray>(array), &return_value.is_copy);

    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jboolean* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jboolean, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseBooleanArrayElements(
        static_cast<jbooleanArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jbyte, 1> : public JniArrayHelperBase {
  using AsArrayType = jbyteArray;

  static inline jbyteArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewByteArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jbyteArray>();
#else
    return jni::JniEnv::GetEnv()->NewByteArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jbyte> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jbyte, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jbyte>{};
#else
    GetArrayElementsResult<jbyte> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetByteArrayElements(
        static_cast<jbyteArray>(array), &return_value.is_copy);

    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jbyte* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jbyte, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseByteArrayElements(
        static_cast<jbyteArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jchar, 1> : public JniArrayHelperBase {
  using AsArrayType = jcharArray;

  static inline jcharArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewCharArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jcharArray>();
#else
    return jni::JniEnv::GetEnv()->NewCharArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jchar> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jchar, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jchar>{};
#else
    GetArrayElementsResult<jchar> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetCharArrayElements(
        static_cast<jcharArray>(array), &return_value.is_copy);

    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jchar* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jchar, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseCharArrayElements(
        static_cast<jcharArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jshort, 1> : public JniArrayHelperBase {
  using AsArrayType = jshortArray;

  static inline jshortArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewShortArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jshortArray>();
#else
    return jni::JniEnv::GetEnv()->NewShortArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jshort> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jshort, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jshort>{};
#else
    GetArrayElementsResult<jshort> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetShortArrayElements(
        static_cast<jshortArray>(array), &return_value.is_copy);

    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jshort* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jshort, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseShortArrayElements(
        static_cast<jshortArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jint, 1> : public JniArrayHelperBase {
  using AsArrayType = jintArray;

  static inline jintArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewIntArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jintArray>();
#else
    return jni::JniEnv::GetEnv()->NewIntArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jint> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jint, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jint>{};
#else
    GetArrayElementsResult<jint> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetIntArrayElements(
        static_cast<jintArray>(array), &return_value.is_copy);

    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, int* native_ptr,
                                          bool copy_on_completion) {
    Trace(
        metaprogramming::LambdaToStr(STR("ReleaseArrayElements, jint, Rank 1")),
        array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseIntArrayElements(
        static_cast<jintArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jlong, 1> : public JniArrayHelperBase {
  using AsArrayType = jlongArray;

  static inline jlongArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewLongArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jlongArray>();
#else
    return jni::JniEnv::GetEnv()->NewLongArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jlong> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jlong, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jlong>{};
#else
    GetArrayElementsResult<jlong> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetLongArrayElements(
        static_cast<jlongArray>(array), &return_value.is_copy);
    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jlong* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jlong, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseLongArrayElements(
        static_cast<jlongArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

template <>
struct JniArrayHelper<jfloat, 1> : public JniArrayHelperBase {
  using AsArrayType = jfloatArray;

  static inline jfloatArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewFloatArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jfloatArray>();
#else
    return jni::JniEnv::GetEnv()->NewFloatArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jfloat> GetArrayElements(jarray array) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElements, jfloat, Rank 1")),
          array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jfloat>{};
#else
    GetArrayElementsResult<jfloat> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetFloatArrayElements(
        static_cast<jfloatArray>(array), &return_value.is_copy);
    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jfloat* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jfloat, Rank 1")),
          array, native_ptr, copy_on_completion);

    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseFloatArrayElements(
        static_cast<jfloatArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jdouble, 1> : public JniArrayHelperBase {
  using AsArrayType = jdoubleArray;

  static inline jdoubleArray NewArray(std::size_t size) {
    Trace(metaprogramming::LambdaToStr(STR("NewDoubleArray, Rank 1")), size);

#ifdef DRY_RUN
    return Fake<jdoubleArray>();
#else
    return jni::JniEnv::GetEnv()->NewDoubleArray(size);
#endif  // DRY_RUN
  }

  static inline GetArrayElementsResult<jdouble> GetArrayElements(jarray array) {
    Trace(
        metaprogramming::LambdaToStr(STR("GetArrayElements, jdouble, Rank 1")),
        array);

#ifdef DRY_RUN
    return GetArrayElementsResult<jdouble>();
#else
    GetArrayElementsResult<jdouble> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetDoubleArrayElements(
        static_cast<jdoubleArray>(array), &return_value.is_copy);
    return return_value;
#endif  // DRY_RUN
  }

  static inline void ReleaseArrayElements(jarray array, jdouble* native_ptr,
                                          bool copy_on_completion) {
    Trace(metaprogramming::LambdaToStr(
              STR("ReleaseArrayElements, jdouble, Rank 1")),
          array, native_ptr, copy_on_completion);

#ifdef DRY_RUN
#else
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseDoubleArrayElements(
        static_cast<jdoubleArray>(array), native_ptr, copy_back_mode);
#endif  // DRY_RUN
  }
};

// Note, this requires both a jclass and a sample jobject to build from which
// is unlike any other new array construction.
template <std::size_t kRank>
struct JniArrayHelper<jobject, kRank> : public JniArrayHelperBase {
  using AsArrayType = jobjectArray;

  static inline jobjectArray NewArray(std::size_t size, jclass class_id,
                                      jobject initial_element) {
    Trace(metaprogramming::LambdaToStr(STR("NewArray, Rank >1")), kRank);

#ifdef DRY_RUN
    return Fake<jobjectArray>();
#else
    return jni::JniEnv::GetEnv()->NewObjectArray(size, class_id,
                                                 initial_element);
#endif  // DRY_RUN
  }

  // The API of fetching objects only permits accessing one object at a time.
  static inline jobject GetArrayElement(jobjectArray array, std::size_t idx) {
    Trace(metaprogramming::LambdaToStr(STR("GetArrayElement, Rank >1")), kRank);

#ifdef DRY_RUN
    return Fake<jobject>();
#else
    return jni::JniEnv::GetEnv()->GetObjectArrayElement(array, idx);
#endif  // DRY_RUN
  };

  // The API of fetching objects only permits accessing one object at a time.
  static inline void SetArrayElement(jobjectArray array, std::size_t idx,
                                     jobject obj) {
    Trace(metaprogramming::LambdaToStr(STR("SetArrayElement, Rank >1")), kRank);

#ifdef DRY_RUN
#else
    jni::JniEnv::GetEnv()->SetObjectArrayElement(array, idx, obj);
#endif  // DRY_RUN
  };
};

}  // namespace jni

namespace jni {

using LocalStringImpl =
    Scoped<LifecycleType::LOCAL, JniT<jstring, kJavaLangString>, jobject,
           jstring>;

// Represents and possibly builds a runtime Java String object.
//
// In order to use a string in memory (as opposed to only using it for function
// arguments), "Pin" the string.
//
// Like |jobjects|, |jstring|s can be either local or global with the same
// ownership semantics.
class LocalString : public LocalStringImpl {
 public:
  using Base = LocalStringImpl;
  using Base::Base;

  LocalString(LocalObject<kJavaLangString>&& obj)
      : Base(AdoptLocal{}, static_cast<jstring>(obj.Release())) {}

  template <typename T>
  LocalString(ArrayViewHelper<T> array_view_helper)
      : LocalString(AdoptLocal{}, array_view_helper.val_) {}

  // Returns a StringView which possibly performs an expensive pinning
  // operation.  String objects can be pinned multiple times.
  UtfStringView Pin() { return {RefBaseTag<jstring>::object_ref_}; }
};

}  // namespace jni

namespace jni {

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
using GlobalObjectImpl =
    Scoped<LifecycleType::GLOBAL,
           JniT<jobject, class_v_, class_loader_v_, jvm_v_>, jobject>;

template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class GlobalObject
    : public GlobalObjectImpl<class_v_, class_loader_v_, jvm_v_> {
 public:
  using Base = GlobalObjectImpl<class_v_, class_loader_v_, jvm_v_>;
  using Base::Base;
  using LifecycleT = LifecycleHelper<jobject, LifecycleType::GLOBAL>;

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  GlobalObject(GlobalObject<class_v, class_loader_v, jvm_v>&& obj)
      : Base(obj.Release()) {}

  template <typename... Ts>
  GlobalObject(Ts&&... vals) : Base(std::forward<Ts&&>(vals)...) {
    RefBaseTag<jobject>::object_ref_ =
        LifecycleT::Promote(RefBaseTag<jobject>::object_ref_);
  }

  GlobalObject() {
    RefBaseTag<jobject>::object_ref_ =
        LifecycleT::Promote(RefBaseTag<jobject>::object_ref_);
  }

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  GlobalObject& operator=(LocalObject<class_v, class_loader_v, jvm_v>&& rhs) {
    static_assert(::jni::metaprogramming::DeepEqualDiminished_v<
                  LocalObject<class_v_, class_loader_v_, jvm_v_>,
                  LocalObject<class_v, class_loader_v, jvm_v>>);
    Base::MaybeReleaseUnderlyingObject();
    Base::object_ref_ = rhs.Release();

    return *this;
  }
};

template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
GlobalObject(LocalObject<class_v, class_loader_v, jvm_v>&&)
    -> GlobalObject<class_v, class_loader_v, jvm_v>;

}  // namespace jni

#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// Metafunction for querying traits return and argument types of a function.
template <typename>
struct FunctionTraits;

// Partial specialisation for functions.
template <typename Return_, typename... Args>
struct FunctionTraits<Return_(Args...)> {
  using Return = Return_;
  using ArgsTup = std::tuple<Args...>;

  static constexpr std::size_t arity = sizeof...(Args);

  template <std::size_t N>
  struct Argument {
    static_assert(N < arity, "Parameter index exceeds argument count.");
    using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
  };
};

// Partial specialisation for function pointer.
template <typename R, typename... Args>
struct FunctionTraits<R (*)(Args...)> : public FunctionTraits<R(Args...)> {};

// Member function pointer
template <class C, class R, class... Args>
struct FunctionTraits<R (C::*)(Args...)>
    : public FunctionTraits<R(C&, Args...)> {};

// const member function pointer
template <class C, class R, class... Args>
struct FunctionTraits<R (C::*)(Args...) const>
    : public FunctionTraits<R(C&, Args...)> {};

// Member object pointer
template <class C, class R>
struct FunctionTraits<R(C::*)> : public FunctionTraits<R(C&)> {};

template <typename T>
using FunctionTraitsReturn_t = typename FunctionTraits<T>::Return;

template <typename T, std::size_t argument_idx>
using FunctionTraitsArg_t =
    typename FunctionTraits<T>::template Argument<argument_idx>::type;

}  // namespace jni::metaprogramming

#include <atomic>

namespace jni {

// Helper for JvmRef to enforce correct sequencing of getting and setting
// process level static fo JavaVM*.
class JvmRefBase {
 protected:
  friend class ThreadGuard;
  friend class ThreadLocalGuardDestructor;

  JvmRefBase(JavaVM* vm) { process_level_jvm_.store(vm); }
  ~JvmRefBase() { process_level_jvm_.store(nullptr); }

  static JavaVM* GetJavaVm() { return process_level_jvm_.load(); }
  static void SetJavaVm(JavaVM* jvm) { process_level_jvm_.store(jvm); }

  static inline std::atomic<JavaVM*> process_level_jvm_ = nullptr;
};

}  // namespace jni

#include <type_traits>

namespace jni {

template <LifecycleType lifecycleType>
using ClassLoaderImpl =
    Scoped<lifecycleType, JniT<jobject, kJavaLangClassLoader>, jobject>;

template <LifecycleType lifecycleType, const auto& class_loader_v_,
          const auto& jvm_v_>
class ClassLoaderRef : public ClassLoaderImpl<lifecycleType> {
 private:
  // Returns kDefaultJvm for default class loaded objects, otherwise returns the
  // jvm associated with this loader.  Default loaders do not use indexing,
  // whereas non-standard loaders do (to allow for programmatic Jvm teardown).
  template <const auto& class_v>
  static constexpr auto& JvmForLoader() {
    if constexpr (ParentLoaderForClass<class_loader_v_, class_v>() !=
                  kDefaultClassLoader) {
      return jvm_v_;
    } else {
      return kDefaultJvm;
    }
  }

 public:
  using Base = ClassLoaderImpl<lifecycleType>;
  using Base::Base;

  template <const auto& class_v, typename... Params>
  [[nodiscard]] auto BuildLocalObject(Params&&... params) {
    using JniClassT = JniT<jobject, class_v>;
    using IdClassT = Id<JniClassT, IdType::CLASS>;
    static_assert(
        !(ParentLoaderForClass<class_loader_v_, class_v>() == kNullClassLoader),
        "Cannot build this class with this loader.");

    if constexpr (ParentLoaderForClass<class_loader_v_, class_v>() !=
                  kDefaultClassLoader) {
      ClassRef_t<JniT<jobject, class_v, class_loader_v_, jvm_v_,
                      0>>::PrimeJClassFromClassLoader([=]() {
        // Prevent the object (which is a runtime instance of a class) from
        // falling out of scope so it is not released.
        LocalObject loaded_class =
            (*this)("loadClass", IdClassT::kNameUsingDots);

        // We only want to create global references if we are actually going
        // to use them so that they do not leak.
        jclass test_class{
            static_cast<jclass>(static_cast<jobject>(loaded_class))};
        return static_cast<jclass>(JniEnv::GetEnv()->NewGlobalRef(test_class));
      });
    }
    return LocalObject<class_v,
                       ParentLoaderForClass<class_loader_v_, class_v>(),
                       JvmForLoader<class_v>()>{
        std::forward<Params>(params)...};
  }

  template <const auto& class_v, typename... Params>
  [[nodiscard]] auto BuildGlobalObject(Params&&... params) {
    LocalObject obj =
        BuildLocalObject<class_v>(std::forward<Params>(params)...);
    jobject promoted_local =
        LifecycleHelper<jobject, LifecycleType::GLOBAL>::Promote(obj.Release());

    return GlobalObject<class_v,
                        ParentLoaderForClass<class_loader_v_, class_v>(),
                        JvmForLoader<class_v>()>{AdoptGlobal{}, promoted_local};
  }
};

}  // namespace jni

#include <iterator>

namespace jni {

template <typename T>
struct ArrayViewHelper {
  const T val_;
  operator T() const { return val_; }

  ArrayViewHelper(const T& val) : val_(val) {}
};

// Primitive Rank 1 Arrays.
template <typename SpanType, std::size_t kRank = 1, typename Enable = void>
class ArrayView {
 public:
  struct Iterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::size_t;
    using value_type = SpanType;
    using pointer = SpanType*;
    using reference = SpanType&;

    Iterator(SpanType* ptr, std::size_t size, std::size_t idx)
        : size_(size), ptr_(ptr), idx_(idx) {}

    Iterator& operator++() {
      idx_++;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    SpanType& operator*() const { return ptr_[idx_]; }
    const SpanType* operator->() const { return ptr_[idx_]; }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
      return (lhs.ptr_ == rhs.ptr_) && (lhs.size_ == rhs.size_) &&
             (lhs.idx_ == rhs.idx_);
    };

    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
      return !(lhs == rhs);
    };

    friend std::size_t operator-(const Iterator& lhs, const Iterator& rhs) {
      return lhs.idx_ - rhs.idx_;
    };

   private:
    const std::size_t size_;
    SpanType* const ptr_;
    std::size_t idx_;
  };

  ArrayView(ArrayView&&) = delete;
  ArrayView(const ArrayView&) = delete;

  ArrayView(jarray array, bool copy_on_completion, std::size_t size)
      : array_(array),
        get_array_elements_result_(
            JniArrayHelper<SpanType, kRank>::GetArrayElements(array)),
        copy_on_completion_(copy_on_completion),
        size_(size) {}

  ~ArrayView() {
    JniArrayHelper<SpanType, kRank>::ReleaseArrayElements(
        array_, get_array_elements_result_.ptr_, copy_on_completion_);
  }

  // Arrays of rank > 1 are object arrays which are not contiguous.
  std::enable_if_t<kRank == 1, SpanType*> ptr() {
    return get_array_elements_result_.ptr_;
  }

  Iterator begin() { return Iterator{ptr(), size_, 0}; }
  Iterator end() { return Iterator{ptr(), size_, size_}; }

 protected:
  const jarray array_;
  const GetArrayElementsResult<SpanType> get_array_elements_result_;
  const bool copy_on_completion_;
  const std::size_t size_;
};

// Object arrays, or arrays with rank > 1 (which are object arrays), or strings.
template <typename SpanType, std::size_t kRank>
class ArrayView<
    SpanType, kRank,
    std::enable_if_t<(kRank > 1) || std::is_same_v<SpanType, jobject> ||
                     std::is_same_v<SpanType, jstring>>> {
 public:
  // Metafunction that returns the type after a single dereference.
  template <std::size_t>
  struct PinHelper {
    using type = jobjectArray;
  };
  template <>
  struct PinHelper<2> {
    using type = RegularToArrayTypeMap_t<SpanType>;
  };
  template <>
  struct PinHelper<1> {
    using type = jobject;
  };

  using PinHelper_t = typename PinHelper<kRank>::type;

  struct Iterator {
    using iterator_category = std::random_access_iterator_tag;
    using difference_type = std::size_t;
    using value_type = PinHelper_t;
    using pointer = PinHelper_t*;
    using reference = PinHelper_t&;

    Iterator(jobjectArray arr, std::size_t size, std::size_t idx)
        : arr_(arr), size_(size), idx_(idx) {}

    Iterator& operator++() {
      idx_++;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    ArrayViewHelper<PinHelper_t> operator*() const {
      if constexpr (kRank >= 2) {
        return {static_cast<PinHelper_t>(
            JniArrayHelper<jobject, kRank>::GetArrayElement(arr_, idx_))};
      } else {
        return {JniArrayHelper<SpanType, kRank>::GetArrayElement(arr_, idx_)};
      }
    }

    friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
      return (lhs.arr_ == rhs.arr_) && (lhs.size_ == rhs.size_) &&
             (lhs.idx_ == rhs.idx_);
    };

    friend bool operator!=(const Iterator& lhs, const Iterator& rhs) {
      return !(lhs == rhs);
    };

    friend std::size_t operator-(const Iterator& lhs, const Iterator& rhs) {
      return lhs.idx_ - rhs.idx_;
    };

   private:
    jobjectArray const arr_;
    const std::size_t size_;
    std::size_t idx_;
  };

  ArrayView(ArrayView&&) = delete;
  ArrayView(const ArrayView&) = delete;

  // This constructor creates a copy of the parent `jarray` so that the
  // lifetime doesn't end before objects. e.g. `obj["field"].Get().Pin()` is a
  // useful pattern in iterators, but the returned Get() `LocalArray` would
  // be released immediately.
  ArrayView(jobjectArray array, bool, std::size_t size)
      : array_(
            LifecycleHelper<jobjectArray, LifecycleType::LOCAL>::NewReference(
                array)),
        size_(size) {}

  ~ArrayView() {
    LifecycleHelper<jobjectArray, LifecycleType::LOCAL>::Delete(array_);
  }

  Iterator begin() { return Iterator(array_, size_, 0); }
  Iterator end() { return Iterator(array_, size_, size_); }

 protected:
  const jobjectArray array_;
  const std::size_t size_;
};

// This CTAD guide is required for materialising new ArrayViews from |Pin()|
// calls as move and copy constructors are deleted.
template <typename SpanType, std::size_t kRank>
ArrayView(ArrayView<SpanType, kRank>&&) -> ArrayView<SpanType, kRank>;

}  // namespace jni

namespace jni {

// Designed to be the very last JniBind object to execute on the thread.
// Objects passed by move for lambdas will be destructed after any contents
// statements within their lambda, and `ThreadGuard` can't be moved into the
// lambda because its construction will be on the host thread. This static
// teardown guarantees a delayed destruction beyond any GlobalObject.
class ThreadLocalGuardDestructor {
 public:
  bool detach_thread_when_all_guards_released_ = false;

  // By calling this the compiler is obligated to perform initalisation.
  void ForceDestructionOnThreadClose() {}

  ~ThreadLocalGuardDestructor() {
    if (detach_thread_when_all_guards_released_) {
      JavaVM* jvm = JvmRefBase::GetJavaVm();
      if (jvm) {
        jvm->DetachCurrentThread();
      }
    }
  }
};

// ThreadGuard attaches and detaches JNIEnv* objects on the creation of new
// threads.  All new threads which want to use JNI Wrapper must hold a
// ThreadGuard beyond the scope of all created objects.  If the ThreadGuard
// needs to create an Env, it will also detach itself.
class ThreadGuard {
 public:
  ~ThreadGuard() { thread_guard_count_--; }

  ThreadGuard(ThreadGuard&) = delete;
  ThreadGuard(ThreadGuard&&) = delete;

  template <const auto& jvm_v_>
  friend class JvmRef;

  // This constructor must *never* be called before a |JvmRef| has been
  // constructed. It depends on static setup from |JvmRef|.
  [[nodiscard]] ThreadGuard() {
    thread_local_guard_destructor.ForceDestructionOnThreadClose();

    // Nested ThreadGuards should be permitted in the same way mutex locks are.
    thread_guard_count_++;
    if (thread_guard_count_ != 1) {
      // SetEnv has been called prior, GetEnv is currently valid.
      return;
    }

    // Declarations for AttachCurrentThread are inconsistent across different
    // JNI headers.  This forces a cast to whatever the expected type is.
    JavaVM* const vm = JvmRefBase::GetJavaVm();
    JNIEnv* jni_env = 0;

    using TypeForGetEnv =
        metaprogramming::FunctionTraitsArg_t<decltype(&JavaVM::GetEnv), 1>;
    const int code =
        vm->GetEnv(reinterpret_cast<TypeForGetEnv>(&jni_env), JNI_VERSION_1_6);

    if (code != JNI_OK) {
      using TypeForAttachment = metaprogramming::FunctionTraitsArg_t<
          decltype(&JavaVM::AttachCurrentThread), 1>;
      vm->AttachCurrentThread(reinterpret_cast<TypeForAttachment>(&jni_env),
                              nullptr);
      thread_local_guard_destructor.detach_thread_when_all_guards_released_ =
          true;
    }
    // Why not store this locally to ThreadGuard?
    //
    // JNIEnv is thread local static, and the context an object is built from
    // may not have easy access to a JNIEnv* (or this ThreadGuard).  For most
    // constructions of new objects, the env is likely trivial (it's passed as
    // part of the JNI call), however, if an object reference is moved from one
    // thread to another, the JNIEnv* is certainly not available.
    JniEnv::SetEnv(jni_env);
  }

 private:
  static inline thread_local int thread_guard_count_ = 0;
  static inline thread_local ThreadLocalGuardDestructor
      thread_local_guard_destructor{};
};

}  // namespace jni

namespace jni {

template <const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class GlobalClassLoader
    : public ClassLoaderRef<LifecycleType::GLOBAL, class_loader_v_, jvm_v_> {
 public:
  using Base = ClassLoaderRef<LifecycleType::GLOBAL, class_loader_v_, jvm_v_>;
  using Base::Base;

  template <const auto& class_loader_v, const auto& jvm_v>
  GlobalClassLoader(GlobalClassLoader<class_loader_v, jvm_v>&& rhs)
      : Base(rhs.Release()) {}
};

}  // namespace jni

#include <optional>

namespace jni {

// Note: All arrays are local (global arrays of local objects is too confusing).
template <typename JniT>
using ScopedArrayImpl =
    Scoped<LifecycleType::LOCAL, JniT, jarray, typename JniT::StorageType>;

// |SpanType| is primitive types like jint, jfloat, etc.
template <typename JniT, typename Enable = void>
class ArrayRef : public ScopedArrayImpl<JniT> {
 public:
  using Base = ScopedArrayImpl<JniT>;

  using Base::Base;
  using SpanType = typename JniT::SpanType;

  ArrayRef(std::size_t size)
      : Base(AdoptLocal{},
             JniArrayHelper<SpanType, JniT::kRank>::NewArray(size)) {}

  template <typename T>
  ArrayRef(const ArrayViewHelper<T>& array_view_helper)
      : Base(AdoptLocal{}, array_view_helper.val) {}

  explicit ArrayRef(int size) : ArrayRef(static_cast<std::size_t>(size)) {}

  ArrayView<SpanType, JniT::kRank> Pin(bool copy_on_completion = true) {
    return {Base::object_ref_, copy_on_completion, Length()};
  }

  std::size_t Length() {
    if (length_.load() == kNoIdx) {
      length_.store(
          JniArrayHelper<SpanType, JniT::kRank>::GetLength(Base::object_ref_));
    }

    return length_.load();
  }

 private:
  std::atomic<std::size_t> length_ = kNoIdx;
};

// Shared behaviour for object like arrays.
template <typename JniT>
class ArrayRefBase : public ScopedArrayImpl<JniT> {
 public:
  using Base = ScopedArrayImpl<JniT>;
  using Base::Base;
  using SpanType = typename JniT::SpanType;

  // Construct array with given size and null values.
  explicit ArrayRefBase(std::size_t size)
      : Base(AdoptLocal{},
             JniArrayHelper<jobject, JniT::kRank>::NewArray(
                 size, ClassRef_t<JniT>::GetAndMaybeLoadClassRef(nullptr),
                 static_cast<jobject>(nullptr))) {}

  // Construct from jobject lvalue (object is used as template).
  explicit ArrayRefBase(std::size_t size, jobject obj)
      : Base(AdoptLocal{}, JniArrayHelper<jobject, JniT::kRank>::NewArray(
                               size,
                               ClassRef_t<JniT>::GetAndMaybeLoadClassRef(
                                   static_cast<jobject>(obj)),
                               static_cast<jobject>(obj))) {}

  // Object arrays cannot be efficiently pinned like primitive types can.
  ArrayView<SpanType, JniT::kRank> Pin() {
    return {Base::object_ref_, false, Length()};
  }

  std::size_t Length() {
    return JniArrayHelper<jobject, JniT::kRank>::GetLength(Base::object_ref_);
  }

  // Note: Globals are not permitted in a local array because it makes reasoning
  // about them confusing.
  //
  // TODO(b/406948932): Permit lvalues of locals and globals as technically
  // they're both viable (the scope will be extended as expected).
  void Set(
      std::size_t idx,
      LocalObject<JniT::class_v, JniT::class_loader_v, JniT::jvm_v>&& val) {
    JniArrayHelper<jobject, JniT::kRank>::SetArrayElement(Base::object_ref_,
                                                          idx, val.Release());
  }
};

// |SpanType| is object and rank is 1.
template <typename JniT>
class ArrayRef<
    JniT, std::enable_if_t<(std::is_same_v<typename JniT::SpanType, jobject> &&
                            JniT::kRank == 1)>> : public ArrayRefBase<JniT> {
 public:
  using Base = ArrayRefBase<JniT>;
  using Base::Base;
  using SpanType = typename JniT::SpanType;

  // Construct from LocalObject lvalue (object is used as template).
  //
  // e.g.
  //  LocalArray arr { 5, LocalObject<kClass> {args...} };
  //  LocalArray arr { 5, GlobalObject<kClass> {args...} };
  template <template <const auto&, const auto&, const auto&>
            class ObjectContainer,
            const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  ArrayRef(std::size_t size,
           const ObjectContainer<class_v, class_loader_v, jvm_v>& obj)
      : ArrayRef(size, static_cast<jobject>(obj)) {}

  LocalObject<JniT::class_v, JniT::class_loader_v, JniT::jvm_v> Get(
      std::size_t idx) {
    return {JniArrayHelper<jobject, JniT::kRank>::GetArrayElement(
        Base::object_ref_, idx)};
  }
};

// |SpanType| is object or rank is > 1.
template <typename JniT>
class ArrayRef<JniT, std::enable_if_t<(JniT::kRank > 1)>>
    : public ArrayRefBase<JniT> {
 public:
  using Base = ArrayRefBase<JniT>;
  using Base::Base;

  static constexpr std::decay_t<decltype(JniT::GetClass())> clazz =
      JniT::GetClass();
  static constexpr std::decay_t<decltype(JniT::GetClassLoader())> class_loader =
      JniT::GetClassLoader();
  static constexpr std::decay_t<decltype(JniT::GetJvm())> jvm = JniT::GetJvm();

  LocalArray<typename JniT::SpanType, JniT::kRank - 1, clazz, class_loader, jvm>
  Get(std::size_t idx) {
    return {AdoptLocal{},
            static_cast<jarray>(
                JniArrayHelper<typename JniT::SpanType,
                               JniT::kRank>::GetArrayElement(Base::object_ref_,
                                                             idx))};
  }

  template <typename SpanType, std::size_t kRank_, const auto& class_v_,
            const auto& class_loader_v_, const auto& jvm_v_>
  void Set(std::size_t idx, const LocalArray<SpanType, kRank_, class_v_,
                                             class_loader_v_, jvm_v_>& val) {
    using ElementT =
        typename JniArrayHelper<SpanType, JniT::kRank - 1>::AsArrayType;
    JniArrayHelper<ElementT, JniT::kRank>::SetArrayElement(
        Base::object_ref_, idx, static_cast<ElementT>(val));
  }
};

}  // namespace jni

#include <atomic>
#include <cstddef>
#include <type_traits>

namespace jni {

// Represents a an array object (e.g. int[], float[][], Object[], etc).
// Currently GlobalArrays do not exist, as reasoning about the lifecycles of the
// underlying objects is non-trivial, e.g. a GlobalArray taking a local object
// would result in a possibly unexpected extension of lifetime.
template <typename SpanType, std::size_t kRank_ = 1,
          const auto& class_v_ = kNoClassSpecified,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class LocalArray
    : public ArrayRef<
          JniT<SpanType, class_v_, class_loader_v_, jvm_v_, kRank_>> {
 public:
  static constexpr Class kClass{class_v_.name_};
  static constexpr std::size_t kRank = kRank_;

  using RawValT = std::conditional_t<std::is_same_v<jobject, SpanType>,
                                     std::decay_t<decltype(kClass)>, SpanType>;

  using JniT_ = JniT<SpanType, class_v_, class_loader_v_, jvm_v_, kRank_>;

  using Base =
      ArrayRef<JniT<SpanType, class_v_, class_loader_v_, jvm_v_, kRank_>>;
  using Base::Base;

  using RefTag = std::conditional_t<(kRank_ > 1), jobject, SpanType>;

  // RefTag ctor (supports multi-dimensions, `jobject` if rank > 1).
  LocalArray(std::size_t size, RefTag arr)
      : Base(AdoptLocal{}, JniArrayHelper<jobject, kRank_>::NewArray(
                               size,
                               ClassRef_t<JniT_>::GetAndMaybeLoadClassRef(
                                   static_cast<jobject>(arr)),
                               arr)) {}

  template <typename T>
  LocalArray(ArrayViewHelper<T> array_view_helper)
      : LocalArray(AdoptLocal{}, array_view_helper.val_) {}

  // Rvalue ctor.
  LocalArray(LocalArray<SpanType, kRank_>&& rhs)
      : Base(AdoptLocal{}, rhs.Release()) {}

  // Rvalue ctor.
  template <typename SpanType_, std::size_t kRank, const auto& class_v,
            const auto& class_loader_v, const auto& jvm_v>
  LocalArray(LocalArray<SpanType_, kRank, class_v, class_loader_v, jvm_v>&& rhs)
      : Base(AdoptLocal{}, rhs.Release()) {
    static_assert(std::is_same_v<SpanType, SpanType_> && kRank == kRank_ &&
                  class_v == class_v_ && class_loader_v == class_loader_v_);
  }

  // Construct from decorated object lvalue (object is used as template).
  //
  // e.g.
  //  LocalArray arr { 5, LocalObject<kClass> {args...} };
  //  LocalArray arr { 5, GlobalObject<kClass> {args...} };
  template <template <const auto&, const auto&, const auto&>
            class ObjectContainer,
            const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  LocalArray(
      std::size_t size,
      const ObjectContainer<class_v, class_loader_v, jvm_v>& local_object)
      : Base(JniArrayHelper<jobject, kRank_>::NewArray(
            size,
            ClassRef_t<JniT_>::GetAndMaybeLoadClassRef(
                static_cast<jobject>(local_object)),
            static_cast<jobject>(local_object))) {}

  operator jobject() { return static_cast<jobject>(Base::object_ref_); }
};

template <template <const auto&, const auto&, const auto&>
          class ObjectContainer,
          const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
LocalArray(std::size_t,
           const ObjectContainer<class_v_, class_loader_v_, jvm_v_>&)
    -> LocalArray<jobject, class_v_, class_loader_v_, jvm_v_>;

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
LocalArray(
    std::size_t,
    const Scoped<LifecycleType::LOCAL,
                 JniT<jobject, class_v_, class_loader_v_, jvm_v_>, jobject>&)
    -> LocalArray<jobject, class_v_, class_loader_v_, jvm_v_>;

template <typename SpanType>
LocalArray(std::size_t, SpanType)
    -> LocalArray<SpanType, 1, kNoClassSpecified, kDefaultClassLoader,
                  kDefaultJvm>;

template <typename SpanType, std::size_t kRank_minus_1>
LocalArray(std::size_t, LocalArray<SpanType, kRank_minus_1>)
    -> LocalArray<SpanType, kRank_minus_1 + 1>;

template <typename SpanType, std::size_t kRank_minus_1, const auto& class_v,
          const auto& class_loader_v, const auto& jvm_v>
LocalArray(std::size_t,
           LocalArray<SpanType, kRank_minus_1, class_v, class_loader_v, jvm_v>)
    -> LocalArray<SpanType, kRank_minus_1 + 1>;

template <typename SpanType, std::size_t kRank_minus_1>
LocalArray(std::size_t, LocalArray<SpanType, kRank_minus_1>&&)
    -> LocalArray<SpanType, kRank_minus_1 + 1>;

template <typename TUndecayed>
struct ProxyHelper;

}  // namespace jni

#include <atomic>
#include <memory>
#include <utility>

namespace jni {

// Represents a runtime instance of a Java Virtual Machine.
// The caller is responsible for dropping this object from scope when
// JNI_OnUnload is called.
//
// For any new thread spawned, a ThreadGuard must be held.  The caller is
// responsible for ensuring all ThreadGuards fall from scope before JvmRef falls
// from scope.
//
// The caller is also responsible for thread safety of all objects that have
// been built during the lifetime of the JVM.  i.e If any objects have been
// built, they must fall from scope prior to JvmRef falling from scope.
//
// There should only be one instance of JvmRef created at a time. If the
// lifetimes of multiple JvmRef overlap, then one instance may get an invalid
// JavaVM after the first instance is destroyed.
template <const auto& jvm_v_>
class JvmRef : public JvmRefBase {
 public:
  template <size_t ClassLoaderIdx>
  struct TeardownClassesHelper {
    template <size_t... Is>
    static constexpr void TeardownClass(
        std::index_sequence<Is...> index_sequence) {
      (ClassRef_t<JniT<jobject, kNoClassSpecified, kDefaultClassLoader, jvm_v_,
                       0, Is, ClassLoaderIdx>

                  >::MaybeReleaseClassRef(),
       ...);
    }
  };

  template <size_t... Is>
  constexpr void TeardownClassloadersHelper(
      std::index_sequence<Is...> index_sequence) {
    (TeardownClassesHelper<Is>::TeardownClass(
         std::make_index_sequence<
             std::tuple_size_v<decltype(std::get<Is>(jvm_v_.class_loaders_)
                                            .supported_classes_)>>()),
     ...);
  }

  JavaVM* BuildJavaVMFromEnv(JNIEnv* env) {
    JavaVM* vm = nullptr;
    // 0 Is success.
    if (env->GetJavaVM(&vm) == 0) {
      return vm;
    } else {
      return nullptr;
    }
  }

  explicit JvmRef(JNIEnv* env) : JvmRefBase(BuildJavaVMFromEnv(env)) {}
  explicit JvmRef(JavaVM* vm) : JvmRefBase(vm) {}

  ~JvmRef() {
    TeardownClassloadersHelper(
        std::make_index_sequence<
            std::tuple_size_v<decltype(jvm_v_.class_loaders_)>>());

    // This object has two lifecycle phases in relation to data races
    // 1)  Value is null, when it is guarded by the ClassRef mutex
    //     (implicitly part of ClassRef's behaviour).
    // 2)  JVM is tearing down.  At this point, the caller is responsible for
    //     releasing all native resources.
    //     ReleaseAllClassRefsForDefaultClassLoader will only ever be torn down
    //     by JvmRef::~JvmRef, and JvmRef cannot be moved, therefore it is
    //     guaranteed to be in a single threaded context.
    auto& default_loaded_class_list = DefaultRefs<jclass>();
    for (metaprogramming::DoubleLockedValue<jclass>* maybe_loaded_class_id :
         default_loaded_class_list) {
      maybe_loaded_class_id->Reset([](jclass clazz) {
        LifecycleHelper<jobject, LifecycleType::GLOBAL>::Delete(clazz);
      });
    }
    default_loaded_class_list.clear();

    // Methods do not need to be released, just forgotten.
    auto& default_loaded_method_ref_list = DefaultRefs<jmethodID>();
    for (metaprogramming::DoubleLockedValue<jmethodID>* cached_method_id :
         default_loaded_method_ref_list) {
      cached_method_id->Reset();
    }
    default_loaded_method_ref_list.clear();

    // Fields do not need to be released, just forgotten.
    auto& default_loaded_field_ref_list = GetDefaultLoadedFieldList();
    for (metaprogramming::DoubleLockedValue<jfieldID>* cached_field_id :
         default_loaded_field_ref_list) {
      cached_field_id->Reset();
    }
    default_loaded_field_ref_list.clear();
  }

  // Deleted in order to make various threading guarantees (see class_ref.h).
  JvmRef(const JvmRef&) = delete;
  JvmRef(JvmRef&&) = delete;

  // All new threads MUST create a guard by calling |BuildThreadGuard|.
  // If a JNIEnv does not exist, this will DetachCurrentThread when done.
  [[nodiscard]] ThreadGuard BuildThreadGuard() const { return {}; }

  // Sets a "fallback" loader for use when default Jvm classes fail to load.
  // This is useful for first use of classes on secondary threads where the
  // jclass is not yet cached and the classloader isn't available directly.
  void SetFallbackClassLoader(
      jni::GlobalClassLoader<kDefaultClassLoader, kDefaultJvm>&& loader) {
    fallback_loader_.reset(
        new jni::GlobalClassLoader<kDefaultClassLoader, kDefaultJvm>(
            AdoptGlobal{}, loader.Release()));

    FallbackLoader() = static_cast<jobject>(*fallback_loader_);
  }

  // Sets a "fallback" loader for use when default Jvm classes fail to load.
  // `host_object *must* be local and will *not* be released.
  void SetFallbackClassLoaderFromJObject(jobject host_object) {
    SetFallbackClassLoader(LocalObject<kJavaLangObject>{host_object}(
        "getClass")("getClassLoader"));
  }

 private:
  // Main thread has a JNIEnv just like every other thread.
  const ThreadGuard thread_guard_ = {};

  std::unique_ptr<jni::GlobalClassLoader<kDefaultClassLoader, kDefaultJvm>>
      fallback_loader_;
};

JvmRef(JNIEnv*) -> JvmRef<kDefaultJvm>;
JvmRef(JavaVM*) -> JvmRef<kDefaultJvm>;

}  // namespace jni

namespace jni {

template <typename CrtpBase_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
struct StaticRefHelper {
  using JniT = JniT<jobject, class_v_, class_loader_v_, jvm_v_>;

  using MethodMapT = metaprogramming::InvocableMap<CrtpBase_, JniT::static_v,
                                                   typename JniT::StaticT,
                                                   &JniT::StaticT::methods_>;
  using FieldMapT = metaprogramming::QueryableMap_t<CrtpBase_, JniT::static_v,
                                                    &JniT::StaticT::fields_>;
};

template <typename CrtpBase_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
using StaticRefHelperMethodMap_t =
    typename StaticRefHelper<CrtpBase_, class_v_, class_loader_v_,
                             jvm_v_>::MethodMapT;

template <typename CrtpBase_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
using StaticRefHelperFieldMap_t =
    typename StaticRefHelper<CrtpBase_, class_v_, class_loader_v_,
                             jvm_v_>::FieldMapT;

template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
struct StaticRef
    : public StaticRefHelperMethodMap_t<
          StaticRef<class_v_, class_loader_v_, jvm_v_>, class_v_,
          class_loader_v_, jvm_v_>,
      StaticRefHelperFieldMap_t<StaticRef<class_v_, class_loader_v_, jvm_v_>,
                                class_v_, class_loader_v_, jvm_v_> {
  using JniT = JniT<jobject, class_v_, class_loader_v_, jvm_v_>;

  jclass GetJClass() const {
    return ClassRef_t<JniT>::GetAndMaybeLoadClassRef(nullptr);
  }

  template <size_t I, typename... Args>
  auto InvocableMapCall(const char* key, Args&&... args) const {
    using IdT = Id<JniT, IdType::STATIC_OVERLOAD_SET, I>;
    using MethodSelectionForArgs =
        OverloadSelector<IdT, IdType::STATIC_OVERLOAD,
                         IdType::STATIC_OVERLOAD_PARAM, Args...>;

    static_assert(MethodSelectionForArgs::kIsValidArgSet,
                  "JNI Error: Invalid argument set.");

    return MethodSelectionForArgs::OverloadRef::Invoke(
        GetJClass(), nullptr, std::forward<Args>(args)...);
  }

  template <size_t I>
  auto QueryableMapCall(const char* key) const {
    return FieldRef<JniT, IdType::STATIC_FIELD, I>{GetJClass(), nullptr};
  }
};

}  // namespace jni

namespace jni {

template <const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class LocalClassLoader
    : public ClassLoaderRef<LifecycleType::LOCAL, class_loader_v_, jvm_v_> {
 public:
  using Base = ClassLoaderRef<LifecycleType::LOCAL, class_loader_v_, jvm_v_>;
  using Base::Base;

  template <const auto& class_loader_v, const auto& jvm_v>
  explicit LocalClassLoader(LocalClassLoader<class_loader_v, jvm_v>&& lhs)
      : LocalClassLoader(AdoptLocal{}, lhs.Release()) {}

 private:
  template <typename>
  friend class ObjectRef;
};

}  // namespace jni

#include <cstddef>
#include <type_traits>

namespace jni {

template <std::size_t kRank_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
class LocalArray<jstring, kRank_, class_v_, class_loader_v_, jvm_v_>
    : public LocalArray<jobject, kRank_, kJavaLangString, kDefaultClassLoader,
                        kDefaultJvm> {
 public:
  using Base = LocalArray<jobject, kRank_, kJavaLangString, kDefaultClassLoader,
                          kDefaultJvm>;

  using Base::Base;
  using StringJniT =
      JniT<jstring, kJavaLangString, kDefaultClassLoader, kDefaultJvm>;

  LocalArray(std::size_t size)
      : Base(JniArrayHelper<jobject, kRank_>::NewArray(
            size, ClassRef<StringJniT>::GetAndMaybeLoadClassRef(nullptr),
            nullptr)) {}

  // Note: Globals are not permitted in a `LocalArray` because it makes
  // reasoning about them confusing.
  void Set(
      std::size_t idx,
      LocalObject<kJavaLangString, kDefaultClassLoader, kDefaultJvm>&& val) {
    return JniArrayHelper<jobject, kRank_>::SetArrayElement(Base::object_ref_,
                                                            idx, val.Release());
  }

  void Set(std::size_t idx, LocalString&& val) {
    return JniArrayHelper<jobject, kRank_>::SetArrayElement(Base::object_ref_,
                                                            idx, val.Release());
  }

  void Set(std::size_t idx, const char* val) { Set(idx, LocalString{val}); }

  void Set(std::size_t idx, std::string val) {
    Set(idx, LocalString{std::move(val)});
  }

  void Set(std::size_t idx, std::string_view val) {
    Set(idx, LocalString{val});
  }
};

}  // namespace jni

namespace jni {

using GlobalStringImpl =
    Scoped<LifecycleType::GLOBAL, JniT<jstring, kJavaLangString>, jobject,
           jstring>;

class GlobalString : public GlobalStringImpl {
 public:
  using Base = GlobalStringImpl;
  using Base::Base;

  using LifecycleT = LifecycleHelper<jstring, LifecycleType::GLOBAL>;

  GlobalString(GlobalObject<kJavaLangString, kDefaultClassLoader, kDefaultJvm>&&
                   global_string)
      : Base(static_cast<jstring>(global_string.Release())) {}

  GlobalString(LocalString&& local_string)
      : Base(LifecycleT::Promote(local_string.Release())) {}

  // Returns a StringView which possibly performs an expensive pinning
  // operation.  String objects can be pinned multiple times.
  UtfStringView Pin() { return {RefBaseTag<jstring>::object_ref_}; }
};

}  // namespace jni

namespace jni {

// This is currently using JNI Bind itself and so cannot be directly included.
// By delaying linkage for this we can use a bootstrapped JNI Bind. This could
// be rewritten in porcelain JNI, but this flow ought to be only on
// secondary threads with previously unused `jclass`'s (which is mostly rare),
// and this is simpler to reason about than porcelain JNI.
inline jclass FindClassFallback(const char* class_name) {
  // The loader will be primed by the JVM, however, it needs to be accessible
  // from the jni_helper layer. See `JvmRef` for how this is primed.
  GlobalClassLoader<kDefaultClassLoader> loader{AdoptGlobal{},
                                                FallbackLoader()};

  jni::LocalObject loaded_class = loader("loadClass", class_name);
  jclass ret{static_cast<jclass>(static_cast<jobject>(loaded_class.Release()))};

  loader.Release();

  return ret;
}

}  // namespace jni


namespace jni {

inline constexpr Class kJavaUtilList{
    "java/util/List",
    Method{"add", jni::Return<jboolean>{}, jni::Params{kJavaLangObject}},
    Method{"clear", jni::Return{}, jni::Params{}},
    Method{"get", jni::Return{kJavaLangObject}, jni::Params<jint>{}},
    Method{"remove", jni::Return{kJavaLangObject}, jni::Params<jint>{}},
    Method{"size", jni::Return<jint>{}, jni::Params{}}};

}  // namespace jni

#endif  // JNI_BIND_RELEASE_
