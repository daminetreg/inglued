cmake_minimum_required(VERSION 3.0.0)
project (inclusive VERSION "0.0.1")

#set (CMAKE_CXX_STANDARD 14)
add_compile_options(-std=c++14)

# Boost
find_package(Boost 1.61 COMPONENTS system filesystem)
include_directories (AFTER ${Boost_INCLUDE_DIRS})

# Shippedwith
include_directories(AFTER ${CMAKE_CURRENT_LIST_DIR}/deps/boost-process/include)
include_directories(AFTER ${CMAKE_CURRENT_LIST_DIR}/deps/nlohmann-json/)

add_executable(deps_meld src/deps-meld.cpp)
target_link_libraries(deps_meld  ${Boost_LIBRARIES})
