# cmake minimum version requirements
cmake_minimum_required(VERSION 3.19)

# project name
project(cpp_boost)

# cpp version
set(CMAKE_CXX_STANDARD 17)

# include all source files
file(GLOB SOURCES
        "memory/memory.cpp"
        "string_algorithm/string_algorithm.cpp"
        "utility/utility.cpp"
        "data_struct/data_struct.cpp"
        "assert_and_test/boost_test.cpp"
        "functions_and_callbacks/functions_and_callbacks.cpp"
        "concurrent_programming/*.cpp"
        )

# executable file
add_executable(cpp_boost common.h main.cpp ${SOURCES})

# vcpkg
find_package(Boost REQUIRED COMPONENTS
        unit_test_framework
        )

target_include_directories(cpp_boost PUBLIC
        ${Boost_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        )

target_link_libraries(cpp_boost ${Boost_LIBRARIES})