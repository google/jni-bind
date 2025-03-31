# JNI
find_package(JNI REQUIRED)
if (NOT JNI_FOUND)
    message(FATAL_ERROR "JDK/JNI environment not found")
endif()

if (NOT TARGET jdk:jni)
    add_library(jdk:jni INTERFACE IMPORTED GLOBAL)

    set_target_properties(
        jdk:jni
        PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${JNI_INCLUDE_DIRS}"
            INTERFACE_LINK_LIBRARIES "${JNI_LIBRARIES}")
endif()

# GoogleTest
include(FetchContent)
FetchContent_Declare(googletest
                    GIT_REPOSITORY https://github.com/google/googletest.git
                    GIT_TAG        f8d7d77) # 1.14.0
FetchContent_MakeAvailable(googletest)