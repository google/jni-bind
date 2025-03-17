cmake_minimum_required(VERSION 3.14)
project(jni_bind VERSION 1.2.0 LANGUAGES CXX)

# Set C++20 as the required standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Try to find JNI, but make it optional for header-only builds
find_package(JNI QUIET)

if(JNI_FOUND)
    include_directories(${JNI_INCLUDE_DIRS})
    message(STATUS "JNI found: ${JNI_INCLUDE_DIRS}")
else()
    message(STATUS "JNI not found. Header-only library will be built, but tests will be disabled.")
    set(JNI_BIND_BUILD_TESTS OFF CACHE BOOL "Build JNI Bind tests" FORCE)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/class_defs
    ${CMAKE_CURRENT_SOURCE_DIR}/implementation
    ${CMAKE_CURRENT_SOURCE_DIR}/implementation/jni_helper
    ${CMAKE_CURRENT_SOURCE_DIR}/metaprogramming
)

# Define the main library (header-only)
add_library(jni_bind INTERFACE)
target_include_directories(jni_bind INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# Only add JNI include directories if JNI was found
if(JNI_FOUND)
    target_include_directories(jni_bind INTERFACE ${JNI_INCLUDE_DIRS})
endif()

# Build tests if enabled and JNI is available
option(JNI_BIND_BUILD_TESTS "Build JNI Bind tests" OFF)

if(JNI_BIND_BUILD_TESTS AND JNI_FOUND)
    enable_testing()
    
    # Find GTest
    find_package(GTest QUIET)
    
    if(NOT GTEST_FOUND)
        message(STATUS "GTest not found. Tests will be disabled.")
    else()
        include_directories(${GTEST_INCLUDE_DIRS})
        
        # Helper function to add test
        function(add_jni_test test_name test_source)
            add_executable(${test_name} ${test_source})
            target_link_libraries(${test_name} PRIVATE 
                jni_bind
                ${JNI_LIBRARIES}
                ${GTEST_LIBRARIES}
                pthread
            )
            add_test(NAME ${test_name} COMMAND ${test_name})
        endfunction()
        
        # Add metaprogramming tests
        file(GLOB METAPROGRAMMING_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/metaprogramming/*_test.cc")
        foreach(TEST_SRC ${METAPROGRAMMING_TESTS})
            get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
            add_jni_test(${TEST_NAME} ${TEST_SRC})
        endforeach()
        
        # Add implementation tests
        file(GLOB IMPLEMENTATION_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/implementation/*_test.cc")
        foreach(TEST_SRC ${IMPLEMENTATION_TESTS})
            get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
            add_jni_test(${TEST_NAME} ${TEST_SRC})
        endforeach()
        
        # Add JNI helper tests
        file(GLOB JNI_HELPER_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/implementation/jni_helper/*_test.cc")
        foreach(TEST_SRC ${JNI_HELPER_TESTS})
            get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
            add_jni_test(${TEST_NAME} ${TEST_SRC})
        endforeach()
    endif()
elseif(JNI_BIND_BUILD_TESTS AND NOT JNI_FOUND)
    message(WARNING "JNI not found but tests were requested. Tests will be disabled.")
endif()

# Create an installation target
install(
    FILES jni_bind_release.h jni_bind.h
    DESTINATION include
)

# Package configuration
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/jni_bind-config-version.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/jni_bind-config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/jni_bind-config.cmake"
    INSTALL_DESTINATION lib/cmake/jni_bind
)

install(
    FILES 
        "${CMAKE_CURRENT_BINARY_DIR}/jni_bind-config-version.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/jni_bind-config.cmake"
    DESTINATION lib/cmake/jni_bind
)

# Export targets
install(
    TARGETS jni_bind
    EXPORT jni_bind-targets
    DESTINATION lib/cmake/jni_bind
)

install(
    EXPORT jni_bind-targets
    NAMESPACE jni_bind::
    DESTINATION lib/cmake/jni_bind
)

# Option to build a simple example
option(JNI_BIND_BUILD_EXAMPLE "Build a simple example" OFF)

if(JNI_BIND_BUILD_EXAMPLE AND JNI_FOUND)
    # Create a simple example subdirectory if it doesn't exist
    set(EXAMPLE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/example)
    if(NOT EXISTS ${EXAMPLE_DIR})
        file(MAKE_DIRECTORY ${EXAMPLE_DIR})
        
        # Create a simple example file
        file(WRITE ${EXAMPLE_DIR}/simple_example.cc 
"// Example of using JNI Bind
#include \"jni_bind.h\"
#include <iostream>

// This is a dummy example that doesn't run, but shows the JNI Bind syntax
int main() {
  // This would be populated from JNI_OnLoad in a real application
  JavaVM* jvm = nullptr;
  
  // Setup JVM reference (in a real app)
  // static auto jvm_ref = std::make_unique<jni::JvmRef<jni::kDefaultJvm>>(jvm);
  
  // Define a Java class
  static constexpr jni::Class kMyClass{
    \"com/example/MyClass\",
    jni::Method{\"doSomething\", jni::Return<jint>{}, jni::Params<jfloat, jstring>{}},
    jni::Field{\"intField\", jint{}},
  };
  
  // In a real application with a valid JVM:
  // jni::LocalObject<kMyClass> obj{};
  // int result = obj.Call<\"doSomething\">(1.5f, \"hello\");
  // obj.Access<\"intField\">().Set(42);
  
  std::cout << \"This is just an example of JNI Bind syntax.\\n\";
  std::cout << \"To use it in a real application, you need a valid JVM.\\n\";
  
  return 0;
}
")
        
        # Create a CMakeLists.txt for the example
        file(WRITE ${EXAMPLE_DIR}/CMakeLists.txt
"add_executable(simple_example simple_example.cc)
target_link_libraries(simple_example PRIVATE jni_bind ${JNI_LIBRARIES})
")
    endif()
    
    # Add the example subdirectory
    add_subdirectory(${EXAMPLE_DIR})
    
    message(STATUS "Example built. Run './example/simple_example' after building.")
elseif(JNI_BIND_BUILD_EXAMPLE AND NOT JNI_FOUND)
    message(WARNING "JNI not found but example was requested. Example will not be built.")
endif()