#ifndef JNI_BIND_SELECTOR_STATIC_INFO_H_
#define JNI_BIND_SELECTOR_STATIC_INFO_H_

#include <string_view>

#include "jni_helper/jni_typename_to_string.h"
#include "metaprogramming/string_concatenate.h"
#include "name_constants.h"
#include "object.h"
#include "return.h"

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
  // Strangely, the compiler refuses to peer through Val and loses the
  // constexpr-ness (i.e std::decay_t<decltype(Val())>; is not a constant
  // expression).
  static constexpr inline const auto& Val() { return Selector::Val(); }
  using RawValT = typename Selector::RawValT;

  static constexpr inline bool kIsObject = std::is_base_of_v<Object, RawValT>;

  static constexpr std::string_view TypeNameOrNothingIfNotAnObject() {
    if constexpr (kIsObject) {
      return Val().name_;
    } else {
      return "";
    }
  }

  static constexpr std::string_view kTypeNameOrNothingIfNotAnObject =
      TypeNameOrNothingIfNotAnObject();

  static constexpr std::string_view TypeName() {
    if constexpr (kIsObject) {
      return metaprogramming::StringConcatenate_v<
          kLetterL, kTypeNameOrNothingIfNotAnObject, kSemiColon>;
    } else {
      return JavaTypeToString<RawValT>();
    }
  }

  static constexpr std::string_view kTypeName = TypeName();
};

}  // namespace jni

#endif  // JNI_BIND_SELECTOR_STATIC_INFO_H_
