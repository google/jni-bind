/*
 * Copyright 2026 Google LLC
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

#ifndef JNI_BIND_IMPLEMENTATION_BIND_LANG_IMPORTS_H_
#define JNI_BIND_IMPLEMENTATION_BIND_LANG_IMPORTS_H_

// IWYU pragma: private, include "third_party/jni_wrapper/jni_bind.h"
// IWYU pragma: begin_exports

#include "bind_lang/id_type.h"
#include "bind_lang/no_idx.h"
#include "bind_lang/ref_base.h"

// IWYU pragma: end_exports

// Re-exports language-agnostic `bind_lang` vocabulary into `namespace jni` so
// that JNI Bind's public API and its implementation can continue to refer to
// them unqualified.
namespace jni {

using ::bind_lang::IdType;
using ::bind_lang::kNoIdx;

using ::bind_lang::RefBase;
using ::bind_lang::RefBaseBase;
using ::bind_lang::RefBaseT_t;

}  // namespace jni

#endif  // JNI_BIND_IMPLEMENTATION_BIND_LANG_IMPORTS_H_
