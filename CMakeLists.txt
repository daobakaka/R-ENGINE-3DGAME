cmake_minimum_required(VERSION 3.16)
project(CppProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message(STATUS "CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")

# 递归查找项目根目录下所有 .cpp 和 .h 文件
file(GLOB_RECURSE SRC_CPP_FILES "${CMAKE_SOURCE_DIR}/*.cpp")
file(GLOB_RECURSE SRC_H_FILES "${CMAKE_SOURCE_DIR}/*.h")

set(SRC_FILES ${SRC_CPP_FILES} ${SRC_H_FILES})

if(NOT SRC_FILES)
    message(FATAL_ERROR "未找到源文件，请检查路径: ${CMAKE_SOURCE_DIR}")
endif()

# 添加可执行文件目标
add_executable(${PROJECT_NAME} ${SRC_FILES})

# 设置头文件搜索路径（这里头文件在项目根目录下）
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}")

# 如果希望在 Visual Studio 中组织文件分组，可以使用 source_group 命令按文件名或其他规则分组
source_group("Source Files" FILES ${SRC_CPP_FILES})
source_group("Header Files" FILES ${SRC_H_FILES})
