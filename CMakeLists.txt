﻿cmake_minimum_required(VERSION 3.10.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")
cmake_policy(SET CMP0057 NEW)

project ("MenuMusicToggle")
set(developer "user95401")
set(version "v3.0.0")
set(description "Adds menu music toggle (like in 2.2) to Geometry Dash")
set(repository "https://github.com/user95401/MenuMusicToggle")
set(issues_url "https://discord.gg/89VGJH6QQg")
set(issues_info "U can write about it on <cb>user666's discord...</c>")
set(LIBRARY_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}") # dll output
set(GEODE_MOD_OUTPUT_PATH "D:\\Games\\user666\\Geometry.Dash.v07.24.2021\\geode\\mods") # geode mod output

#curly-eureka
include("C:\\Users\\Lenovo\\source\\curly-eureka\\CURLY-EUREKA-INCL.cmake")

#mod
file(GLOB_RECURSE src
src/* 
src/Layers/*
)
add_library(${PROJECT_NAME} SHARED ${src} ${curly_eureka_src})

#geode-makeup
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/geode geode-build)