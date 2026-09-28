# Do not depend on any targets in this package. Instead, depend on //third_party/jni_wrapper:proto_bind
package(
    default_visibility = ["//:__subpackages__"],
)

licenses(["notice"])

################################################################################
# Headers for export.
################################################################################
filegroup(
    name = "headers_for_export",
    srcs = glob(["**/*.h"]),
    visibility = ["//visibility:public"],
)
