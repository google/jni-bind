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

#ifndef JNI_BIND_PROTO_BIND_H_
#define JNI_BIND_PROTO_BIND_H_

// IWYU pragma: begin_exports

#include "proto_dep.h"

// Shared Metaprogramming & Corpus.
#include "bind_lang/metaprogramming/corpus.h"
#include "bind_lang/metaprogramming/corpus_tag.h"
#include "bind_lang/metaprogramming/string_literal.h"

// Proto Porcelain Helpers.
#include "implementation/jni_helper/fake_test_constants.h"
#include "implementation/proto_helper/fake_proto_test_constants.h"
#include "implementation/proto_helper/field_value.h"
#include "implementation/proto_helper/lifecycle_proto.h"
#include "implementation/proto_helper/proto_env.h"
#include "implementation/proto_helper/proto_helper.h"

// Static Schema Declarations & Shared Proxy Machinery.
#include "bind_lang/id_type.h"
#include "bind_lang/no_idx.h"
#include "bind_lang/ref_base.h"
#include "implementation/descriptor_ref.h"
#include "implementation/proto_field.h"
#include "implementation/proto_field_ref.h"
#include "implementation/proto_id.h"
#include "implementation/proto_template.h"
#include "implementation/proto_type.h"
#include "implementation/proxy_definitions_proto.h"

// Runtime Message Bindings.
#include "implementation/message.h"
#include "implementation/message_ref.h"

// IWYU pragma: end_exports

#endif  // JNI_BIND_PROTO_BIND_H_
