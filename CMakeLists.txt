cmake_minimum_required(VERSION 3.16)
project(obs-media-auto-fix)

# 寻找官方 libobs 库
find_package(libobs REQUIRED)

# 编译 C 语言模块
add_library(${CMAKE_PROJECT_NAME} MODULE src/plugin-main.c)

# 链接核心库
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE obs::libobs)

# Windows 下去掉 "lib" 前缀，直接生成 obs-media-auto-fix.dll
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PREFIX "")
