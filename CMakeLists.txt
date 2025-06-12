cmake_minimum_required(VERSION 3.20.0)

file(READ ${CMAKE_SOURCE_DIR}/JNI_BIND_VERSION.inc JNI_BIND_VERSION)
project(jni-bind VERSION "${JNI_BIND_VERSION}" LANGUAGES C CXX)

include(${CMAKE_SOURCE_DIR}/cmake/add_jni_test.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/compile_flags.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/find_dependencies.cmake)

# main target
add_library(jni_bind INTERFACE)

target_include_directories(jni_bind INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/class_defs
    ${CMAKE_CURRENT_SOURCE_DIR}/implementation
    ${CMAKE_CURRENT_SOURCE_DIR}/implementation/jni_helper
    ${CMAKE_CURRENT_SOURCE_DIR}/metaprogramming)

target_link_libraries(jni_bind INTERFACE jdk:jni)

# Testsuite
if (NOT TARGET gtest_main)
    return()
endif()

enable_testing()

file(GLOB METAPROGRAMMING_TESTS "${CMAKE_SOURCE_DIR}/metaprogramming/*_test.cc")
# file(GLOB METAPROGRAMMING_TESTS "${CMAKE_SOURCE_DIR}/metaprogramming/*ext_test.cc")
#file(GLOB METAPROGRAMMING_TESTS "${CMAKE_SOURCE_DIR}/metaprogramming/*pack*.cc")
foreach(TEST_SRC ${METAPROGRAMMING_TESTS})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_jni_test(${TEST_NAME} ${TEST_SRC})
endforeach()
#
#file(GLOB IMPLEMENTATION_TESTS "${CMAKE_SOURCE_DIR}/implementation/*_test.cc")
#foreach(TEST_SRC ${IMPLEMENTATION_TESTS})
#    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
#    add_jni_test(${TEST_NAME} ${TEST_SRC})
#endforeach()
#
#file(GLOB JNI_HELPER_TESTS "${CMAKE_SOURCE_DIR}/implementation/jni_helper/*_test.cc")
#foreach(TEST_SRC ${JNI_HELPER_TESTS})
#    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
#    add_jni_test(${TEST_NAME} ${TEST_SRC})
#endforeach()
