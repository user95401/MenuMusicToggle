cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 17)

project ("MenuMusicToggle")

file(GLOB_RECURSE SOURCE_FILES src/*.cpp ../curly-eureka/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
  ../curly-eureka/minhook/include
  ../curly-eureka/gd.h/include
  ../curly-eureka/gd.h
  ../curly-eureka
)
add_subdirectory(../curly-eureka/cocos-headers cocos-headers-build)
add_subdirectory(../curly-eureka/minhook minhook-build)
target_link_libraries(${PROJECT_NAME} minhook cocos2d)

# this is so stupid
set(CMAKE_SIZEOF_VOID_P 4)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
  # ensure 32 bit on clang
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -target i386-pc-windows-msvc")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -target i386-pc-windows-msvc")
  add_definitions("--target=i386-pc-windows-msvc")
endif()