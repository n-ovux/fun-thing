cmake_minimum_required(VERSION 3.15..3.31)

set(SRC "./src")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

project(fun LANGUAGES CXX)

find_package(raylib REQUIRED)
find_package(GLEW REQUIRED)

include(FetchContent)
FetchContent_Declare(
  raygui
  GIT_REPOSITORY https://github.com/raysan5/raygui
  GIT_TAG master
)
FetchContent_MakeAvailable(raygui)

FetchContent_Declare(
  serialib
  GIT_REPOSITORY https://github.com/imabot2/serialib
  GIT_TAG master
)
FetchContent_MakeAvailable(serialib)
add_library(serialib STATIC ${serialib_SOURCE_DIR}/lib/serialib.cpp)

add_executable(fun ${SRC}/main.cpp)

target_compile_options(fun PRIVATE -Wno-narrowing)
target_include_directories(fun PRIVATE ${raygui_SOURCE_DIR}/src/)
target_include_directories(fun PRIVATE ${serialib_SOURCE_DIR}/lib/)
target_link_libraries(fun serialib)
target_link_libraries(fun raylib)
target_link_libraries(fun GLEW)
