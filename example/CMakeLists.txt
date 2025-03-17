cmake_minimum_required(VERSION 3.14)
project(jni_bind_example CXX)

# Set C++20 as the required standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# If built as part of the main project, use the library target
if(TARGET jni_bind)
    message(STATUS "Using jni_bind from parent project")
    set(JNI_BIND_LIB jni_bind)
else()
    # Otherwise, try to find the installed package
    message(STATUS "Looking for installed jni_bind")
    find_package(jni_bind REQUIRED)
    set(JNI_BIND_LIB jni_bind::jni_bind)
    
    # Also need to find JNI
    find_package(JNI REQUIRED)
endif()

# Create the example executable
add_executable(simple_example simple_example.cc)

# Link with JNI Bind
target_link_libraries(simple_example PRIVATE ${JNI_BIND_LIB})

# If we're not part of the main build, we need to link with JNI
if(NOT TARGET jni_bind)
    target_link_libraries(simple_example PRIVATE ${JNI_LIBRARIES})
endif()

message(STATUS "Example configured. Build with 'cmake --build .'")