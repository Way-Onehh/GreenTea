cmake_minimum_required(VERSION 3.12)
project(GreenTea VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY lib)

include_directories(.)

add_library(core source/Object.cpp)
add_library(UniqueDB source/UniqueDB.cpp)
add_library(Threadpool source/Threadpool.cpp)

add_executable(main main.cpp)
target_link_libraries(main PUBLIC core Threadpool)
