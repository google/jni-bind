#ifndef JNI_BIND_CLASS_DEFS_JAVA_LANG_BOOLEAN_H_
#define JNI_BIND_CLASS_DEFS_JAVA_LANG_BOOLEAN_H_

#include "jni_bind.h"

namespace jni {

static constexpr Class kBoolean{
    "java/lang/Boolean",
    Constructor{jboolean{}},
    Constructor{kJavaLangString},
    Method{"booleanValue", Return{jboolean{}}, Params{}},
    Method{"valueOf", Return{Class{"java/lang/Boolean"}}, Params{jboolean{}}},
    Method{"valueOf", Return{Class{"java/lang/Boolean"}},
           Params{kJavaLangString}},
    Method{"toString", Return{kJavaLangString}, Params{jboolean{}}},
    Method{"toString", Return{kJavaLangString}, Params{}},
    Method{"parseBoolean", Return{jboolean{}}, Params{kJavaLangString}},
};

}  // namespace jni

#endif  // JNI_BIND_CLASS_DEFS_JAVA_LANG_BOOLEAN_H_
