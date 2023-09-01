# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project (Graphics)

# Only set the C++ standard once
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/thirdparty/eigen)
include_directories(${CMAKE_SOURCE_DIR}/include/color)
include_directories(${CMAKE_SOURCE_DIR}/include/geometry)
include_directories(${CMAKE_SOURCE_DIR}/include/ray_tracing)
include_directories(${CMAKE_SOURCE_DIR}/include/grating/)
include_directories(${CMAKE_SOURCE_DIR}/test/Graphics2DTest.h)
include_directories(${CMAKE_SOURCE_DIR}/test/RayTracingTest.h)

file(GLOB_RECURSE SOURCES "src/grating/*.cpp")

add_executable (Graphics "test/main.cpp" ${SOURCES})
