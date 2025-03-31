message("Detected compiler [${CMAKE_CXX_COMPILER_ID}]")

SET(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CXX_FLAGS "/Zc:__cplusplus /EHsc")
#elseif(Clang)
# ...
endif()

set(CMAKE_CXX_FLAGS "${CXX_FLAGS}"                CACHE STRING "" FORCE)
#set(CMAKE_CXX_FLAGS_DEBUG ""                     CACHE STRING "" FORCE)
#set(CMAKE_CXX_FLAGS_RELEASE ""                   CACHE STRING "" FORCE)