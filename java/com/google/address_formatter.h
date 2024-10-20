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

#ifndef JNI_BIND_JAVA_COM_GOOGLE_ADDRESS_FORMATTER_H_
#define JNI_BIND_JAVA_COM_GOOGLE_ADDRESS_FORMATTER_H_

#include "jni_bind.h"
#include "jni_dep.h"

namespace address::formatter {

class AddressFormatter {
 public:
  std::string Format() {
    return std::string{jni::LocalString{object_("format")}.Pin().ToString()};
  }

 private:
  static constexpr jni::Class kAddressFormatter{
      "com/google/AddressFormatter", jni::Constructor{},
      jni::Method{"format", jni::Return<jstring>{}, jni::Params{}}};

  jni::GlobalObject<kAddressFormatter> object_;
};

}  // namespace address::formatter

#endif  // JNI_BIND_JAVA_COM_GOOGLE_ADDRESS_FORMATTER_H_
