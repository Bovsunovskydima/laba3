cmake_minimum_required(VERSION 3.20)
project(ThreadScheduling)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

if (MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(prog src/main.cpp)

if (MSVC)
    target_compile_definitions(prog PRIVATE _UNICODE UNICODE)
endif()

find_package(Threads REQUIRED)
target_link_libraries(prog Threads::Threads)
