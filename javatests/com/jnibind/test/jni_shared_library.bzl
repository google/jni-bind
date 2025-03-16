# Helper macro to avoid duplicate library warnings by configuring platform-specific linker flags
def jni_shared_library(name, deps, testonly = True):
    native.cc_binary(
        name = name,
        testonly = testonly,
        linkshared = True,
        linkopts = select({
            "@platforms//os:macos": [
                # macOS specific flags to reduce duplicate library warnings
                "-Wl,-dead_strip",
                "-Wl,-dead_strip_dylibs",
                "-Wl,-bind_at_load",
            ],
            "@platforms//os:linux": [
                # Linux specific flags
                "-Wl,--as-needed",
                "-Wl,--allow-multiple-definition",
            ],
            "//conditions:default": [],
        }),
        deps = deps,
    )