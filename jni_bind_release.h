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
 * JNI Bind Version 0.1.
 * Alpha Public Release.
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


namespace jni {

struct Object {
  const char* name_;
  constexpr explicit Object(const char* name) : name_(name) {}
};

}  // namespace jni


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

}  // namespace jni::metaprogramming


#include <tuple>
#include <type_traits>

namespace jni::metaprogramming {

// "Invokes" a metafunctions (i.e. a type alias for Func::type).
// Handles both variadic or non-variadic args for a consistent syntax.
template <typename... Func>
struct Invoke {};

// Single function invocation (typical case).
template <typename Func>
struct Invoke<Func> {
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

}  // namespace jni::metaprogramming


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
// some minimal type wrapping such as Object<Name> (see JniTypeProxy).
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


#include <utility>

namespace jni {

/**
// JniHelper is a shim to using a JNIenv object.
// This extra layer of indirection allows for quickly shimming all JNICalls
// (e.g. adding exception checking, logging each JNI call, etc).
**/
class JniHelper {
 public:
  // Finds a class with "name".  Note, the classloader used is whatever is
  // present on the statck when this is caled.  No caching is performed,
  // returned jclass is a local.
  static jclass FindClass(const char* name);

  // Returns a local ref jclass for the given jobject.
  // Note, if the object is polymorphic it may be a sub or superclass.
  static jclass GetObjectClass(jobject object);

  // See FindClass and jni::Jvm.
  static void ReleaseClass(jclass clazz);

  // Gets a method for a signature (no caching is performed).
  static inline jmethodID GetMethodID(jclass clazz, const char* method_name,
                                      const char* method_signature);

  // FieldIds follow the same semantics as MethodIds.
  static inline jfieldID GetFieldID(jclass clazz, const char* name,
                                    const char* signature);

  // Objects.
  template <typename... CtorArgs>
  static jobject NewLocalObject(jclass clazz, jmethodID ctor_method,
                                CtorArgs&&... ctor_args);

  // Creates a new GlobalRef to |local_object|, then deletes the local
  // reference.
  static jobject PromoteLocalToGlobalObject(jobject local_object);

  // Creates a new GlobalRef to |local_class|, then deletes the local
  // reference.
  static jclass PromoteLocalToGlobalClass(jclass local_class);

  static void DeleteLocalObject(jobject object);

  template <typename... CtorArgs>
  static jobject NewGlobalObject(jclass clazz, jmethodID ctor_method,
                                 CtorArgs&&... ctor_args);

  static void DeleteGlobalObject(jobject obj_ref);

  // Strings.
  static jstring NewLocalString(const char*);

  // Creates a new GlobalRef to |local_string| , then deletes the local string.
  static jstring PromoteLocalToGlobalString(jstring local_string);

  static void DeleteGlobalString(jstring string);

  static const char* GetStringUTFChars(jstring str);

  static void ReleaseStringUTFChars(jstring str, const char* chars);
};

//==============================================================================

inline jclass JniHelper::FindClass(const char* name) {
  return jni::JniEnv::GetEnv()->FindClass(name);
}

inline jclass JniHelper::GetObjectClass(jobject object) {
  return jni::JniEnv::GetEnv()->GetObjectClass(object);
}

inline void JniHelper::ReleaseClass(jclass clazz) {
  jni::JniEnv::GetEnv()->DeleteGlobalRef(clazz);
}

jmethodID JniHelper::GetMethodID(jclass clazz, const char* method_name,
                                 const char* method_signature) {
  return jni::JniEnv::GetEnv()->GetMethodID(clazz, method_name,
                                            method_signature);
}

jfieldID JniHelper::GetFieldID(jclass clazz, const char* name,
                               const char* signature) {
  return jni::JniEnv::GetEnv()->GetFieldID(clazz, name, signature);
}

template <typename... CtorArgs>
jobject JniHelper::NewLocalObject(jclass clazz, jmethodID ctor_method,
                                  CtorArgs&&... ctor_args) {
  return jni::JniEnv::GetEnv()->NewObject(clazz, ctor_method, ctor_args...);
}

inline void JniHelper::DeleteLocalObject(jobject object) {
  jni::JniEnv::GetEnv()->DeleteLocalRef(object);
}

inline jobject JniHelper::PromoteLocalToGlobalObject(jobject local_object) {
  JNIEnv* const env = jni::JniEnv::GetEnv();
  jobject global_object = env->NewGlobalRef(local_object);
  env->DeleteLocalRef(local_object);
  return global_object;
}

inline jclass JniHelper::PromoteLocalToGlobalClass(jclass local_class) {
  return reinterpret_cast<jclass>(JniEnv::GetEnv()->NewGlobalRef(local_class));
}

template <typename... CtorArgs>
inline jobject JniHelper::NewGlobalObject(jclass clazz, jmethodID ctor_method,
                                          CtorArgs&&... ctor_args) {
  // Note, this local ref handle created below is never leaked outside of this
  // scope and should naturally be cleaned up when invoking JNI function
  // completes.  That said, the maximum number of local refs can be extremely
  // limited (the standard only requires 16), and if the caller doesn't
  // explicitly reach for the performant option, it doesn't make sense to
  // provide a micro optimisation of skipping the delete call below.
  //
  // If consumers want the most performant option, they should use LocalRef
  // implementations when building their dynamic object.
  JNIEnv* const env = jni::JniEnv::GetEnv();
  jobject local_object = NewLocalObject(env, clazz, ctor_method,
                                        std::forward<CtorArgs>(ctor_args)...);
  jobject global_object = env->NewGlobalRef(local_object);

  env->DeleteLocalRef(local_object);

  return global_object;
}

inline void JniHelper::DeleteGlobalObject(jobject obj_ref) {
  jni::JniEnv::GetEnv()->DeleteGlobalRef(obj_ref);
}

inline jstring JniHelper::NewLocalString(const char* chars) {
  return jni::JniEnv::GetEnv()->NewStringUTF(chars);
}

inline jstring JniHelper::PromoteLocalToGlobalString(jstring local_string) {
  // jstrings follow the semantics of regular objects.
  JNIEnv* const env = jni::JniEnv::GetEnv();
  jstring global_string = static_cast<jstring>(env->NewGlobalRef(local_string));
  env->DeleteLocalRef(local_string);
  return global_string;
}

inline void JniHelper::DeleteGlobalString(jstring string) {
  jni::JniEnv::GetEnv()->DeleteGlobalRef(string);
}

inline const char* JniHelper::GetStringUTFChars(jstring str) {
  // If is_copy is an address of bool it will be set to true or false if a copy
  // is made.  That said, this seems to be of no consequence, as the API still
  // requires you to release the string at the end. There's no discernible
  // reason you would ever be able to meaningfully act differently based on
  // this parameter of the API (except to do the wrong thing).
  return jni::JniEnv::GetEnv()->GetStringUTFChars(str, /*isCopy=*/nullptr);
}

inline void JniHelper::ReleaseStringUTFChars(jstring str, const char* chars) {
  jni::JniEnv::GetEnv()->ReleaseStringUTFChars(str, chars);
}

}  // namespace jni


#include <string_view>

namespace jni {

template <typename Raw_>
struct Return {
  const Raw_ raw_ = {};

  using Raw = Raw_;

  constexpr Return() = default;

  template <typename Raw>
  constexpr explicit Return(Raw raw) : raw_(raw) {}
};

template <>
struct Return<void> {
  using Raw = void;

  constexpr Return() = default;
};

Return()->Return<void>;

template <typename Raw>
Return(Raw) -> Return<Raw>;

//==============================================================================

template <typename T>
using Raw_t = typename T::Raw;

}  // namespace jni


#include <tuple>
#include <type_traits>

namespace jni {

struct OverloadBase {};

// Represents a single overload of a given method.
template <typename ReturnT_, typename Params_>
struct Overload : OverloadBase {
  const ReturnT_ return_;
  const Params_ params_;

  constexpr Overload(ReturnT_ return_type, Params_ params)
      : return_(return_type), params_(params) {}
};

template <typename ReturnT_, typename Params_>
Overload(ReturnT_, Params_) -> Overload<ReturnT_, Params_>;

struct MethodBase {};

template <typename Returns_, typename Params_>
struct Method {};

template <typename... Returns, typename... Params_>
struct Method<std::tuple<Returns...>, std::tuple<Params_...>>
    : public MethodBase {
 public:
  const char* name_;
  const std::tuple<Overload<Returns, Params_>...> invocations_;

  template <typename ReturnT_, typename ParamsT_,
            std::enable_if_t<std::is_base_of_v<ParamsBase, ParamsT_>, int> = 0>
  constexpr Method(const char* name, ReturnT_ return_type, ParamsT_ params)
      : name_(name), invocations_(Overload{return_type, params}) {}

  constexpr Method(const char* name, Overload<Returns, Params_>... invocations)
      : name_(name), invocations_(invocations...) {}
};

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


#include <string>

namespace jni {

struct FieldBase {};

template <typename Raw_>
struct Field : public FieldBase {
 public:
  using Raw = Raw_;

  const char* name_;

  const Raw_ raw_ = {};
  const Raw_ raw_type_ = raw_;

