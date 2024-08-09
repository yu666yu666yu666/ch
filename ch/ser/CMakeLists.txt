cmake_minimum_required(VERSION 3.10)
project(ser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找hiredis库
find_path(HIREDIS_INCLUDE_DIR hiredis)
find_library(HIREDIS_LIBRARY hiredis)

# 添加可执行文件目标
add_executable(mser mser.cpp)

# 将hiredis库链接到可执行文件目标
target_include_directories(mser PUBLIC ${HIREDIS_INCLUDE_DIR})
target_link_libraries(mser ${HIREDIS_LIBRARY})