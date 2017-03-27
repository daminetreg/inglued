cmake_minimum_required(VERSION 3.0.0)
project (inclusive VERSION "0.0.1")

#set (CMAKE_CXX_STANDARD 14)
add_compile_options(-std=c++14)

# Threads
find_package(Threads REQUIRED)


# Boost
find_package(Boost 1.61 COMPONENTS system filesystem)
include_directories (AFTER ${Boost_INCLUDE_DIRS})


# Shippedwith
add_subdirectory(deps/mstch)
include_directories(${mstch_INCLUDE_DIR})

include_directories(AFTER ${CMAKE_CURRENT_LIST_DIR}/deps/boost-process/include)
include_directories(AFTER ${CMAKE_CURRENT_LIST_DIR}/deps/nlohmann-json/src)

include_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR}/src)
add_executable(inglued src/inglued.cpp)
target_link_libraries(inglued ${Boost_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} mstch)
