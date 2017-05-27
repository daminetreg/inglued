cmake_minimum_required(VERSION 3.0.0)

include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.18.65.tar.gz"
    SHA1 "59cd3223f10ee630bc3596709a22f2fea4926a7c"
)

project (inclusive VERSION "0.0.1")

#set (CMAKE_CXX_STANDARD 14)
add_compile_options(-std=c++14)

if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

# Almost-Static build on ubuntu (ease deployment)
if (LINUX) 
  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    SET(BUILD_SHARED_LIBRARIES OFF)
    link_libraries("-static-libgcc -static-libstdc++")
  endif()
endif() 

# Threads
find_package(Threads REQUIRED)

# Boost
hunter_add_package(Boost COMPONENTS system filesystem)
find_package(Boost 1.64 COMPONENTS system filesystem)
include_directories (AFTER ${Boost_INCLUDE_DIRS})

# Shippedwith
add_subdirectory(deps/mstch)
include_directories(${mstch_INCLUDE_DIR})

include_directories(AFTER ${CMAKE_CURRENT_LIST_DIR}/deps/boost-process/include)
include_directories(AFTER ${CMAKE_CURRENT_LIST_DIR}/deps/nlohmann-json/src)

include_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR}/src)
add_executable(inglued src/inglued.cpp)
target_link_libraries(inglued ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} mstch)
