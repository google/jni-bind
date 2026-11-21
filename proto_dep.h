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

#ifndef JNI_BIND_PROTO_DEP_H_
#define JNI_BIND_PROTO_DEP_H_

// IWYU pragma: begin_exports
#include "third_party/protobuf/descriptor.h"
#include "third_party/protobuf/descriptor.pb.h"
#include "third_party/protobuf/dynamic_message.h"
#include "third_party/protobuf/message.h"

#ifndef PROTO2_OPENSOURCE
namespace google::protobuf {
using namespace ::proto2;  // NOLINT
}  // namespace google::protobuf
#endif  // !PROTO2_OPENSOURCE

// IWYU pragma: end_exports

#endif  // JNI_BIND_PROTO_DEP_H_
