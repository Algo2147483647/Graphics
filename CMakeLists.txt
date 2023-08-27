# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project (Graphics)

set(CMAKE_CXX_STANDARD 11)

include_directories(../matrix/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/color)
include_directories(${CMAKE_SOURCE_DIR}/include/geometry)
include_directories(${CMAKE_SOURCE_DIR}/include/grating)
include_directories(${CMAKE_SOURCE_DIR}/include/image_processing)
include_directories(${CMAKE_SOURCE_DIR}/include/modeling)
include_directories(${CMAKE_SOURCE_DIR}/include/ray_tracing)

file(GLOB_RECURSE SOURCES "src/grating/*.cpp")

add_executable (Graphics "test/main.cpp" ${SOURCES}  "test/Graphics2DTest.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Graphics PROPERTY CXX_STANDARD 20)
endif()