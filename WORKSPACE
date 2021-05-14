load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

android_sdk_repository(name = "androidsdk")
android_ndk_repository(name = "androidndk")

register_toolchains("@androidsdk//:all")
register_toolchains("@androidndk//:all")

# Bazel Rules.
http_archive(
  name = "rules_cc",
  urls = ["https://github.com/bazelbuild/rules_cc/archive/40548a2974f1aea06215272d9c2b47a14a24e556.zip"],
  strip_prefix = "rules_cc-40548a2974f1aea06215272d9c2b47a14a24e556",
)

# GUnit Support
http_archive(
  name = "googletest",
  urls = ["https://github.com/google/googletest/archive/011959aafddcd30611003de96cfd8d7a7685c700.zip"],
  strip_prefix = "googletest-011959aafddcd30611003de96cfd8d7a7685c700",
)

# Android Test Support
ATS_COMMIT = "652578dec92e081d6d5c6d7afc8b4964da9ba860"
http_archive(
    name = "android_test_support",
    strip_prefix = "android-test-%s" % ATS_COMMIT,
    urls = ["https://github.com/android/android-test/archive/%s.tar.gz" % ATS_COMMIT],
)

load("@android_test_support//:repo.bzl", "android_test_repositories")
android_test_repositories()
