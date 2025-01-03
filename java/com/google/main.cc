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

#include <dlfcn.h>
#include <sys/stat.h>

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "java_runtime_environment.h"  // NOLINT
#include "jni_bind.h"

// This binary is meant as a test to exercise a JVM loaded entirely from
// native. For now the path to the JVM so lib is hard coded, but it should be
// an argument to the binary.

namespace {

using ::jni_bind_sample_binary::JavaRuntimeEnvironment;

static constexpr int32_t kNumIterations = 1000;

void RunIterationsToCompletion(jni::JvmRef<jni::kDefaultJvm> *jvm_ref) {
  static constexpr jni::Class kRandomString{
      "com/google/RandomString", jni::Constructor{},
      jni::Method{"format", jni::Return<jstring>{}, jni::Params{}}};

  jni::GlobalObject<kRandomString> random_string;

  for (int i = 0; i < kNumIterations; ++i) {
    printf("Iteration %i: %s\n", i,
           random_string.Call<"format">().Pin().ToString().data());
  }
}

std::string LibNameToFullPath(std::string runfiles_path, std::string lib_name) {
  return runfiles_path + lib_name;
}

}  // namespace

int main(int argc, char *argv[]) {
  std::string execution_path = argv[0];
  std::string runfiles_path = execution_path + ".runfiles";

  std::cout
      << "##############################################################"
      << '\n'
      << "Application Run Directory: " << execution_path << "\n"
      << "Runfiles Directory: " << runfiles_path << "\n"
      << "Number of iterations: " << kNumIterations
      << "\n##############################################################"
      << std::endl;
  ;

  std::string full_class_collection =
      "-Djava.class.path=" +
      LibNameToFullPath(runfiles_path,
                        "/google3/third_party/jni_wrapper/java/com/google/"
                        "librandom_string_java.jar");

  auto java_runtime_environment = JavaRuntimeEnvironment::Build(
      "/usr/local/buildtools/java/jdk/lib/server/libjvm.so",
      full_class_collection);

  if (java_runtime_environment == std::nullopt) {
    return -1;
  }

  {
    jni::JvmRef<jni::kDefaultJvm> jvm_ref{(*java_runtime_environment).GetJvm()};
    RunIterationsToCompletion(&jvm_ref);
  }

  std::cout << '\n'
            << "##############################################################"
            << '\n'
            << "DONE." << '\n'
            << "##############################################################"
            << std::endl;

  return 0;
}
