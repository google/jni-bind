# Do not depend on any targets in this package.  Instead, depend //third_party/jni_wrapper:jni_bind
package(
    default_visibility = ["//:__subpackages__"],
)

licenses(["notice"])

filegroup(
    name = "headers_for_export",
    srcs = glob(["**/*.h"]),
)

exports_files(["LICENSE"])

cc_library(
    name = "java_lang_classes",
    hdrs = ["java_lang_classes.h"],
    deps = [
        "//:jni_dep",
        "//implementation:array",
        "//implementation:class",
        "//implementation:method",
        "//implementation:params",
        "//implementation:return",
    ],
)

cc_library(
    name = "java_util_classes",
    hdrs = ["java_util_classes.h"],
    deps = [
        ":java_lang_classes",
        "//:jni_dep",
        "//implementation:class",
        "//implementation:method",
        "//implementation:params",
        "//implementation:return",
    ],
)
