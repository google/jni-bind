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

#ifndef JNI_BIND_OBJECT_REF_H
#define JNI_BIND_OBJECT_REF_H

#include <optional>
#include <string>
#include <type_traits>
#include <utility>

#include "class.h"
#include "class_ref.h"
#include "constructor.h"
#include "field_ref.h"
#include "jni_type_proxy.h"
#include "method_ref.h"
#include "ref_base.h"
#include "class_loader.h"
#include "jni_dep.h"
#include "jni_helper/jni_env.h"
#include "jvm_ref.h"
#include "metaprogramming/invocable_map.h"
#include "metaprogramming/optional_wrap.h"
#include "metaprogramming/queryable_map.h"
#include "metaprogramming/tuple_manipulation.h"
#include "method_selection.h"

namespace jni {

// Represents a runtime instance of a JNI Object.  Instead of using this class
// directly, instead the more specialised types such as LocalObject,
// GlobalObject, etc.
//
// To call methods on the object, use the  operator(), to access fields, use
// operator[].
template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_,
          typename CrtpBase_>
class ObjectRef
    : public MethodMap_t<
          class_loader_v_, class_v_,
          ObjectRef<jvm_v_, class_v_, class_loader_v_, CrtpBase_>>,
      public metaprogramming::QueryableMap_t<
          CrtpBase_, class_v_, &std::decay_t<decltype(class_v_)>::fields_>,
      public RefBase<jobject, class_v_, class_loader_v_> {
 protected:
  static_assert(
      class_loader_v_.template SupportedDirectlyOrIndirectly<class_v_>(),
      "This class is not directly or indirectly supported by this loader.");
  using RefBase = RefBase<jobject, class_v_, class_loader_v_>;

  explicit ObjectRef(ObjectRef&& rhs) = default;

  ObjectRef(const ObjectRef& rhs) = delete;
  ObjectRef& operator=(const ObjectRef& rhs) = delete;

  ~ObjectRef() {
    if (RefBase::object_ref_) {
      static_cast<CrtpBase_&>(*this).ClassSpecificDeleteObjectRef(
          *RefBase::object_ref_);
    }
  }

  jclass GetJClass() const {
    return ClassRef_t<jvm_v_, class_loader_v_,
                      class_v_>::GetAndMaybeLoadClassRef(*RefBase::object_ref_);
  }

 public:
  explicit ObjectRef(RefBaseTag<jobject>&& rhs) : RefBase(std::move(rhs)) {}

  // Invoked through CRTP from InvocableMap.
  template <size_t I, typename... Args>
  auto InvocableMapCall(const char* key, Args&&... args) const {
    using MethodSelectionForTs =
        MethodSelection_t<class_loader_v_, class_v_, false, I>;
    using OverloadForTs =
        typename MethodSelectionForTs::template FindOverload<Args...>;
    using PermutationForTs =
        typename MethodSelectionForTs::template FindPermutation<Args...>;

    static_assert(MethodSelectionForTs::template ArgSetViable<Args...>(),
                  "JNI Error: Invalid argument set.");

    return PermutationRef<MethodSelectionForTs, OverloadForTs,
                          PermutationForTs>::Invoke(GetJClass(),
                                                    *RefBase::object_ref_,
                                                    std::forward<Args>(
                                                        args)...);
  }

  // Invoked through CRTP from QueryableMap.
  template <size_t I>
  auto QueryableMapCall(const char* key) const {
    return FieldRef<class_loader_v_, class_v_, I>{GetJClass(),
                                                  *RefBase::object_ref_};
  }
};

template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_,
          typename CrtpBase_, size_t I>
class ConstructorOverloadSet;

// The terminal constructor overload, specialised on std::tuple<>.  Note, this
// is when there are no more constructors left to represent, not, a constructor
// with an empty set of arguments.
template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_,
          typename CrtpBase_>
class ConstructorOverloadSet<jvm_v_, class_v_, class_loader_v_, CrtpBase_, 0>
    : public ObjectRef<jvm_v_, class_v_, class_loader_v_, CrtpBase_> {
 public:
  using Base = ObjectRef<jvm_v_, class_v_, class_loader_v_, CrtpBase_>;
  using Base::Base;
};

// Never instantiated (used to extract Args).
template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_,
          typename CrtpBase_, typename BaseClass_, size_t I, typename... Args>
class ImbueConstructor {};

// This class "imbues" a contructor onto a given type based on a tuple of args.
// E.g. Imbueing std::tuple<int, int> onto Foo gives Foo::Foo(int, int);
//
// The reason for this extra indirection is that variadic packs can't be
// trivially passed around, they have to be packed into tuples, and extracted
// through partial specialisation. ConstructorOverloadSet explodes the set of
// all jni Constructor's, and then they inherit from this class (as well as
// inheriting the constructor) to take a signature with the correct types.
//
// Lastly, this class will derive from BaseClass_ which allows for the types to
// be daisy chained.  Each constructor overload inherits from this class to
// imbue a constructor with the proper definition, and then uses this daisy
// chaining to progress through all remaining constructors.
//
template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_,
          typename CrtpBase_, typename BaseClass_, size_t I, typename... Args>
class ImbueConstructor<jvm_v_, class_v_, class_loader_v_, CrtpBase_,
                        BaseClass_, I, std::tuple<Args...>>
    : public BaseClass_ {
 public:
  using BaseClass_::BaseClass_;

  // ClassSpecificNewObjectRef are static throughout the class, but
  // GetConstructorMethodID is obviously particular to the constructor in use.
  //
  // The CrtpBase refers to the injected creation function that specialises to
  // the object being wrapped, e.g. NewLocalRef, NewGlobalRef, etc...
  //
  // Despite the signature being explicit, this will unfortunately allow
  // implicit conversions at the call site.  E.g. jint can implicitly convert
  // a char.  It might be possible to delete these constructors, although I
  // don't know if those deletions would shadow parent class constructors, and
  // you'd actually need to delete every proxyable permutation of Args (e.g.
  // ctor(jchar, jchar) needs to delete ctor(int, jchar), ctor(int, int),
  // ctor(jchar, int).  The code complexity isn't worth the gain.
  explicit ImbueConstructor(JniTypeProxyAsInputParamT_t<Args>&&... args)
      : BaseClass_(static_cast<CrtpBase_&>(*this).ClassSpecificNewObjectRef(
            ConstructorRef<class_v_, I - 1>::GetConstructorMethodID(
                BaseClass_::GetJClass()),
            JniTypeProxy<std::decay_t<Args>>::Proxy(
                std::forward<decltype(args)>(args))...)) {}
};

// This class exists to extract the parameters from each constructor and "imbue"
// them onto another type.  By providing the partial specialisation, they can
// effectively steal the types from a constructor and apply them onto the
// object.  See ImbueConstructor for details.
//
template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_,
          typename CrtpBase_, size_t I>
class ConstructorOverloadSet
    : public ImbueConstructor<
          jvm_v_, class_v_, class_loader_v_, CrtpBase_,
          ConstructorOverloadSet<jvm_v_, class_v_, class_loader_v_, CrtpBase_,
                                 I - 1>,
          I,
          metaprogramming::ExtractTupleFromType_t<std::decay_t<
              decltype(std::get<I - 1>(class_v_.constructors_))>>> {
 public:
  using NewConstructorT =
      std::decay_t<decltype(std::get<I - 1>(class_v_.constructors_))>;
  using ArgsAsTuple = metaprogramming::ExtractTupleFromType_t<NewConstructorT>;

  using Base =
      ImbueConstructor<jvm_v_, class_v_, class_loader_v_, CrtpBase_,
                       ConstructorOverloadSet<jvm_v_, class_v_, class_loader_v_,
                                              CrtpBase_, I - 1>,
                       I,
                       metaprogramming::ExtractTupleFromType_t<std::decay_t<
                           decltype(std::get<I - 1>(class_v_.constructors_))>>>;
  using Base::Base;
};

// Imbues constructors for class-loaded objects.
// Constructors are exposed through |jni::ClassLoaderRef::BuildLocalObject| as
// construction actually needs to happen through an instance of a classloader.
template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_,
          typename CrtpBase_>
class ConstructorValidator
    : public ConstructorOverloadSet<
          jvm_v_, class_v_, class_loader_v_, CrtpBase_,
          std::tuple_size_v<decltype(class_v_.constructors_)>> {
 public:
  using Base = ConstructorOverloadSet<
      jvm_v_, class_v_, class_loader_v_, CrtpBase_,
      std::tuple_size_v<decltype(class_v_.constructors_)>>;

  // Objects can still be wrapped.  This could happen if a classloaded object
  // is built in Java and then passed through to JNI.
  ConstructorValidator(jobject obj) : Base(obj) {}

  ConstructorValidator(RefBaseTag<jobject>&& rhs) : Base(std::move(rhs)) {}

 protected:
  template <const auto& jvm_v, const auto& class_loader_v, typename CrtpBase>
  friend class ClassLoaderRef;

  template <typename... Args>
  ConstructorValidator(Args&&... args) : Base(std::forward<Args>(args)...) {}
};

// Imbues constructors for objects not loaded through a classloader.
template <const auto& jvm_v_, const auto& class_v_, typename CrtpBase_>
class ConstructorValidator<jvm_v_, class_v_, kDefaultClassLoader, CrtpBase_>
    : public ConstructorOverloadSet<
          jvm_v_, class_v_, kDefaultClassLoader, CrtpBase_,
          std::tuple_size_v<decltype(class_v_.constructors_)>> {
 public:
  using Base = ConstructorOverloadSet<
      jvm_v_, class_v_, kDefaultClassLoader, CrtpBase_,
      std::tuple_size_v<decltype(class_v_.constructors_)>>;
  using Base::Base;
};

template <const auto& jvm_v_, const auto& class_v_, const auto& class_loader_v_,
          typename CrtpBase>
using ObjectRefBuilder_t =
    ConstructorValidator<jvm_v_, class_v_, class_loader_v_, CrtpBase>;

}  // namespace jni

#endif  // JNI_BIND_OBJECT_REF_H
