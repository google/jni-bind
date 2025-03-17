# Building JNI Bind with CMake

JNI Bind now includes a CMake build system to make it easier to integrate with CMake-based projects.

## Basic Usage

```bash
# Create a build directory
mkdir cmake-build
cd cmake-build

# Configure the project
cmake ..

# Build
cmake --build .

# Install (optional)
cmake --install .
```

## Options

The CMake configuration supports the following options:

- `JNI_BIND_BUILD_TESTS`: Build the test suite (requires JNI and GTest)
- `JNI_BIND_BUILD_EXAMPLE`: Build a simple example (requires JNI)

Example:
```bash
cmake -DJNI_BIND_BUILD_EXAMPLE=ON ..
```

## Integration with Your Project

### Method 1: Use the installed package

If you've installed JNI Bind using `cmake --install .`, you can use it in your CMake project like this:

```cmake
find_package(jni_bind REQUIRED)
target_link_libraries(your_target PRIVATE jni_bind::jni_bind)
```

### Method 2: Include as a subdirectory

You can also include JNI Bind directly in your project:

```cmake
add_subdirectory(path/to/jni-bind)
target_link_libraries(your_target PRIVATE jni_bind)
```

## Requirements

- CMake 3.14 or newer
- C++20 compiler
- JNI (for building tests and examples)
- GTest (for building tests)

## Troubleshooting

### JNI Not Found

If CMake cannot find JNI, you can specify the JNI include directories manually:

```bash
cmake -DJAVA_HOME=/path/to/java ..
```

Or:

```bash
cmake -DJAVA_INCLUDE_PATH=/path/to/java/include -DJAVA_INCLUDE_PATH2=/path/to/java/include/darwin ..
```

### Notes for macOS Users

On macOS, you might need to install Java and set JAVA_HOME:

```bash
export JAVA_HOME=$(/usr/libexec/java_home)
cmake ..
```