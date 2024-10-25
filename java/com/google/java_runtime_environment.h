#ifndef JNI_BIND_JAVA_COM_GOOGLE_JAVA_RUNTIME_ENVIRONMENT_H_
#define JNI_BIND_JAVA_COM_GOOGLE_JAVA_RUNTIME_ENVIRONMENT_H_

#include <dlfcn.h>
#include <sys/stat.h>

#include <cstdio>
#include <optional>
#include <string_view>

#include "jni_bind.h"

namespace jni_bind_sample_binary {

// True if `path` is present on disk.
inline bool FileExists(std::string_view path) {
  struct stat buf;

  return stat(path.data(), &buf) == 0 && S_ISREG(buf.st_mode);
}

inline std::optional<void *> LoadLibrary(std::string_view lib_path) {
  void *lib_handle = dlopen(lib_path.data(), RTLD_NOW);
  if (lib_handle == nullptr) {
    printf("Failed to load libjvm so lib: %s\n", dlerror());
    return std::nullopt;
  }

  return lib_handle;
}

// Represents JVM so lib, normally launched by some kind of launcher, but in
// this sample the JVM is driven entirely by native. This class may be suitable
// for use beyond this toy binary, but needs more rigorous review.
class JavaRuntimeEnvironment {
  using CreateJvmFunc = jint (*)(JavaVM **, JNIEnv **, JavaVMInitArgs *);
  using GetJvmFunc = jint (*)(JavaVM **, jsize, jsize *);

 private:
  inline JavaRuntimeEnvironment(void *lib_handle,
                                std::string_view class_collection)
      : lib_handle_(lib_handle),
        create_function_(reinterpret_cast<CreateJvmFunc>(
            dlsym(lib_handle_, "JNI_CreateJavaVM"))),
        get_jvm_function_(reinterpret_cast<GetJvmFunc>(
            dlsym(lib_handle_, "JNI_GetCreatedJavaVMs"))) {
    auto *vm_options = new JavaVMOption[1];
    vm_options[0] = {.optionString =
                         const_cast<char *>(class_collection.data())};

    vm_args_ = JavaVMInitArgs{.version = JNI_VERSION_1_6,
                              .nOptions = 1,
                              .options = vm_options,
                              .ignoreUnrecognized = JNI_FALSE};

    jint result = create_function_(&jvm_, &jenv_, &vm_args_);
    if (result < 0) {
      printf("JNI_CreateJavaVM() failed\n");
      return;
    }

    success_loading_jvm_ = true;
    printf("JVM created.\n");
  }

 public:
  ~JavaRuntimeEnvironment() { dlclose(lib_handle_); }

  // Builds a `JavaRuntimeEnvironment`, or nothing if failure.
  static inline std::optional<JavaRuntimeEnvironment> Build(
      std::string_view jvm_so_lib, std::string_view class_collection) {
    if (!FileExists(jvm_so_lib.data())) {
      printf("libjvm so solib does not exist.");
      return std::nullopt;
    }

    auto lib_handle = LoadLibrary(jvm_so_lib);
    if (lib_handle == std::nullopt) {
      printf("Failed to load libjvm so lib.\n");
      return std::nullopt;
    }

    JavaRuntimeEnvironment ret(*lib_handle, class_collection);
    if (ret.success_loading_jvm_ == false) {
      printf("Initializing JVM failed.\n");
      return std::nullopt;
    }

    printf("Loading succeeded.\n");

    return ret;
  }

  JavaVM *GetJvm() { return jvm_; }

 private:
  void *lib_handle_;
  const CreateJvmFunc create_function_;
  const GetJvmFunc get_jvm_function_;
  JavaVMInitArgs vm_args_;

  JavaVM *jvm_;
  JNIEnv *jenv_;

  bool success_loading_jvm_ = false;
};

}  // namespace jni_bind_sample_binary

#endif  // JNI_BIND_JAVA_COM_GOOGLE_JAVA_RUNTIME_ENVIRONMENT_H_