  constexpr Field(const char* name) : name_(name) {}
  constexpr Field(const char* name, Raw_ value_raw)
      : name_(name), raw_(value_raw) {}
};

template <typename Raw_>
Field(const char*, Raw_) -> Field<Raw_>;

template <typename T>
using Raw_t = typename T::Raw;

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
                          const Constructor<LhsParams...>& rhs) {
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


#include <string_view>
#include <tuple>
#include <type_traits>

namespace jni {

static constexpr struct NoClass {
} kNoClassSpecified;

template <typename Constructors_, typename Fields_, typename Methods_>
struct Class {};

template <typename... Constructors_, typename... Fields_, typename... Methods_>
struct Class<std::tuple<Constructors_...>, std::tuple<Fields_...>,
             std::tuple<Methods_...>> : public Object {
 public:
  const std::tuple<Constructors_...> constructors_;
  const std::tuple<Methods_...> methods_;
  const std::tuple<Fields_...> fields_;

  explicit constexpr Class(const char* class_name,
                           Constructors_... constructors, Methods_... methods,
                           Fields_... fields)
      : Object(class_name),
        constructors_(constructors...),
        methods_(methods...),
        fields_(fields...) {}

  explicit constexpr Class(const char* class_name, Methods_... methods,
                           Fields_... fields)
      : Class(class_name, Constructor<>{}, methods..., fields...) {}

  template <typename... Params, typename... Constructors, typename... Fields,
            typename... Methods>
  constexpr bool operator==(
      const Class<std::tuple<Constructors...>, std::tuple<Fields...>,
                  std::tuple<Methods...>>& rhs) const {
    // Don't compare the other parameters so classes can be used as parameters
    // or return values before the class itself is defined.
    return std::string_view(name_) == std::string_view(rhs.name_);
  }
};

template <typename... Params>
Class(const char*, Params...)
    -> Class<metaprogramming::BaseFilterWithDefault_t<ConstructorBase,
                                                      Constructor<>, Params...>,
             metaprogramming::BaseFilter_t<FieldBase, Params...>,
             metaprogramming::BaseFilter_t<MethodBase, Params...>>;

Class(const char*)
    ->Class<std::tuple<Constructor<>>, std::tuple<>, std::tuple<>>;

}  // namespace jni


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
  template <const auto&, bool ignore_default_loader = false>
  constexpr bool SupportedDirectlyOrIndirectly() const {
    return false;
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

inline constexpr NullClassLoader kNullClassLoader;
inline constexpr DefaultClassLoader kDefaultClassLoader;

}  // namespace jni


namespace jni {

inline constexpr Class kJavaLangClass{"java/lang/Class"};

inline constexpr Class kJavaLangObject{"java/lang/Object"};

inline constexpr Method kLoadClassMethod{"loadClass", Return{kJavaLangClass},
                                         Params<jstring>{}};

inline constexpr Class kJavaLangClassLoader{"java/lang/ClassLoader",
                                            kLoadClassMethod};

// TODO (b/143908983): This should be in kJavaLangClass, but we currently
// cannot represent dependency loops (ClassLoader.findClass() returns a class,
// Class.getClassLoader() returns a ClassLoader) without a dummy class.
inline constexpr Method kGetClassLoaderMethod{
    "getClassLoader", Return{kJavaLangClassLoader}, Params{}};

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

#include <type_traits>

namespace jni {

// Convenience struct for returning results from pinning array.
template <typename SpanType>
struct GetArrayElementsResult {
  SpanType* ptr_;
  jboolean is_copy;
};

struct JniArrayHelperBase {
  static inline std::size_t GetLength(jarray array) {
    return jni::JniEnv::GetEnv()->GetArrayLength(array);
  }
};

template <typename SpannedType>
struct JniArrayHelper {};

template <>
struct JniArrayHelper<jboolean> : public JniArrayHelperBase {
  using AsArrayType = jbooleanArray;

  static inline jbooleanArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewBooleanArray(size);
  }

  static inline GetArrayElementsResult<jboolean> GetArrayElements(
      jarray array) {
    GetArrayElementsResult<jboolean> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetBooleanArrayElements(
        static_cast<jbooleanArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jboolean* native_ptr,
                                          bool copy_on_completion) {
    const jboolean copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseBooleanArrayElements(
        static_cast<jbooleanArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jbyte> : public JniArrayHelperBase {
  using AsArrayType = jbyteArray;

  static inline jbyteArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewByteArray(size);
  }

  static inline GetArrayElementsResult<jbyte> GetArrayElements(jarray array) {
    GetArrayElementsResult<jbyte> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetByteArrayElements(
        static_cast<jbyteArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jbyte* native_ptr,
                                          bool copy_on_completion) {
    const jbyte copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseByteArrayElements(
        static_cast<jbyteArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jchar> : public JniArrayHelperBase {
  using AsArrayType = jcharArray;

  static inline jcharArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewCharArray(size);
  }

  static inline GetArrayElementsResult<jchar> GetArrayElements(jarray array) {
    GetArrayElementsResult<jchar> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetCharArrayElements(
        static_cast<jcharArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jchar* native_ptr,
                                          bool copy_on_completion) {
    const jchar copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseCharArrayElements(
        static_cast<jcharArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jshort> : public JniArrayHelperBase {
  using AsArrayType = jshortArray;

  static inline jshortArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewShortArray(size);
  }

  static inline GetArrayElementsResult<jshort> GetArrayElements(jarray array) {
    GetArrayElementsResult<jshort> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetShortArrayElements(
        static_cast<jshortArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jshort* native_ptr,
                                          bool copy_on_completion) {
    const jshort copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseShortArrayElements(
        static_cast<jshortArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jint> : public JniArrayHelperBase {
  using AsArrayType = jintArray;

  static inline jintArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewIntArray(size);
  }

  static inline GetArrayElementsResult<jint> GetArrayElements(jarray array) {
    GetArrayElementsResult<jint> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetIntArrayElements(
        static_cast<jintArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, int* native_ptr,
                                          bool copy_on_completion) {
    const jint copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseIntArrayElements(
        static_cast<jintArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jlong> : public JniArrayHelperBase {
  using AsArrayType = jlongArray;

  static inline jlongArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewLongArray(size);
  }

  static inline GetArrayElementsResult<jlong> GetArrayElements(jarray array) {
    GetArrayElementsResult<jlong> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetLongArrayElements(
        static_cast<jlongArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jlong* native_ptr,
                                          bool copy_on_completion) {
    const jlong copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseLongArrayElements(
        static_cast<jlongArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jfloat> : public JniArrayHelperBase {
  using AsArrayType = jfloatArray;

  static inline jfloatArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewFloatArray(size);
  }

  static inline GetArrayElementsResult<jfloat> GetArrayElements(jarray array) {
    GetArrayElementsResult<jfloat> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetFloatArrayElements(
        static_cast<jfloatArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jfloat* native_ptr,
                                          bool copy_on_completion) {
    const jfloat copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseFloatArrayElements(
        static_cast<jfloatArray>(array), native_ptr, copy_back_mode);
  }
};

template <>
struct JniArrayHelper<jdouble> : public JniArrayHelperBase {
  using AsArrayType = jdoubleArray;

  static inline jdoubleArray NewArray(std::size_t size) {
    return jni::JniEnv::GetEnv()->NewDoubleArray(size);
  }

  static inline GetArrayElementsResult<jdouble> GetArrayElements(jarray array) {
    GetArrayElementsResult<jdouble> return_value;
    return_value.ptr_ = jni::JniEnv::GetEnv()->GetDoubleArrayElements(
        static_cast<jdoubleArray>(array), &return_value.is_copy);
    return return_value;
  }

  static inline void ReleaseArrayElements(jarray array, jdouble* native_ptr,
                                          bool copy_on_completion) {
    const jdouble copy_back_mode = copy_on_completion ? 0 : JNI_ABORT;
    jni::JniEnv::GetEnv()->ReleaseDoubleArrayElements(
        static_cast<jdoubleArray>(array), native_ptr, copy_back_mode);
  }
};

// Note, this requires both a jclass and a sample jobject to build from which
// is unlike any other new array construction.
template <>
struct JniArrayHelper<jobject> : public JniArrayHelperBase {
  static inline jobjectArray NewArray(std::size_t size, jclass class_id,
                                      jobject initial_element) {
    return jni::JniEnv::GetEnv()->NewObjectArray(size, class_id,
                                                 initial_element);
  }

  // The API of fetching objects only permits accessing one object at a time.
  // TODO CONST or at least nasty warning.
  static inline jobject GetArrayElement(jobjectArray array, std::size_t idx) {
    return jni::JniEnv::GetEnv()->GetObjectArrayElement(array, idx);
  };

  // The API of fetching objects only permits accessing one object at a time.
  static inline void SetArrayElement(jobjectArray array, std::size_t idx,
                                     jobject obj) {
    jni::JniEnv::GetEnv()->SetObjectArrayElement(array, idx, obj);
  };
};

}  // namespace jni


#include <tuple>
#include <type_traits>
#include <utility>

namespace jni {

inline constexpr struct NoClassLoader {
} kNoClassLoaderSpecified;

// Represents the compile time info we have about a class loader. In general,
// this is just the list of classes we expect to be loadable from a class loader
// and its parent loader.
//
// Classes from different loaders are typically incompatible, but Class loaders
// delegate classes that they cannot directly load to their parent loaders, so
// classes attached to two different class loaders will still be compatible if
// they were loaded by a shared parent loader.
template <typename ParentLoader_, typename... SupportedClasses_>
class ClassLoader {
 public:
  const ParentLoader_ parent_loader_;
  const std::tuple<SupportedClasses_...> supported_classes_;

  // TODO (b/143908983): Loaders should not be able to supply classes that their
  // parents do.
  explicit constexpr ClassLoader(
      ParentLoader_ parent_loader,
      SupportedClassSet<SupportedClasses_...> supported_class_set)
      : parent_loader_(parent_loader),
        supported_classes_(supported_class_set.supported_classes_) {}

  explicit constexpr ClassLoader(
      SupportedClassSet<SupportedClasses_...> supported_class_set)
      : parent_loader_(kDefaultClassLoader),
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


#include <mutex>
#include <atomic>
#include <functional>

namespace jni::metaprogramming {

// This class is intended to mimic a function local static except that it
// supports being reset. The object is constructed with a lambda and all
// subsequent loads will be thread safe, invoking the lambda once (and only
// once) if the currently stored value is T{0}.
//
// This class is thread-safe.  Loads will be cheap (after a potentially
// expensive initial init), stores are expensive.
template<typename T_>
class DoubleLockedValue {
 public:
  template <typename Lambda>
  inline T_ LoadAndMaybeInit(Lambda lambda) {
    // Typical case, value already initialised, perform cheap load and return.
    T_ return_value = value_.load(std::memory_order_acquire);

    if(return_value != T_{0}) {
      return return_value;
    }

    // Value was nil (uninitialised), perform heavy-weight lock.
    std::lock_guard<std::mutex> lock_guard {lock_};

    // Check another thread didn't race to lock before.
    return_value = value_.load(std::memory_order_acquire);
    if(return_value != T_{}) {
      return return_value;
    }

    // Perform the potentially expensive initialisation and return.
    return_value = lambda();
    value_.store(return_value, std::memory_order_release);
    return return_value;
  }

  // Sets the value to {0}.
  inline void Reset() {
    std::lock_guard<std::mutex> lock_guard {lock_};
    value_.store(0, std::memory_order_release);
  }

  // Sets the value to {0} and iff the value was not {0} prior to being torn
  // down, the teardown lambda will be invoked with this value.
  template <typename TeardownLambda>
  inline void Reset(TeardownLambda lambda) {
    std::lock_guard<std::mutex> lock_guard {lock_};
    auto val = value_.load();
    if(val != 0) {
      lambda(val);
      value_.store(0, std::memory_order_release);
    }
  }

 private:
  std::atomic<T_> value_ = {0};
  std::mutex lock_;
};

}  // namespace jni::metaprogramming


#include <utility>

namespace jni {

template <typename ReturnType, bool is_array = false>
class JniMethodInvoke {};

template <>
struct JniMethodInvoke<void, false> {
  template <typename... Ts>
  static void Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    jni::JniEnv::GetEnv()->CallVoidMethod(object, method_id,
                                          std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jboolean, false> {
  template <typename... Ts>
  static jboolean Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallBooleanMethod(object, method_id,
                                                    std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jint, false> {
  template <typename... Ts>
  static jint Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallIntMethod(object, method_id,
                                                std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jlong, false> {
  template <typename... Ts>
  static jlong Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallLongMethod(object, method_id,
                                                 std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jfloat, false> {
  template <typename... Ts>
  static jfloat Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallFloatMethod(object, method_id,
                                                  std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jdouble, false> {
  template <typename... Ts>
  static jdouble Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallDoubleMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jobject, false> {
  // This always returns a local reference which should be embedded in type
  // information wherever this is used.
  template <typename... Ts>
  static jobject Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

template <>
struct JniMethodInvoke<jstring, false> {
  template <typename... Ts>
  static jobject Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return jni::JniEnv::GetEnv()->CallObjectMethod(object, method_id,
                                                   std::forward<Ts>(ts)...);
  }
};

////////////////////////////////////////////////////////////////////////////////
// Array Types.
////////////////////////////////////////////////////////////////////////////////
template <>
struct JniMethodInvoke<jbooleanArray, true> {
  template <typename... Ts>
  static jbooleanArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jbooleanArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jbyteArray, true> {
  template <typename... Ts>
  static jbyteArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jbyteArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jcharArray, true> {
  template <typename... Ts>
  static jcharArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jcharArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jshortArray, true> {
  template <typename... Ts>
  static jshortArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jshortArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jintArray, true> {
  template <typename... Ts>
  static jintArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jintArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jfloatArray, true> {
  template <typename... Ts>
  static jfloatArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jfloatArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jdoubleArray, true> {
  template <typename... Ts>
  static jdoubleArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jdoubleArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jlongArray, true> {
  template <typename... Ts>
  static jlongArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jlongArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

template <>
struct JniMethodInvoke<jobjectArray, true> {
  template <typename... Ts>
  static jobjectArray Invoke(jobject object, jmethodID method_id, Ts&&... ts) {
    return static_cast<jobjectArray>(jni::JniEnv::GetEnv()->CallObjectMethod(
        object, method_id, std::forward<Ts>(ts)...));
  }
};

}  // namespace jni


#include <optional>

namespace jni {

// Used to detect RefBase in type proxying.
// This is useful, e.g. when you want to say "an object that might be passed"
// but the object's type (i.e. full name + loader information) is unknown.
template <typename NativeJavaType_>
class RefBaseTag {
 public:
  RefBaseTag(NativeJavaType_ object) : object_ref_(object) {}
  RefBaseTag(RefBaseTag&& rhs) : object_ref_(rhs.Release()) {}

  NativeJavaType_ Release() {
    NativeJavaType_ return_value = *object_ref_;
    object_ref_ = std::nullopt;
    return return_value;
  }

  explicit operator NativeJavaType_() const {
    if (object_ref_) {
      return *object_ref_;
    }
    return nullptr;
  }

 protected:
  std::optional<NativeJavaType_> object_ref_;
};

// Represents a runtime object with only Name information.  It is ephemeral and
// can be used a return value which is immediately wrapped in a rich object type
// (such as LocalObject, LocalString) which has the complete class definition.
//
// This can also be used as a temporary when passed into a function that accepts
// objects.  This ensures type correctness (names must match) but doesn't
// require the full class description be used when describing the function.
template <typename NativeJavaType_, const auto& class_v_ = kNoClassSpecified,
          const auto& loader_v_ = kDefaultClassLoader>
class RefBase : public RefBaseTag<NativeJavaType_> {
 public:
  static inline const char* name_ = class_v_.name_;

  using RefBaseTag<NativeJavaType_>::RefBaseTag;
  using RefBaseTag<NativeJavaType_>::operator NativeJavaType_;

  RefBase(RefBaseTag<NativeJavaType_>&& rhs)
      : RefBaseTag<NativeJavaType_>(std::move(rhs)) {}
  RefBase(RefBase&&) = default;
};

//==============================================================================
template <typename T>
using RefBaseT_t = typename T::RefBaseT;

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

template <typename SpanType>
class ArrayView {
 public:
  ArrayView(ArrayView&&) = delete;
  ArrayView(const ArrayView&) = delete;

  ArrayView(jarray array, bool copy_on_completion)
      : array_(array),
        get_array_elements_result_(
            JniArrayHelper<SpanType>::GetArrayElements(array)),
        copy_on_completion_(copy_on_completion) {}

  ~ArrayView() {
    JniArrayHelper<SpanType>::ReleaseArrayElements(
        array_, get_array_elements_result_.ptr_, copy_on_completion_);
  }

  SpanType* ptr() { return get_array_elements_result_.ptr_; }

 protected:
  const jarray array_;
  const GetArrayElementsResult<SpanType> get_array_elements_result_;
  const bool copy_on_completion_;
};

// This CTAD guide is required for materialising new ArrayViews from |Pin()|
// calls as move and copy constructors are deleted.
template <typename SpanType>
ArrayView(ArrayView<SpanType>&&) -> ArrayView<SpanType>;

}  // namespace jni

#include <type_traits>

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

template <typename T>
static constexpr bool kIsPrimitiveArrayType =
    metaprogramming::TupContains_v<T, PrimitiveKeys>;

// Simple type for proxying types used in the API (e.g. jint) to their
// corresponding array type (e.g. jintarray). Only use the former type when
// using JNI Bind (e.g. LocalArray<jint>, not LocalArray<jintArray>).
using RegularToArrayTypeMap = metaprogramming::TypeToTypeMap<
    std::tuple<jbyte, jchar, jshort, jint, jlong, jfloat, jdouble, jboolean,
               jobject, jarray>,
    std::tuple<jbyteArray, jcharArray, jshortArray, jintArray, jlongArray,
               jfloatArray, jdoubleArray, jbooleanArray, jobjectArray, jarray>>;

// Given a type, returns the corresponding array type (e.g. jint => jintArray).
template <typename T>
using RegularToArrayTypeMap_t =
    metaprogramming::TypeToTypeMapQuery_t<RegularToArrayTypeMap, T>;

template <typename T>
using ArrayToRegularTypeMap_t =
    metaprogramming::TypeToTypeMapQuery_t<RegularToArrayTypeMap::Invert, T>;

template <typename RawType>
struct Array;

////////////////////////////////////////////////////////////////////////////////
// Array Non-Object Implementation.
////////////////////////////////////////////////////////////////////////////////
template <typename RawType>
struct ArrayNonObjectTypeImpl {
  RawType raw_type_;

  constexpr ArrayNonObjectTypeImpl(RawType raw_type) : raw_type_(raw_type) {}

  template <typename RawTypeRhs>
  constexpr bool operator==(const Array<RawTypeRhs>& rhs) const {
    if constexpr (std::is_same_v<RawType, RawTypeRhs>) {
      return (raw_type_ == rhs.raw_type_);
    }
    return false;
  }

  template <typename RawTypeRhs>
  constexpr bool operator!=(const Array<RawTypeRhs>& rhs) const {
    return !(*this == rhs);
  }
};

// Primitive array implementaiton.
template <typename T, bool HoldsObject>
struct ArrayImpl : public ArrayNonObjectTypeImpl<T>,
                   ArrayTag<RegularToArrayTypeMap_t<T>> {
 public:
  using ArrayNonObjectTypeImpl<T>::ArrayNonObjectTypeImpl;
};

// Arrays of arrays are nominally special in that they index as jarrays but they
// will CDecl as their most base type.
template <typename T>
struct ArrayImpl<Array<T>, false> : public ArrayNonObjectTypeImpl<Array<T>>,
                                    ArrayTag<jarray> {
 public:
  using ArrayNonObjectTypeImpl<Array<T>>::ArrayNonObjectTypeImpl;
};

////////////////////////////////////////////////////////////////////////////////
// Array Object Implementation.
////////////////////////////////////////////////////////////////////////////////
template <typename RawType>
struct ArrayImpl<RawType, true> : public ArrayTag<jobjectArray> {
  RawType raw_type_;

  constexpr ArrayImpl(RawType raw_type) : raw_type_(raw_type) {}

  template <typename RawTypeRhs>
  constexpr bool operator==(const Array<RawTypeRhs>& rhs) const {
    if constexpr (std::is_same_v<RawType, RawTypeRhs>) {
      return (raw_type_ == rhs.raw_type_);
    }
    return false;
  }

  template <typename RawTypeRhs>
  constexpr bool operator!=(const Array<RawTypeRhs>& rhs) const {
    return !(*this == rhs);
  }
};

// This type correlates to those used in declarations,
//   e.g. Field { Array { Array { jint {} } } }.
template <typename RawType>
struct Array : public ArrayImpl<RawType, std::is_base_of_v<Object, RawType>> {
  constexpr Array(RawType raw_type)
      : ArrayImpl<RawType, std::is_base_of_v<Object, RawType>>(raw_type) {}
};

template <typename RawType>
Array(RawType) -> Array<RawType>;

template <typename RawType>
Array(Array<RawType>) -> Array<Array<RawType>>;

////////////////////////////////////////////////////////////////////////////////
// Rank Utilities.
////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct Rankifier {
  static constexpr bool kComputableRank = true;

  template <typename ArrayT>
  static inline constexpr std::size_t Rank(const ArrayT& maybe_array) {
    if constexpr (kIsArrayType<std::decay_t<decltype(maybe_array)>>) {
      return Rank(maybe_array.raw_type_) + 1;
    } else {
      return 0;
    }
  }
};

template <>
struct Rankifier<void> {
  static constexpr bool kComputableRank = false;

  template <typename ArrayT>
  constexpr std::size_t Rank(const ArrayT& maybe_array) {
    return 0;
  }
};

////////////////////////////////////////////////////////////////////////////////
// Strip Utilities.
// Takes an native array like type, and emits the innermost type.
// e.g. {Array<int>, Array{Array<int>}, Array{Array{Array<int>}} } => int.
////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct ArrayStrip {
  using type = T;
};

template <typename T>
using ArrayStrip_t = typename ArrayStrip<T>::type;

template <typename T>
struct ArrayStrip<Array<T>> {
  using type = ArrayStrip_t<T>;
};

template <typename T>
constexpr auto FullArrayStripV(const T& val) {
  if constexpr (kIsArrayType<std::decay_t<decltype(val)>>) {
    return FullArrayStripV(val.raw_type_);
  } else {
    return val;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Array Build from Span + Rank.
////////////////////////////////////////////////////////////////////////////////
template <typename RawType, std::size_t kRank>
struct ArrayFromRank {
  // Deferred
  template <std::size_t kRecursedRank>
  struct Helper {
    using type = Array<typename ArrayFromRank<RawType, kRank - 1>::type>;
  };

  template <>
  struct Helper<0> {
    using type = RawType;
  };

  using type = typename Helper<kRank>::type;
};

template <typename RawType, std::size_t kRank>
using ArrayFromRank_t = typename ArrayFromRank<RawType, kRank>::type;

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
// Because it is against style to import have a using declaration header wide,
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


#include <optional>
#include <string>
#include <string_view>

namespace jni {

static constexpr Class kJavaLangString{"java/lang/String"};

template <typename CrtpBase>
class StringRefBase : public RefBase<jstring, kJavaLangString> {
 public:
  using RefBaseT = RefBase<jstring>;
  StringRefBase(jstring object) : RefBase<jstring, kJavaLangString>(object) {}

  ~StringRefBase() {
    if (object_ref_) {
      static_cast<CrtpBase &>(*this).ClassSpecificDeleteObjectRef(*object_ref_);
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
        chars_(java_string_
                   ? JniEnv::GetEnv()->GetStringUTFChars(java_string,
                                                         /*isCopy=*/nullptr)
                   : nullptr) {}

  ~UtfStringView() {
    if (chars_) {
      JniHelper::ReleaseStringUTFChars(java_string_, chars_);
    }
  }

  UtfStringView(UtfStringView &&) = delete;
  UtfStringView(const UtfStringView &) = delete;

  // Returns a view into the pinned character string.
  // Warning: std::string_view{nullptr} is undefined behaviour and may crash.
  const std::string_view ToString() const { return std::string_view{chars_}; }

 private:
  const jstring java_string_;
  const char *chars_;
};

}  // namespace jni


#include <string_view>

namespace jni {

////////////////////////////////////////////////////////////////////////////////
// Constants for signature generation.
////////////////////////////////////////////////////////////////////////////////

static constexpr std::string_view kLeftBracket{"["};
static constexpr std::string_view kRightBracket{"]"};
static constexpr std::string_view kLeftParenthesis{"("};
static constexpr std::string_view kRightParenthesis{")"};
static constexpr std::string_view kInit{"<init>"};
static constexpr std::string_view kComma{","};
static constexpr std::string_view kSemiColon{";"};
static constexpr std::string_view kLetterL{"L"};

}  // namespace jni


#include <atomic>
#include <mutex>  // NOLINT
#include <vector>

namespace jni {

// See JvmRef::~JvmRef.
static std::vector<metaprogramming::DoubleLockedValue<jclass>*>&
GetDefaultLoadedClassList() {
  static auto* ret_val =
      new std::vector<metaprogramming::DoubleLockedValue<jclass>*>{};
  return *ret_val;
}

static inline jclass LoadClassFromObject(const char* name, jobject object_ref);

// Represents a fully specified class and class loader pair for a given Jvm.
// Because this is fully specified, classes associated with this type of loader
// can be programmatically torn down.
template <const auto& jvm_v_, size_t class_loader_idx_, size_t class_idx_>
class ClassRef {
 public:
  static_assert(kDefaultJvm != jvm_v_, "For default Jvm use DefaultClassRef.");

  static const auto& GetClassLoader() {
    return std::get<class_loader_idx_>(jvm_v_.class_loaders_);
  }

  static const auto& GetClass() {
    return std::get<class_idx_>(GetClassLoader().supported_classes_);
  }

  template <typename Lambda>
  static void PrimeJClassFromClassLoader(Lambda lambda) {
    class_ref_.LoadAndMaybeInit(lambda);
  }

  static jclass GetAndMaybeLoadClassRef(
      jobject optional_object_to_build_loader_from) {
    return class_ref_.LoadAndMaybeInit([=]() {
      return LoadClassFromObject(GetClass().name_,
                                 optional_object_to_build_loader_from);
    });
  }

  static jclass GetAlreadyLoadedClassRef() {
    return class_ref_.LoadAndMaybeInit([]() { return jclass{0}; });
  }

  static void MaybeReleaseClassRef() {
    class_ref_.Reset([](jclass maybe_loaded_class) {
      JniHelper::ReleaseClass(maybe_loaded_class);
    });
  }

 private:
  // A global reference to a jclass object that is returned from FindClass.
  // The variable has static storage because ClassIDs are static to the lifetime
  // of a JVM.  See GetAndMaybeLoadClassRef and MaybeReleaseClassRef.
  static inline metaprogramming::DoubleLockedValue<jclass> class_ref_;
};

template <const auto& class_loader_v, const auto& class_v>
class DefaultClassRef {
 public:
  template <typename Lambda>
  static void PrimeJClassFromClassLoader(Lambda lambda) {
    class_ref_.LoadAndMaybeInit(lambda);
  }

  // Gets the class ref for class_v, possibly loading it in the process.
  // Note, all class refs are pinned with GlobalRef and will be released
  // when JvmRef is torn down.
  //
  // This function is threadsafe, although the result is immediately stale.
  // This will never matter however, as part of the API's contract is call
  // Jvm::Destroy on all threads and release all objects in order to transition
  // to a new JVM.
  static jclass GetAndMaybeLoadClassRef(
      jobject optional_object_to_build_loader_from) {
    return class_ref_.LoadAndMaybeInit([=]() {
      GetDefaultLoadedClassList().push_back(&class_ref_);
      return LoadClassFromObject(class_v.name_,
                                 optional_object_to_build_loader_from);
    });
  }

 private:
  // A global reference to a jclass object that is returned from FindClass.
  // The variable has static storage because ClassIDs are static to the lifetime
  // of a JVM.  See GetAndMaybeLoadClassRef and MaybeReleaseClassRef.
  static inline metaprogramming::DoubleLockedValue<jclass> class_ref_;
};

// Default class loader, default Jvm.
template <const auto& class_v>
class DefaultClassRef<kDefaultClassLoader, class_v> {
 public:
  static jclass GetAndMaybeLoadClassRef(jobject /*unused*/ = nullptr) {
    static metaprogramming::DoubleLockedValue<jclass> return_value;
    return return_value.LoadAndMaybeInit([]() {
      // (b/174287131): This can be removed, but tests must be updated as they
      // guarantee all globals must be unpinned which this will break.
      GetDefaultLoadedClassList().push_back(&return_value);
      return JniHelper::PromoteLocalToGlobalClass(
          JniHelper::FindClass(class_v.name_));
    });
  }
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
      DefaultClassRef<kDefaultClassLoader,
                      kJavaLangClass>::GetAndMaybeLoadClassRef();

  jclass java_lang_class_loader_jclass =
      DefaultClassRef<kDefaultClassLoader,
                      kJavaLangClassLoader>::GetAndMaybeLoadClassRef();

  jclass class_of_object_jclass = JniHelper::GetObjectClass(object_ref);

  jmethodID get_class_loader_jmethod = JniHelper::GetMethodID(
      java_lang_class_jclass, "getClassLoader", "()Ljava/lang/ClassLoader;");

  jobject object_ref_class_loader_jobject = JniMethodInvoke<jobject>::Invoke(
      class_of_object_jclass, get_class_loader_jmethod);

  jmethodID load_class_jmethod =
      JniHelper::GetMethodID(java_lang_class_loader_jclass, "loadClass",
                             "(Ljava/lang/String;)Ljava/lang/Class;");

  jstring name_string = JniHelper::NewLocalString(name);
  jobject local_jclass_of_correct_loader = JniMethodInvoke<jobject>::Invoke(
      object_ref_class_loader_jobject, load_class_jmethod, name_string);
  jobject promote_jclass_of_correct_loader =
      JniHelper::PromoteLocalToGlobalObject(local_jclass_of_correct_loader);

  JniHelper::DeleteLocalObject(object_ref_class_loader_jobject);

  return static_cast<jclass>(promote_jclass_of_correct_loader);
}

// Selects among either |ClassRef| (which can be reset) or |DefaultClassRef|
// (which supports only Jvm during it's lifetime).
//
// jclass IDs are queried through FindClass for default class loaded
// objects, but derived from class loaders otherwise.  There are 4 options:
//
//   1) Default JVM, default class loader.
//   2) Non-default JVM, default class loader.
//   3) Default JVM, non-default class loader.
//   4) Non-default JVM, non default class loader.
template <const auto& jvm_v_, const auto& class_loader_v_, const auto& class_v_>
struct ClassRefSelector {
  using type =
      ClassRef<jvm_v_, jvm_v_.template IdxOfClassLoader<class_loader_v_>(),
               class_loader_v_.template IdxOfClass<class_v_>()>;
};

template <const auto& class_loader_v_, const auto& class_v_>
struct ClassRefSelector<kDefaultJvm, class_loader_v_, class_v_> {
  using type = DefaultClassRef<class_loader_v_, class_v_>;
};

template <const auto& jvm_v_, const auto& class_v_>
struct ClassRefSelector<jvm_v_, kDefaultClassLoader, class_v_> {
  using type = DefaultClassRef<kDefaultClassLoader, class_v_>;
};

template <const auto& class_v_>
struct ClassRefSelector<kDefaultJvm, kDefaultClassLoader, class_v_> {
  using type = DefaultClassRef<kDefaultClassLoader, class_v_>;
};

template <const auto& jvm_v_, const auto& class_loader_v_, const auto& class_v_>
using ClassRef_t =
    typename ClassRefSelector<jvm_v_, class_loader_v_, class_v_>::type;

}  // namespace jni


namespace jni {

struct ArrayRefPrimitiveBaseTag {};

// Tag for non object array ref like tags (e.g. jintArray but not jobjectArray).
template <typename SpanType>
struct ArrayRefPrimitiveTag : ArrayRefPrimitiveBaseTag {};

// |SpanType| is primitive types like jint, jfloat, etc.
template <typename SpanType, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
class ArrayRef : public RefBaseTag<RegularToArrayTypeMap_t<SpanType>>,
                 ArrayRefPrimitiveTag<SpanType> {
 public:
  using Base = RefBaseTag<RegularToArrayTypeMap_t<SpanType>>;
  using Base::Base;

  ArrayView<SpanType> Pin(bool copy_on_completion = true) {
    return {*Base::object_ref_, copy_on_completion};
  }

  std::size_t Length() {
    return JniArrayHelper<SpanType>::GetLength(*Base::object_ref_);
  }
};

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class LocalObject;

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class ArrayRef<jobject, class_v_, class_loader_v_, jvm_v_>
    : public RefBaseTag<jobjectArray> {
 public:
  using Base = RefBaseTag<jobjectArray>;
  using Base::Base;

  std::size_t Length() {
    return JniArrayHelper<jobject>::GetLength(*Base::object_ref_);
  }

  LocalObject<class_v_, class_loader_v_, jvm_v_> Get(std::size_t idx) {
    return {JniArrayHelper<jobject>::GetArrayElement(*Base::object_ref_, idx)};
  }

  // Note: Globals are not permitted in a local array because it makes reasoning
  // about them confusing.
  //
  // TODO(b/406948932): Permit lvalues of locals and globals as technically
  // they're both viable (the scope will be extended as expected).
  void Set(std::size_t idx,
           LocalObject<class_v_, class_loader_v_, jvm_v_>&& val) {
    return JniArrayHelper<jobject>::SetArrayElement(*Base::object_ref_, idx,
                                                    val.Release());
  }
};

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
  using type = std::remove_pointer_t<decltype(
      Interleave<std::tuple<T0...>, std::tuple<T1...>>())>;
};

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


#include <string_view>

namespace jni {

// Helper to generate full signature information for a "selected" value, and
// possibly some container information.  Here, |Selector| is |MethodSelection|,
// |FieldSelection|, etc.
//
// Unfortunately, the type system does not permit passing subobjects, however,
// types can be used that represent a specific selection upon an object.  This
// consolidates all signature info.
//
// |Selector| must express a type alias for RawVal (e.g. jint, Class{...}, etc.)
// and a type alias |RawValT| which is std::decay_t<RawVal>;
template <typename Selector>
struct SelectorStaticInfo {
  template <std::size_t I>
  struct IthRawTypeMember {
    template <typename T>
    static constexpr const auto& Val(const T& val) {
      return IthRawTypeMember<I - 1>::Val(val.raw_type_);
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
  static constexpr inline const auto& Val() {
    if constexpr (Selector::kRank == 0) {
      return Selector::Val();
    } else {
      return IthRawTypeMember<kRank>::Val(Selector::Val());
    }
  }

  using RawValT = typename Selector::RawValT;

  static constexpr inline bool kIsObject = std::is_base_of_v<Object, RawValT>;
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

  template <std::size_t repeat_cnt>
  struct Repeat {
    static constexpr std::string_view val =
        metaprogramming::StringConcatenate_v<kLeftBracket,
                                             Repeat<repeat_cnt - 1>::val>;
  };

  template <>
  struct Repeat<0> {
    static constexpr std::string_view val = "";
  };

  static constexpr std::string_view kEmptyStr = "";
  static constexpr std::string_view kModifierStr =
      (kRank == 0) ? "" : Repeat<kRank>::val;

  static constexpr std::string_view UndecoratedTypeName() {
    if constexpr (kIsObject) {
      return metaprogramming::StringConcatenate_v<
          kLetterL, kTypeNameOrNothingIfNotAnObject, kSemiColon>;
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


namespace jni {

template <typename TUndecayed>
struct ProxyHelper;

// See jni::Proxy.
template <typename T>
using Proxy_t = typename ProxyHelper<T>::Proxy_t;

template <typename T>
using Index_t = typename ProxyHelper<T>::Index;

template <typename T, typename Overload = void>
using CDecl_t = typename ProxyHelper<T>::template CDecl<Overload>;

template <typename T, typename OverloadSelection>
using Return_t =
    typename ProxyHelper<T>::template AsReturn_t<OverloadSelection>;

template <typename T, typename ParamSelection>
using Arg_t = typename ProxyHelper<T>::template AsArg_t<ParamSelection>;

template <typename T>
using AsDecl_t = typename ProxyHelper<T>::AsDecl_t;

}  // namespace jni

namespace jni {

// Represents and possibly builds a runtime Java String object.
//
// In order to use a string in memory (as opposed to only using it for function
// arguments), "Pin" the string.
//
// Like jobjects, jstrings can be either local or global with the same ownership
// semantics.
class LocalString : public StringRefBase<LocalString> {
 public:
  friend class StringRefBase<LocalString>;

  LocalString(jobject java_string_as_object)
      : StringRefBase<LocalString>(
            static_cast<jstring>(java_string_as_object)) {}

  LocalString(std::string_view str)
      : LocalString(JniHelper::NewLocalString(str.data())) {}

  explicit LocalString(RefBaseTag<jstring> &&rhs)
      : LocalString(static_cast<jstring>(rhs.Release())) {}

  // Returns a StringView which possibly performs an expensive pinning
  // operation.  String objects can be pinned multiple times.
  UtfStringView Pin() { return {*RefBaseTag<jstring>::object_ref_}; }

 private:
  // Invoked through CRTP on dtor.
  void ClassSpecificDeleteObjectRef(jstring object_ref) {
    JniHelper::DeleteLocalObject(object_ref);
  }
};

}  // namespace jni

#include <cstddef>
#include <span>
#include <type_traits>

namespace jni {

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class LocalObject;

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class GlobalObject;

// Represents a an array object (e.g. int[], float[][], Object[], etc).
// Currently GlobalArrays do not exist, as reasoning about the lifecycles of the
// underlying objects is non-trivial, e.g. a GlobalArray taking a local object
// would result in a possibly unexpected extension of lifetime.
template <typename SpanType, std::size_t kRank = 1,
          const auto& class_v_ = kNoClassSpecified,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class LocalArray : public ArrayRef<SpanType, kNoClassSpecified,
                                   kDefaultClassLoader, kDefaultJvm> {
 public:
  using Base =
      ArrayRef<SpanType, kNoClassSpecified, kDefaultClassLoader, kDefaultJvm>;

  LocalArray(RegularToArrayTypeMap_t<SpanType> array) : Base(array) {}
  LocalArray(LocalArray<SpanType, kRank>&& rhs) : Base(rhs.Release()) {}

  ~LocalArray() {
    if (Base::object_ref_) {
      JniHelper::DeleteLocalObject(*Base::object_ref_);
    }
  }

  // TODO(b/406948932): Add span views for construction.
  LocalArray(std::size_t size)
      : Base(JniArrayHelper<SpanType>::NewArray(size)) {}
};

// For classes (only default class loaded objects supported).
// TODO(b/406948932): Add span views for construction.
template <std::size_t kRank_, const auto& class_v_, const auto& class_loader_v_,
          const auto& jvm_v_>
class LocalArray<jobject, kRank_, class_v_, class_loader_v_, jvm_v_>
    : public ArrayRef<jobject, class_v_, class_loader_v_, jvm_v_> {
 public:
  using Base = ArrayRef<jobject, class_v_, class_loader_v_, jvm_v_>;
  using ObjectClassRefT = ClassRef_t<jvm_v_, class_loader_v_, class_v_>;

  // Note: jintArray, jfloatArray, etc. are implicitly convertible to jarray.
  LocalArray(jobjectArray array) : Base(array) {}

  template <std::size_t kRank, const auto& class_v, const auto& class_loader_v,
            const auto& jvm_v>
  LocalArray(LocalArray<jobject, kRank, class_v, class_loader_v, jvm_v>&& rhs)
      : Base(rhs.Release()) {
    static_assert(kRank == kRank_ && class_v == class_v_ &&
                  class_loader_v == class_loader_v_);
  }

  // Construct from LocalObject lvalue (object is used as template).
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
      : Base(JniArrayHelper<jobject>::NewArray(
            size,
            ObjectClassRefT::GetAndMaybeLoadClassRef(
                static_cast<jobject>(local_object)),
            static_cast<jobject>(local_object))) {}

  // Same as above.
  template <template <const auto&, const auto&, const auto&>
            class ObjectContainer>
  LocalArray(std::size_t size,
             ObjectContainer<class_v_, class_loader_v_, jvm_v_>&& local_object)
      : Base(JniArrayHelper<jobject>::NewArray(
            size,
            ObjectClassRefT::GetAndMaybeLoadClassRef(
                static_cast<jobject>(local_object)),
            local_object.Release())) {}
};

template <template <const auto&, const auto&, const auto&>
          class ObjectContainer,
          const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
LocalArray(std::size_t,
           const ObjectContainer<class_v_, class_loader_v_, jvm_v_>&)
    -> LocalArray<jobject, class_v_, class_loader_v_, jvm_v_>;

template <typename SpanType>
LocalArray(std::size_t, SpanType)
    -> LocalArray<SpanType, 1, kNoClassSpecified, kDefaultClassLoader,
                  kDefaultJvm>;

template <const auto& kArrayVal>
struct LocalArrayBuildFromArray_Helper {};

template <typename TUndecayed>
struct ProxyHelper;

}  // namespace jni


namespace jni {

class GlobalString : public StringRefBase<GlobalString> {
 public:
  using ClassT = decltype(kJavaLangString);
  friend class StringRefBase<GlobalString>;

  GlobalString(GlobalString &&local_string)
      : StringRefBase<GlobalString>(
            JniHelper::PromoteLocalToGlobalString(local_string.Release())) {}

  GlobalString(std::string_view str)
      : StringRefBase<GlobalString>(JniHelper::PromoteLocalToGlobalString(
            JniHelper::NewLocalString(str.data()))) {}

  // Returns a StringView which possibly performs an expensive pinning
  // operation.  String objects can be pinned multiple times.
  UtfStringView Pin() { return {*RefBaseTag<jstring>::object_ref_}; }

 private:
  // Invoked through CRTP on dtor.
  void ClassSpecificDeleteObjectRef(jstring object_ref) {
    JniHelper::DeleteGlobalString(object_ref);
  }
};

}  // namespace jni


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
template <typename CrtpBase, const auto& tup_container_v, std::size_t container_size,
          typename TupContainerT, const auto TupContainerT::*nameable_member>
class QueryableMap
    : public QueryableMapBase<CrtpBase, tup_container_v, TupContainerT,
                              nameable_member,
                              std::make_index_sequence<container_size>> {};

template <typename CrtpBase, const auto& tup_container_v,
          const auto std::decay_t<decltype(tup_container_v)>::*nameable_member>
using QueryableMap_t =
    QueryableMap<CrtpBase, tup_container_v,
                 std::tuple_size_v<std::decay_t<decltype(
                     (tup_container_v.*nameable_member))>>,
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


namespace jni {

template <typename Raw>
struct FieldHelper {
  static Raw GetValue(const jobject object_ref, const jfieldID field_ref_);

  static void SetValue(const jobject object_ref, const jfieldID field_ref_,
                       Raw&& value);
};

template <>
inline jboolean FieldHelper<jboolean>::GetValue(const jobject object_ref,
                                                const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetBooleanField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jboolean>::SetValue(const jobject object_ref,
                                            const jfieldID field_ref_,
                                            jboolean&& value) {
  jni::JniEnv::GetEnv()->SetBooleanField(object_ref, field_ref_, value);
}

template <>
inline jbyte FieldHelper<jbyte>::GetValue(const jobject object_ref,
                                          const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetByteField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jbyte>::SetValue(const jobject object_ref,
                                         const jfieldID field_ref_,
                                         jbyte&& value) {
  jni::JniEnv::GetEnv()->SetByteField(object_ref, field_ref_, value);
}

template <>
inline jchar FieldHelper<jchar>::GetValue(const jobject object_ref,
                                          const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetCharField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jchar>::SetValue(const jobject object_ref,
                                         const jfieldID field_ref_,
                                         jchar&& value) {
  jni::JniEnv::GetEnv()->SetCharField(object_ref, field_ref_, value);
}

template <>
inline jshort FieldHelper<jshort>::GetValue(const jobject object_ref,
                                            const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetShortField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jshort>::SetValue(const jobject object_ref,
                                          const jfieldID field_ref_,
                                          jshort&& value) {
  jni::JniEnv::GetEnv()->SetShortField(object_ref, field_ref_, value);
}

template <>
inline jint FieldHelper<jint>::GetValue(const jobject object_ref,
                                        const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetIntField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jint>::SetValue(const jobject object_ref,
                                        const jfieldID field_ref_,
                                        jint&& value) {
  jni::JniEnv::GetEnv()->SetIntField(object_ref, field_ref_, value);
}

template <>
inline jlong FieldHelper<jlong>::GetValue(const jobject object_ref,
                                          const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetLongField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jlong>::SetValue(const jobject object_ref,
                                         const jfieldID field_ref_,
                                         jlong&& value) {
  jni::JniEnv::GetEnv()->SetLongField(object_ref, field_ref_, value);
}

template <>
inline jfloat FieldHelper<jfloat>::GetValue(const jobject object_ref,
                                            const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetFloatField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jfloat>::SetValue(const jobject object_ref,
                                          const jfieldID field_ref_,
                                          jfloat&& value) {
  jni::JniEnv::GetEnv()->SetFloatField(object_ref, field_ref_, value);
}

template <>
inline jdouble FieldHelper<jdouble>::GetValue(const jobject object_ref,
                                              const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetDoubleField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jdouble>::SetValue(const jobject object_ref,
                                           const jfieldID field_ref_,
                                           jdouble&& value) {
  jni::JniEnv::GetEnv()->SetDoubleField(object_ref, field_ref_, value);
}

template <>
inline jobject FieldHelper<jobject>::GetValue(const jobject object_ref,
                                              const jfieldID field_ref_) {
  return jni::JniEnv::GetEnv()->GetObjectField(object_ref, field_ref_);
}

template <>
inline void FieldHelper<jobject>::SetValue(const jobject object_ref,
                                           const jfieldID field_ref_,
                                           jobject&& new_value) {
  jni::JniEnv::GetEnv()->SetObjectField(object_ref, field_ref_, new_value);
}

}  // namespace jni


#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>

namespace jni {

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
class LocalObject;

template <typename SpanType, std::size_t kRank, const auto& class_v_,
          const auto& class_loader_v_, const auto& jvm_v_>
class LocalArray;

template <typename Overload>
struct ArrayHelper;

template <typename TUndecayed>
struct ProxyHelper;

// Default Proxy, all types and values are pure passthrough.
template <typename Key_>
struct ProxyBase {
  using Key = Key_;

  template <typename = void>
  using CDecl = Key_;

  template <typename>
  using AsReturn = Key_;

  using AsArg = std::tuple<Key_>;
  using AsDecl = std::tuple<Key_>;

  template <typename T>
  static auto ProxyAsArg(T&& t) {
    return std::forward<T>(t);
  }
};

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
};

template <typename BooleanType>
struct Proxy<BooleanType,
             typename std::enable_if_t<std::is_same_v<BooleanType, jboolean>>>
    : public ProxyBase<jboolean> {
  using AsArg = std::tuple<jboolean, bool>;
  using AsDecl = std::tuple<jboolean, bool>;
};

template <typename LongType>
struct Proxy<LongType,
             typename std::enable_if_t<std::is_same_v<LongType, jlong>>>
    : public ProxyBase<jlong> {
  using AsArg = std::tuple<long, jlong>;
  using AsDecl = std::tuple<long, jlong>;

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

template <typename JString>
struct Proxy<JString,
             typename std::enable_if_t<std::is_same_v<JString, jstring>>>
    : public ProxyBase<JString> {
  using AsArg = std::tuple<std::string, jstring, const char*, std::string_view>;

  template <typename Return>
  using AsReturn = LocalString;

  // These leak local instances of strings.  Usually, RAII mechanisms would
  // correctly release local instances, but here we are stripping that so it can
  // be used in a method.  This could be obviated by wrapping the calling scope
  // in a local stack frame.
  static jstring ProxyAsArg(jstring s) { return s; }
  static jstring ProxyAsArg(const char* s) { return LocalString{s}.Release(); }
  static jstring ProxyAsArg(std::string s) { return LocalString{s}.Release(); }
  static jstring ProxyAsArg(std::string_view s) {
    return LocalString{s}.Release();
  }
};

template <typename JObject>
struct Proxy<JObject,
             typename std::enable_if_t<std::is_same_v<JObject, jobject>>>
    : public ProxyBase<jobject> {
  using AsDecl = std::tuple<Object>;
  using AsArg = std::tuple<jobject, RefBaseTag<jobject>>;

  template <typename OverloadT>
  struct Helper {
    // It's illegal to initialise this type with a sub-object of another,
    // however, we can construct types with enough validation to guarantee
    // correctness.
    static constexpr Class kClass{OverloadT::GetReturn().raw_.name_};

    // TODO(b/174272629): Class loaders should also be enforced.
    using type = LocalObject<kClass, kDefaultClassLoader, kDefaultJvm>;
  };

  template <typename Overload>
  using AsReturn = typename Helper<Overload>::type;

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
// Array Proxy Definitions.
////////////////////////////////////////////////////////////////////////////////

// Proxy for arrays of arrays (e.g. Array{Array{int}}).
template <typename JArrayType>
struct Proxy<JArrayType,
             typename std::enable_if_t<std::is_same_v<JArrayType, jarray>>>
    : public ProxyBase<JArrayType> {
  using AsDecl = std::tuple<ArrayTag<jarray>>;
  using AsArg = std::tuple<jarray, ArrayTag<jarray>, RefBaseTag<jarray>>;

  template <typename Overload>
  using CDecl = typename ArrayHelper<Overload>::StrippedCDecl;

  template <typename Overload>
  using AsReturn = typename ArrayHelper<Overload>::AsReturn;

  static jarray ProxyAsArg(jarray arr) { return arr; };

  template <typename T>
  static jarray ProxyAsArg(T& t) {
    return jarray{t};
  };

  template <typename T, typename = std::enable_if_t<
                            std::is_base_of_v<RefBaseTag<jarray>, T>>>
  static jarray ProxyAsArg(T&& t) {
    return t.Release();
  };
};

template <typename SpanType>
struct ArrayRefPrimitiveTag;

// Primitive array like types such as jintArray, jfloatArray.
template <typename JArrayType>
struct Proxy<JArrayType,
             typename std::enable_if_t<kIsPrimitiveArrayType<JArrayType>>>
    : public ProxyBase<JArrayType> {
  // Non-array primitive type (e.g. jintArray => jint).
  using CDeclOfPrimitiveType = ArrayToRegularTypeMap_t<JArrayType>;

  using AsDecl = std::tuple<ArrayTag<JArrayType>>;
  using AsArg =
      std::tuple<JArrayType, RefBaseTag<JArrayType>, ArrayTag<JArrayType>,
                 ArrayRefPrimitiveTag<CDeclOfPrimitiveType>>;

  template <typename = void>
  using CDecl = RegularToArrayTypeMap_t<CDeclOfPrimitiveType>;

  template <typename Overload>
  using AsReturn = typename ArrayHelper<Overload>::AsReturn;

  static JArrayType ProxyAsArg(JArrayType arr) { return arr; };

  template <typename T>
  static JArrayType ProxyAsArg(T& t) {
    return JArrayType{t};
  };

  template <typename T, typename = std::enable_if_t<
                            std::is_base_of_v<RefBaseTag<JArrayType>, T>>>
  static JArrayType ProxyAsArg(T&& t) {
    return t.Release();
  };
};

// Arrays of objects.
template <typename JObjectArrayType>
struct Proxy<
    JObjectArrayType,
    typename std::enable_if_t<std::is_same_v<JObjectArrayType, jobjectArray>>>
    : public ProxyBase<JObjectArrayType> {
  using AsDecl = std::tuple<ArrayTag<jobjectArray>>;
  using AsArg = std::tuple<jobjectArray, ArrayTag<jobjectArray>,
                           RefBaseTag<jobjectArray>>;

  template <typename = void>
  using CDecl = jobjectArray;

  template <typename Overload>
  using AsReturn = typename ArrayHelper<Overload>::AsReturn;

  static jobjectArray ProxyAsArg(jobjectArray arr) { return arr; };

  template <typename T>
  static jobjectArray ProxyAsArg(T& t) {
    return jobjectArray{t};
  };

  template <typename T, typename = std::enable_if_t<
                            std::is_base_of_v<RefBaseTag<jobjectArray>, T>>>
  static jobjectArray ProxyAsArg(T&& t) {
    return t.Release();
  };
};

// This must be defined outside of Proxy so implicit definition doesn't occur.
template <typename Overload>
struct ArrayHelper {
  template <const auto& t>
  struct Helper {
    static constexpr auto val = FullArrayStripV(t.raw_type_);

    using StrippedCDecl = CDecl_t<std::decay_t<decltype(val)>>;
    using ConvertedCDecl = RegularToArrayTypeMap_t<StrippedCDecl>;
  };

  template <const auto& array_val>
  static constexpr auto LocalArrayBuildFromArray() {
    using RawT = std::decay_t<ArrayStrip_t<decltype(array_val.raw_type_)>>;
    constexpr std::size_t kRank =
        Rankifier<decltype(array_val)>::Rank(array_val);

    // TODO(b/143908983): Support multi-dimensional arrays.
    if constexpr (!std::is_same_v<CDecl_t<RawT>, jobject>) {
      return LocalArray<RawT, kRank, kNoClassSpecified>{nullptr};
    } else {
      return LocalArray<jobject, kRank, Helper<array_val>::val>{
          jobjectArray{nullptr}};
    }
  }

  static constexpr auto kVal{Overload::GetReturn().raw_};

  using StrippedCDecl = typename Helper<kVal>::ConvertedCDecl;
  using AsReturn = decltype(LocalArrayBuildFromArray<kVal>());
};

}  // namespace jni


#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace jni {

template <typename t1, typename t2 = void>
struct Proxy;

// Everything you are permitted to declare at method prototypes.
// Note, if the size can reasonably differ, the jtype is enforced by virtue of
// being a different type (i.e. you can't accidentally widen).
using AllKeys =
    std::tuple<void, jboolean, jbyte, jshort, jint, jfloat, jlong, jchar,
               jdouble, jstring, jobject, jarray, jobjectArray, jintArray,
               jbooleanArray, jbyteArray, jcharArray, jshortArray, jdoubleArray,
               jfloatArray, jlongArray>;

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

  template <typename Overload = void>
  using CDecl = typename Proxy_t::template CDecl<Overload>;

  template <typename Overload>
  using AsReturn_t = typename Proxy_t::template AsReturn<Overload>;

  template <typename ParamSelection>
  using AsArg_t = typename Proxy_t::AsArg;

  using AsDecl_t = typename Proxy_t::AsDecl;
};

////////////////////////////////////////////////////////////////////////////////
// MetaFunction Helpers.
////////////////////////////////////////////////////////////////////////////////
struct ProxyAsArgMetaFunc {
  template <typename T>
  using type = Arg_t<T, void>;
};

template <const auto& loader, const auto& class_v>
struct ProxyAsDeclMetaFunc {
  template <typename T>
  using type = AsDecl_t<T>;
};

}  // namespace jni

// Consumers of Proxy *must* include proxy defininitions after proxy.h. This is
// because Arrays define themselves using the proxies of other types.


namespace jni {

template <const auto& class_loader_v_, const auto& class_v_, size_t field_idx_>
struct FieldSelection {
  static constexpr const auto& GetClass() { return class_v_; }
  static constexpr const auto& GetClassLoader() { return class_loader_v_; }

  static constexpr auto& GetField() {
    return std::get<field_idx_>(class_v_.fields_);
  }
  static constexpr auto& Val() {
    return std::get<field_idx_>(class_v_.fields_);
  }

  static constexpr auto& GetReturn() {
    return std::get<field_idx_>(class_v_.fields_);
  }

  using FieldT = std::decay_t<decltype(GetField().raw_)>;
  using RawValT = std::decay_t<decltype(GetField().raw_)>;

  static constexpr bool kIsObject = std::is_base_of_v<Object, FieldT>;
  static constexpr std::size_t kRank = 1;

  static constexpr std::string_view NameOrNothingIfNotAnObject() {
    if constexpr (kIsObject) {
      return SelectorStaticInfo<
          FieldSelection>::TypeNameOrNothingIfNotAnObject();
    } else {
      return "";
    }
  }

  static constexpr std::string_view kNameOrNothingIfNotAnObject =
      NameOrNothingIfNotAnObject();

  static constexpr std::string_view FieldTypeName() {
    if constexpr (kIsObject) {
      return metaprogramming::StringConcatenate_v<
          kLetterL, kNameOrNothingIfNotAnObject, kSemiColon>;
    } else {
      return JavaTypeToString<FieldT>();
    }
  }

  static constexpr std::string_view kName = FieldTypeName();

  static constexpr std::string_view GetSignature() { return kName; }
};

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


#include <mutex>
#include <tuple>
#include <utility>

namespace jni {

// See JvmRef::~JvmRef.
static inline auto& GetDefaultLoadedMethodList() {
  static auto* ret_val =
      new std::vector<metaprogramming::DoubleLockedValue<jmethodID>*>{};
  return *ret_val;
}

template <typename Method, typename Overload>
struct OverloadRef {
  static constexpr std::string_view GetMethodSignature() {
    return Overload::GetOverloadSignature();
  }

  static jmethodID GetMethodID(jclass clazz) {
    static jni::metaprogramming::DoubleLockedValue<jmethodID> return_value;

    return return_value.LoadAndMaybeInit([=]() {
      if constexpr (Method::GetClassLoader() == kDefaultClassLoader) {
        GetDefaultLoadedMethodList().push_back(&return_value);
      }

      return jni::JniHelper::GetMethodID(clazz, Method::Name(),
                                         GetMethodSignature().data());

    });
  }
};

template <typename Method, typename Overload, typename Permutation>
struct PermutationRef {
  using ReturnProxied = typename Overload::AsReturn;
  using OverloadRef = OverloadRef<Method, Overload>;

  template <typename... Params>
  static ReturnProxied Invoke(jclass clazz, jobject object,
                              Params&&... params) {
    if constexpr (std::is_same_v<ReturnProxied, void>) {
      JniMethodInvoke<void>::Invoke(
          object, OverloadRef::GetMethodID(clazz),
          Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...);
    } else if constexpr (Method::kIsConstructor) {
      return {JniHelper::NewLocalObject(
          clazz, OverloadRef::GetMethodID(clazz),
          Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...)};
    } else {
      static constexpr bool is_array =
          kIsArrayType<decltype(Overload::GetReturn().raw_)>;

      return {JniMethodInvoke<typename Overload::CDecl, is_array>::Invoke(
          object, OverloadRef::GetMethodID(clazz),
          Proxy_t<Params>::ProxyAsArg(std::forward<Params>(params))...)};
    }
  }
};

//==============================================================================
// Helper class for ObjectRef to inherit from.
// Inheriting from MethodMapHelper::type exposes an operator() which keys on
// method names.
template <const auto& class_loader_v, const auto& class_v_, typename CrtpBase_>
struct MethodMapHelper {
  using MethodTup = std::decay_t<decltype(class_v_.methods_)>;
  using ClassTDecayed = std::decay_t<decltype(class_v_)>;

  using type = metaprogramming::InvocableMap<CrtpBase_, class_v_, ClassTDecayed,
                                             &ClassTDecayed::methods_>;
};

template <const auto& class_loader_v, const auto& class_v_, typename CrtpBase_>
using MethodMap_t =
    typename MethodMapHelper<class_loader_v, class_v_, CrtpBase_>::type;

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
template <const auto& class_loader_v_, const auto& class_v_, size_t I>
class FieldRef {
 public:
  using Raw = Raw_t<std::decay_t<decltype(std::get<I>(class_v_.fields_))>>;
  using FieldSelectionT = FieldSelection<class_loader_v_, class_v_, I>;

  explicit FieldRef(jclass class_ref, jobject object_ref)
      : class_ref_(class_ref), object_ref_(object_ref) {}

  FieldRef(const FieldRef&) = delete;
  FieldRef(const FieldRef&&) = delete;
  void operator=(const FieldRef&) = delete;

  static constexpr auto& GetField() { return std::get<I>(class_v_.fields_); }

  static constexpr std::string_view GetFieldSignature() {
    return FieldSelection<class_loader_v_, class_v_, I>::GetSignature();
  }

  // This method is thread safe.
  static jfieldID GetFieldID(jclass clazz) {
    static jni::metaprogramming::DoubleLockedValue<jfieldID> return_value;

    return return_value.LoadAndMaybeInit([=]() {
      if constexpr (class_loader_v_ == kDefaultClassLoader) {
        GetDefaultLoadedFieldList().push_back(&return_value);
      }

      return jni::JniHelper::GetFieldID(clazz, GetField().name_,
                                        GetFieldSignature().data());
    });
  }

  using ProxyForField = Proxy_t<Raw>;
  using CDeclForField = CDecl_t<Raw>;

  Return_t<Raw, FieldSelectionT> Get() {
    return FieldHelper<CDeclForField>::GetValue(object_ref_,
                                                GetFieldID(class_ref_));
  }

  template <typename T>
  void Set(T&& value) {
    FieldHelper<CDecl_t<Raw>>::SetValue(
        object_ref_, GetFieldID(class_ref_),
        ProxyForField::ProxyAsArg(std::forward<T>(value)));
  }

 private:
  const jclass class_ref_;
  const jobject object_ref_;
};

}  // namespace jni


#include <limits>
#include <type_traits>
#include <utility>

namespace jni {

static constexpr std::size_t kNoSelection = std::numeric_limits<size_t>::max();

// Represents an indexing into a specific class and method.
template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx>
struct MethodSelection;

// Represents an overload which itself may be a set of permutations.
template <typename MethodSelectionT, size_t overload_idx>
struct OverloadSelection;

// Represents a permutation (e.g. jstring => {std::string, const char*, etc...}
template <typename MethodSelectionT, typename OverloadSelectionT, size_t permutation_idx>
struct Permutation;

// Represents the exact selection of a specific parameter from a permutation.
template <typename MethodSelectionT, typename OverloadSelectionT, typename PermutationT,
          size_t param_idx>
struct ParamSelection;

// The the type of an exact selection parameter in a method as part of the class
// specification (vs. the selection of a parameter in some generated candidate).
template <typename MethodSelectionT, typename OverloadSelectionT,
          size_t param_idx>
struct InputParamSelection;

// Compares a ParamSelection (the type associated with an exact parameter of an
// exact permutation) and exposes a value if they are equal.
template <typename ParamSelectionT, typename Query>
struct ParamCompare;

// The type correlating to the selection of a return type of a method.
template <typename MethodSelectionT, typename OverloadSelectionT>
struct ReturnSelection;

// Represents the exact permutation selection for a set of arguments.
template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx, typename... Args>
struct PermutationSelectionForArgs;

////////////////////////////////////////////////////////////////////////////////
// Helper Aliases.
////////////////////////////////////////////////////////////////////////////////
template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx>
using MethodSelection_t =
    MethodSelection<class_loader_v_, class_v_, is_constructor, method_idx>;

template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx, typename... Args>
using PermutationSelectionForArgs_t =
    PermutationSelectionForArgs<class_loader_v_, class_v_, is_constructor,
                                method_idx, Args...>;

////////////////////////////////////////////////////////////////////////////////
// Implementation Details.
////////////////////////////////////////////////////////////////////////////////
template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx>
struct MethodSelection {
  static constexpr bool kIsConstructor = is_constructor;

  static constexpr const auto& GetClass() { return class_v_; }
  static constexpr const auto& GetClassLoader() { return class_loader_v_; }

  static constexpr const auto& GetMethod() {
    if constexpr (is_constructor) {
      static_assert(method_idx == 0,
                    "If using MethodSelection for a constructor, there is only "
                    "ever one method (\"<init>\"), set method_idx to 0.");
      return class_v_.constructors_;
    } else {
      return std::get<method_idx>(class_v_.methods_);
    }
  }

  static constexpr const auto& Name() {
    if constexpr (is_constructor) {
      return "<init>";
    } else {
      return std::get<method_idx>(class_v_.methods_).name_;
    }
  }

  static constexpr std::size_t NumOverloads() {
    if constexpr (is_constructor) {
      return std::tuple_size<decltype(class_v_.constructors_)>();
    } else {
      return std::tuple_size<decltype(GetMethod().invocations_)>();
    }
  }

  template <typename Is, typename... Ts>
  struct Helper;

  template <size_t... Is, typename... Ts>
  struct Helper<std::index_sequence<Is...>, Ts...> {
    static constexpr bool val =
        (OverloadSelection<MethodSelection, Is>::template OverloadViable<Ts...>() ||
         ...);

    // kNoSelection is the max of std::size_t, so, this essentially selects any
    // idx (if a valid one exists), or defaults to kNoSelection.
    static constexpr std::pair<std::size_t, std::size_t>
        overload_permutation_idx_if_valid{
            std::min({OverloadSelection<MethodSelection,
                               Is>::template OverloadIdxIfViable<Ts...>()...}),
            std::min(
                {OverloadSelection<MethodSelection,
                          Is>::template PermutationIdxIfViable<Ts...>()...})};
  };

  template <typename... Ts>
  static constexpr bool ArgSetViable() {
    return Helper<std::make_index_sequence<NumOverloads()>,
                  std::decay_t<Ts>...>::val;
  }

  // The method and permutation overload that is viable for a set of args, or
  // {kNoSelection, kNoSelection} if no permutation is valid.
  template <typename... Ts>
  static constexpr std::pair<size_t, size_t> IdxPair() {
    return Helper<std::make_index_sequence<NumOverloads()>,
                  std::decay_t<Ts>...>::overload_permutation_idx_if_valid;
  }

  template <typename... Ts>
  using FindOverloadSelection = OverloadSelection<MethodSelection, IdxPair<Ts...>().first>;

  template <typename... Ts>
  using FindPermutation = OverloadSelection<MethodSelection, IdxPair<Ts...>().second>;
};

template <typename MethodSelectionT, typename OverloadSelectionT>
struct ReturnSelection {
  using RawValT = ArrayStrip_t<
      Raw_t<std::decay_t<decltype(OverloadSelectionT::GetReturn())>>>;

  static inline constexpr std::size_t ComputeRank() {
    if constexpr (Rankifier<RawValT>::kComputableRank) {
      return Rankifier<RawValT>::Rank(OverloadSelectionT::GetReturn().raw_);
    } else {
      return 0;
    }
  }

  static constexpr std::size_t kRank = ComputeRank();

  static constexpr inline auto& Val() {
    return OverloadSelectionT::GetReturn().raw_;
  }
};

template <typename MethodSelectionT, typename OverloadSelectionT,
          size_t param_idx>
struct InputParamSelection {
  static constexpr inline const auto& Val() {
    return std::get<param_idx>(OverloadSelectionT::GetParams().values_);
  }

  using RawValT = ArrayStrip_t<std::decay_t<decltype(Val())>>;

  static constexpr std::size_t kRank = Rankifier<RawValT>::Rank(
      std::get<param_idx>(OverloadSelectionT::GetParams().values_));
};

template <typename MethodSelectionT, size_t overload_idx>
struct OverloadSelection {
  static constexpr Return kObjectWhenConstructed{
      Class{MethodSelectionT::GetClass().name_}};

  static constexpr const auto& GetReturn() {
    if constexpr (MethodSelectionT::kIsConstructor) {
      return kObjectWhenConstructed;
    } else {
      return std::get<overload_idx>(MethodSelectionT::GetMethod().invocations_)
          .return_;
    }
  }

  static constexpr const auto& GetParams() {
    if constexpr (MethodSelectionT::kIsConstructor) {
      return std::get<overload_idx>(MethodSelectionT::GetMethod()).params_;
    } else {
      return std::get<overload_idx>(MethodSelectionT::GetMethod().invocations_)
          .params_;
    }
  }

  // CDecl is the type used by the C API for a return type.
  using CDecl =
      CDecl_t<Raw_t<std::decay_t<decltype(GetReturn())>>, OverloadSelection>;

  // Return type is the richly decorated type returned (e.g LocalArray).
  using AsReturn =
      Return_t<Raw_t<std::decay_t<decltype(GetReturn())>>, OverloadSelection>;

  // Proxy every parameter argument as an argument that can be shown in a
  // function prototype.
  //
  // A void function generates no arguments, so we generate an empty tuple.
  using ParamsRaw = typename std::decay_t<decltype(GetParams())>::ParamsRawTup;
  using ParamsProxiedTemp =
      metaprogramming::InvokePerTupArg_t<ProxyAsArgMetaFunc, ParamsRaw>;
  using ParamsProxied =
      std::conditional_t<std::is_same_v<ParamsProxiedTemp, std::tuple<>>,
                         std::tuple<std::tuple<>>, ParamsProxiedTemp>;

  // Lastly, we create a sequence to iterate all viable permutations.
  using NBitSequence = metaprogramming::TupleUnroller_t<
      metaprogramming::GenerateBitSequenceFromTupSetsFunc, ParamsProxied>;

  static constexpr const size_t permutation_count =
      (NBitSequence::max_representable_size_ == 0
           ? 1
           : NBitSequence::max_representable_size_);

  template <typename Is, typename... Ts>
  struct Helper;

  // Iterator for Permutation
  template <size_t... Is, typename... Ts>
  struct Helper<std::index_sequence<Is...>, Ts...> {
    static_assert(sizeof...(Is) == permutation_count);

    static constexpr bool val =
        (Permutation<MethodSelectionT, OverloadSelection,
                     Is>::template PermutationViable<Ts...>() ||
         ...);

    static constexpr size_t first_valid_permutation = std::min(
        {Permutation<MethodSelectionT, OverloadSelection,
                     Is>::template PermutationIdxIfViable<Ts...>()...});
  };

  static constexpr size_t kNumParams =
      std::tuple_size_v<decltype(GetParams().values_)>;

  template <typename... Ts>
  static constexpr size_t PermutationIdxIfViable() {
    constexpr size_t num_params =
        std::tuple_size_v<decltype(GetParams().values_)>;
    if constexpr (sizeof...(Ts) != num_params) {
      return kNoSelection;
    } else {
      return Helper<std::make_index_sequence<permutation_count>,
                    Ts...>::first_valid_permutation;
    }
  }

  template <typename... Ts>
  static constexpr bool OverloadViable() {
    return PermutationIdxIfViable<Ts...>() != kNoSelection;
  }

  template <typename... Ts>
  static constexpr size_t OverloadIdxIfViable() {
    return OverloadViable<Ts...>() ? overload_idx : kNoSelection;
  }

  ////////////////////////////////////////////////////////////////////////////////
  // Static Signature Generation.
  ////////////////////////////////////////////////////////////////////////////////
  template <typename Is>
  struct ParamHelper;

  template <>
  struct ParamHelper<std::index_sequence<>> {
    static constexpr std::string_view val = "";
  };

  template <size_t... Is>
  struct ParamHelper<std::index_sequence<Is...>> {
    static constexpr std::string_view val =
        metaprogramming::StringConcatenate_v<
            SelectorStaticInfo<InputParamSelection<
                MethodSelectionT, OverloadSelection, Is>>::kTypeName...>;
  };

  static constexpr std::string_view kParamSignature =
      metaprogramming::StringConcatenate_v<
          kLeftParenthesis,
          ParamHelper<std::make_index_sequence<kNumParams>>::val,
          kRightParenthesis>;

  static constexpr std::string_view GetReturnSignature() {
    if constexpr (MethodSelectionT::kIsConstructor) {
      return "V";
    } else {
      return SelectorStaticInfo<
          ReturnSelection<MethodSelectionT, OverloadSelection>>::kTypeName;
    }
  }

  static constexpr std::string_view kReturnSignature = GetReturnSignature();

  static constexpr std::string_view GetOverloadSignature() {
    return metaprogramming::StringConcatenate_v<kParamSignature,
                                                kReturnSignature>;
  }
};

template <typename MethodSelectionT, typename OverloadSelectionT, size_t permutation_idx>
struct Permutation {
  using NBitSelection =
      metaprogramming::Increment_t<typename OverloadSelectionT::NBitSequence,
                                   permutation_idx>;

  template <size_t I>
  using Param = ParamSelection<MethodSelectionT, OverloadSelectionT, Permutation, I>;

  template <typename Is, typename... Ts>
  struct Helper;

  template <size_t... Is, typename... Ts>
  struct Helper<std::index_sequence<Is...>, Ts...> {
    static constexpr bool val = (Param<Is>::template viable<Ts> && ...);
  };

  template <typename... Ts>
  static constexpr bool PermutationViable() {
    return Helper<std::index_sequence_for<Ts...>, Ts...>::val;
  }

  template <typename... Ts>
  static constexpr size_t PermutationIdxIfViable() {
    return PermutationViable<Ts...>() ? permutation_idx : kNoSelection;
  }
};

template <typename MethodSelectionT, typename OverloadSelectionT, typename PermutationT,
          size_t param_idx>
struct ParamSelection {
  static constexpr size_t selection_idx =
      PermutationT::NBitSelection::template GetBit<param_idx>::value_;

  constexpr static auto& GetParam() {
    return std::get<param_idx>(OverloadSelectionT::GetParams().values_);
  }

  using ParamT = metaprogramming::TypeOfNthTupleElement_t<
      selection_idx, metaprogramming::TypeOfNthTupleElement_t<
                         param_idx, typename OverloadSelectionT::ParamsProxied>>;

  template <typename T>
  static constexpr bool viable = ParamCompare<ParamSelection, T>::val;
};

template <typename ParamSelectionT, typename Query>
struct ParamCompare {
  using ParamT = typename ParamSelectionT::ParamT;

  template <typename, typename Enable = void>
  struct Helper {
    static constexpr bool val = false;
  };

  template <typename T>
  struct Helper<T, std::enable_if_t<std::is_same_v<std::remove_cv_t<T>,
                                                   std::remove_cv_t<ParamT>>>> {
    static constexpr bool val = true;
  };

  // The partial specialisation to compare a Local or Global object
  // against the specific selected permutation.
  template <template <const auto&, const auto&, const auto&> class T,
            const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  struct Helper<
      T<class_v, class_loader_v, jvm_v>,
      std::enable_if_t<std::is_base_of_v<RefBaseTag<jobject>, ParamT> &&
                       std::is_base_of_v<RefBaseTag<jobject>,
                                         T<class_v, class_loader_v, jvm_v>>>> {
    // TODO(b/174272629): Exclude objects loaded by invalid loaders.
    static constexpr bool val =
        std::string_view(class_v.name_) ==
        std::string_view(ParamSelectionT::GetParam().name_);
  };

  // ArrayRef objects (i.e. lvalue and rvalues of |LocalArray|s).
  template <
      template <typename, std::size_t, const auto&, const auto&, const auto&>
      class T,
      typename SpanType, std::size_t kRank, const auto& class_v,
      const auto& class_loader_v, const auto& jvm_v>
  struct Helper<
      T<SpanType, kRank, class_v, class_loader_v, jvm_v>,
      std::enable_if_t<
          std::is_base_of_v<ArrayRefPrimitiveBaseTag,
                            std::decay_t<T<SpanType, kRank, class_v,
                                           class_loader_v, jvm_v>>> &&
          std::is_base_of_v<ArrayRefPrimitiveTag<SpanType>, ParamT>>> {
    static constexpr bool val = true;
  };

  // The partial specialisation to compare an object Array.
  template <
      template <typename, std::size_t, const auto&, const auto&, const auto&>
      class T,
      typename SpanType, std::size_t kRank, const auto& class_v,
      const auto& class_loader_v, const auto& jvm_v>
  struct Helper<
      T<SpanType, kRank, class_v, class_loader_v, jvm_v>,
      std::enable_if_t<std::is_base_of_v<RefBaseTag<jobjectArray>,
                                         T<SpanType, kRank, class_v,
                                           class_loader_v, jvm_v>> &&
                       std::is_base_of_v<RefBaseTag<jobjectArray>, ParamT>>> {
    static constexpr bool val =
        std::string_view(class_v.name_) ==
        std::string_view(ParamSelectionT::GetParam().raw_type_.name_);
  };

  static constexpr bool val = Helper<Query>::val;
};

template <const auto& class_loader_v_, const auto& class_v_,
          bool is_constructor, size_t method_idx, typename... Args>
struct PermutationSelectionForArgs {
  using MethodSelectionForArgs =
      MethodSelection_t<class_loader_v_, class_v_, is_constructor, method_idx>;
  using OverloadSelectionForArgs =
      typename MethodSelectionForArgs::template FindOverloadSelection<Args...>;
  using PermutationForArgs =
      typename MethodSelectionForArgs::template FindPermutation<Args...>;

  static constexpr bool kIsValidArgSet =
      MethodSelectionForArgs::template ArgSetViable<Args...>();

  using PermutationRef = PermutationRef<MethodSelectionForArgs, OverloadSelectionForArgs,
                                        PermutationForArgs>;
};

}  // namespace jni


#include <atomic>
#include <utility>

namespace jni {

template <const auto& jvm_v_>
class JvmRef;

class ThreadGuard;

// Helper for JvmRef to enforce correct sequencing of getting and setting
// process level static fo JavaVM*.
class JvmRefBase {
 protected:
  friend class ThreadGuard;

  JvmRefBase(JavaVM* vm) { process_level_jvm_.store(vm); }
  ~JvmRefBase() { process_level_jvm_.store(nullptr); }

  static JavaVM* GetJavaVm() { return process_level_jvm_.load(); }
  static void SetJavaVm(JavaVM* jvm) { process_level_jvm_.store(jvm); }

  static inline std::atomic<JavaVM*> process_level_jvm_ = nullptr;
};

// ThreadGuard attaches and detaches JNIEnv* objects on the creation of new
// threads.  All new threads which want to use JNI Wrapper must hold a
// ThreadGuard beyond the scope of all created objects.  If the ThreadGuard
// needs to create an Env, it will also detach itself.
class ThreadGuard {
 public:
  ~ThreadGuard() {
    thread_guard_count_--;
    if (thread_guard_count_ == 0 && detach_thread_when_all_guards_released_) {
      JvmRefBase::GetJavaVm()->DetachCurrentThread();
    }
  }

  ThreadGuard(ThreadGuard&) = delete;
  ThreadGuard(ThreadGuard&&) = delete;

  template <const auto& jvm_v_>
  friend class JvmRef;

  // This constructor must *never* be called before a |JvmRef| has been
  // constructed. It depends on static setup from |JvmRef|.
  [[nodiscard]] ThreadGuard() {
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
      detach_thread_when_all_guards_released_ = true;
    } else {
      detach_thread_when_all_guards_released_ = false;
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
  static inline thread_local bool detach_thread_when_all_guards_released_;
};

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
      (ClassRef<jvm_v_, ClassLoaderIdx, Is>::MaybeReleaseClassRef(), ...);
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
    auto& default_loaded_class_list = GetDefaultLoadedClassList();
    for (metaprogramming::DoubleLockedValue<jclass>* maybe_loaded_class_id :
         default_loaded_class_list) {
      maybe_loaded_class_id->Reset(
          [](jclass clazz) { JniHelper::ReleaseClass(clazz); });
    }
    default_loaded_class_list.clear();

    // Methods do not need to be released, just forgotten.
    auto& default_loaded_method_ref_list = GetDefaultLoadedMethodList();
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

 private:
  // Main thread has a JNIEnv just like every other thread.
  const ThreadGuard thread_guard_ = {};
};

}  // namespace jni


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
template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_>
class ObjectRef
    : public MethodMap_t<class_loader_v_, class_v_,
                         ObjectRef<jvm_v_, class_v_, class_loader_v_>>,
      public metaprogramming::QueryableMap_t<
          ObjectRef<jvm_v_, class_v_, class_loader_v_>, class_v_,
          &std::decay_t<decltype(class_v_)>::fields_>,
      public RefBase<jobject, class_v_, class_loader_v_> {
 protected:
  static_assert(
      class_loader_v_.template SupportedDirectlyOrIndirectly<class_v_>(),
      "This class is not directly or indirectly supported by this loader.");
  using RefBase = RefBase<jobject, class_v_, class_loader_v_>;

  ObjectRef() = delete;
  explicit ObjectRef(ObjectRef&& rhs) = default;
  ObjectRef(const ObjectRef& rhs) = delete;
  ObjectRef& operator=(const ObjectRef& rhs) = delete;

  jclass GetJClass() const {
    return ClassRef_t<jvm_v_, class_loader_v_,
                      class_v_>::GetAndMaybeLoadClassRef(*RefBase::object_ref_);
  }

 public:
  explicit ObjectRef(RefBaseTag<jobject>&& rhs) : RefBase(std::move(rhs)) {}

  // Invoked through CRTP from InvocableMap.
  template <size_t I, typename... Args>
  auto InvocableMapCall(const char* key, Args&&... args) const {
    using PermutationForArgs =
        PermutationSelectionForArgs_t<class_loader_v_, class_v_, false, I,
                                      Args...>;

    static_assert(PermutationForArgs::kIsValidArgSet,
                  "JNI Error: Invalid argument set.");

    return PermutationForArgs::PermutationRef::Invoke(
        GetJClass(), *RefBase::object_ref_, std::forward<Args>(args)...);
  }

  // Invoked through CRTP from QueryableMap.
  template <size_t I>
  auto QueryableMapCall(const char* key) const {
    return FieldRef<class_loader_v_, class_v_, I>{GetJClass(),
                                                  *RefBase::object_ref_};
  }
};

// Imbues constructors for ObjectRefs and handles calling the correct
// intermediate constructors.  Access to this class is constrainted for non
// default classloaders (see |ValidatorProxy|).
template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_>
class ConstructorValidator
    : public ObjectRef<jvm_v_, class_v_, class_loader_v_> {
 public:
  using Base = ObjectRef<jvm_v_, class_v_, class_loader_v_>;
  using Base::Base;

  // Objects can still be wrapped.  This could happen if a classloaded object
  // is built in Java and then passed through to JNI.
  ConstructorValidator(jobject obj) : Base(obj) {}

  template <const auto& jvm_v, const auto& class_loader_v>
  friend class ClassLoaderRef;

  static constexpr std::size_t kNumConstructors =
      std::tuple_size_v<decltype(class_v_.constructors_)>;

  template <typename... Args>
  struct Helper {
    // 0 is (always) used to represent the constructor.
    using type = PermutationSelectionForArgs_t<class_loader_v_, class_v_, true,
                                               0, Args...>;
  };

  template <typename... Args>
  using Permutation_t = typename Helper<Args...>::type;

  template <typename... Args,
            typename std::enable_if<sizeof...(Args) != 0, int>::type = 0>
  ConstructorValidator(Args&&... args)
      : Base(Permutation_t<Args...>::PermutationRef::Invoke(
                 Base::GetJClass(), *Base::object_ref_,
                 std::forward<Args>(args)...)
                 .Release()) {
    static_assert(Permutation_t<Args...>::kIsValidArgSet,
                  "You have passed invalid arguments to construct this type.");
  }

  ConstructorValidator()
      : Base(Permutation_t<>::PermutationRef::Invoke(Base::GetJClass(),
                                                     *Base::object_ref_)
                 .Release()) {
    static_assert(
        kNumConstructors != 0,
        "You are attempting to construct an object which has no Constructor "
        "defined.  If you intended to construct this object with no arguments, "
        "ensure you define a jni::Constructor with no arguments.");
  }
};

template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_>
struct ValidatorProxy
    : public ConstructorValidator<jvm_v_, class_v_, class_loader_v_> {
  ValidatorProxy(jobject obj) : Base(obj) {}

 protected:
  using Base = ConstructorValidator<jvm_v_, class_v_, class_loader_v_>;
  using Base::Base;
};

template <const auto& jvm_v_, const auto& class_v_>
struct ValidatorProxy<jvm_v_, kDefaultClassLoader, class_v_>
    : public ConstructorValidator<jvm_v_, class_v_, kDefaultClassLoader> {
  using Base = ConstructorValidator<jvm_v_, class_v_, kDefaultClassLoader>;
  using Base::Base;
};

template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_>
using ObjectRefBuilder_t = ValidatorProxy<jvm_v_, class_v_, class_loader_v_>;

}  // namespace jni


#include <type_traits>

namespace jni {

// Represents a "Local" JNI reference of a class.  This object is *not* thread
// safe, but can be promoted to a GlobalObject which *is* thread safe.
template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class LocalObject
    : public ObjectRefBuilder_t<jvm_v_, class_v_, class_loader_v_> {
 public:
  using ObjectRefT = ObjectRefBuilder_t<jvm_v_, class_v_, class_loader_v_>;
  using ObjectRefT::ObjectRefT;

  LocalObject(jobject object) : ObjectRefT(object) {}

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  LocalObject(LocalObject<class_v, class_loader_v, jvm_v>&& rhs)
      : ObjectRefT(rhs.Release()) {
    static_assert(
        std::string_view(class_v.name_) == std::string_view(class_v_.name_),
        "You are attempting to initialise a LocalObject from another class "
        "type");
  }

  ~LocalObject() {
    if (ObjectRefT::object_ref_) {
      JniHelper::DeleteLocalObject(*ObjectRefT::object_ref_);
    }
  }
};

template <const auto& class_v_, const auto& class_loader_v_, const auto& jvm_v_>
LocalObject(LocalObject<class_v_, class_loader_v_, jvm_v_>&&)
    -> LocalObject<class_v_, class_loader_v_, jvm_v_>;

}  // namespace jni


namespace jni {

template <const auto& class_v_,
          const auto& class_loader_v_ = kDefaultClassLoader,
          const auto& jvm_v_ = kDefaultJvm>
class GlobalObject
    : public ObjectRefBuilder_t<jvm_v_, class_v_, class_loader_v_> {
 public:
  using ObjectRefT = ObjectRefBuilder_t<jvm_v_, class_v_, class_loader_v_>;
  using ObjectRefT::ObjectRefT;

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  GlobalObject(LocalObject<class_v, class_loader_v, jvm_v>&& local_object)
      : ObjectRefT(
            JniHelper::PromoteLocalToGlobalObject(jobject{local_object})) {
    static_assert(
        std::string_view(class_v.name_) == std::string_view(class_v_.name_),
        "You are attempting to initialise a LocalObject from another class "
        "type");
  }

  template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
  GlobalObject(GlobalObject<class_v, class_loader_v, jvm_v>&& rhs)
      : ObjectRefT(rhs.Release()) {
    static_assert(
        std::string_view(class_v.name_) == std::string_view(class_v_.name_),
        "You are attempting to initialise a GlobalObject from another class "
        "type");
  }

  // Promotes then wraps a local jobject.  Prefer using regular API over this.
  explicit GlobalObject()
      : GlobalObject(JniHelper::PromoteLocalToGlobalObject(
            LocalObject<class_v_, class_loader_v_, jvm_v_>{}.Release())) {}

  GlobalObject(const GlobalObject&) = delete;
  GlobalObject(GlobalObject&& rhs) = default;

  ~GlobalObject() {
    if (ObjectRefT::object_ref_) {
      JniHelper::DeleteGlobalObject(*ObjectRefT::object_ref_);
    }
  }
};

template <const auto& class_v, const auto& class_loader_v, const auto& jvm_v>
GlobalObject(LocalObject<class_v, class_loader_v, jvm_v>&&)
    -> GlobalObject<class_v, class_loader_v, jvm_v>;

}  // namespace jni


#include <type_traits>

namespace jni {

template <const auto& jvm_v_, const auto& class_loader_v_>
class ClassLoaderRef
    : public ObjectRef<kDefaultJvm, kJavaLangClassLoader, kDefaultClassLoader> {
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
  ClassLoaderRef(jobject class_loader)
      : ObjectRef<kDefaultJvm, kJavaLangClassLoader, kDefaultClassLoader>(
            class_loader) {}

  static_assert(class_loader_v_ != kDefaultClassLoader,
                "Custom class loaders should not use the default class loader,"
                "objects will automatically use the default.");

  template <const auto& class_v, typename... Params>
  [[nodiscard]] auto BuildLocalObject(Params&&... params) {
    static_assert(
        !(ParentLoaderForClass<class_loader_v_, class_v>() == kNullClassLoader),
        "Cannot build this class with this loader.");

    if constexpr (ParentLoaderForClass<class_loader_v_, class_v>() !=
                  kDefaultClassLoader) {
      ClassRef_t<jvm_v_, class_loader_v_, class_v>::PrimeJClassFromClassLoader(
          [=]() {
            // Prevent the object (which is a runtime instance of a class) from
            // falling out of scope so it is not released.
            LocalObject loaded_class = (*this)("loadClass", class_v.name_);

            // We only want to create global references if we are actually going
            // to use it them so that they do not leak.
            jclass test_class{
                static_cast<jclass>(static_cast<jobject>(loaded_class))};
            return static_cast<jclass>(
                JniEnv::GetEnv()->NewGlobalRef(test_class));
          });
    }
    return LocalObject<class_v,
                       ParentLoaderForClass<class_loader_v_, class_v>(),
                       JvmForLoader<class_v>()>{
        std::forward<Params>(params)...};
  }

  template <const auto& class_v, typename... Params>
  [[nodiscard]] auto BuildGlobalObject(Params&&... params) {
    // TODO(b/174256299): Promotion of locals to globals is clumsy.
    LocalObject obj =
        BuildLocalObject<class_v>(std::forward<Params>(params)...);
    jobject promoted_local =
        JniHelper::PromoteLocalToGlobalObject(obj.Release());
    return GlobalObject<class_v,
                        ParentLoaderForClass<class_loader_v_, class_v>(),
                        JvmForLoader<class_v>()>{promoted_local};
  }
};

}  // namespace jni


namespace jni {

template <const auto& class_loader_v_, const auto& jvm_v_ = kDefaultJvm>
class LocalClassLoader : public ClassLoaderRef<jvm_v_, class_loader_v_> {
 public:
  using Base = ClassLoaderRef<jvm_v_, class_loader_v_>;

  LocalClassLoader(jobject class_loader)
      : ClassLoaderRef<jvm_v_, class_loader_v_>(class_loader) {}

  ~LocalClassLoader() {
    if (Base::object_ref_) {
      JniHelper::DeleteLocalObject(*Base::object_ref_);
    }
  }

 private:
  template <const auto&, const auto&, const auto&>
  friend class ObjectRef;
};

}  // namespace jni


namespace jni {

template <const auto& class_loader_v_, const auto& jvm_v_ = kDefaultJvm>
class GlobalClassLoader : public ClassLoaderRef<jvm_v_, class_loader_v_> {
 public:
  using Base = ClassLoaderRef<jvm_v_, class_loader_v_>;

  // TODO(b/174256299): Make "global" from jobject more intuitive.
  GlobalClassLoader(jobject class_loader)
      : ClassLoaderRef<jvm_v_, class_loader_v_>(
            JniHelper::PromoteLocalToGlobalObject(class_loader)) {}

  template <const auto& class_loader_v, const auto& jvm_v>
  GlobalClassLoader(GlobalClassLoader<class_loader_v, jvm_v>&& rhs)
      : Base(rhs.Release()) {}

  ~GlobalClassLoader() {
    if (Base::object_ref_) {
      JniHelper::DeleteGlobalObject(*Base::object_ref_);
    }
  }
};

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
