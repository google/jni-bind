message("Detected compiler [${CMAKE_CXX_COMPILER_ID}]")

SET(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CXX_FLAGS "/Zc:__cplusplus /EHsc")
endif()

set(CMAKE_CXX_FLAGS "${CXX_FLAGS}"                CACHE STRING "" FORCE)
