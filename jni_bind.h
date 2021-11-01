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
#include "array.h"
#include "class.h"
#include "class_loader.h"
#include "constructor.h"
#include "field.h"
#include "jvm.h"
#include "method.h"
#include "params.h"
#include "return.h"
#include "string.h"
#include "supported_class_set.h"

// Headers for dynamic definitions.
#include "global_class_loader.h"
#include "global_object.h"
#include "global_string.h"
#include "jvm_ref.h"
#include "local_array.h"
#include "local_class_loader.h"
#include "local_object.h"
#include "local_string.h"

#endif  // JNI_BIND_JNI_BIND_H_
