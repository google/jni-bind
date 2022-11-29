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
#ifndef JNI_BIND_JNI_BIND_H_
#define JNI_BIND_JNI_BIND_H_

// <jni.h> include helper.
#include "jni_dep.h"

// Headers for static definitions.
#include "implementation/array.h"
#include "implementation/array_type_conversion.h"
#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/constructor.h"
#include "implementation/field.h"
#include "implementation/id.h"
#include "implementation/jni_type.h"
#include "implementation/jvm.h"
#include "implementation/method.h"
#include "implementation/no_idx.h"
#include "implementation/params.h"
#include "implementation/return.h"
#include "implementation/selector_static_info.h"
#include "implementation/static.h"
#include "implementation/string.h"
#include "implementation/supported_class_set.h"

// Convenience headers for system libraries.
#include "class_defs/java_lang_classes.h"
#include "class_defs/java_util_classes.h"

// Headers for dynamic definitions.
#include "implementation/global_class_loader.h"
#include "implementation/global_object.h"
#include "implementation/global_string.h"
#include "implementation/jvm_ref.h"
#include "implementation/local_array.h"
#include "implementation/local_class_loader.h"
#include "implementation/local_object.h"
#include "implementation/local_string.h"

#endif  // JNI_BIND_JNI_BIND_H_
