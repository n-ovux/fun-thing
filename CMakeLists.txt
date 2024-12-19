cmake_minimum_required(VERSION 3.15..3.31)

set(SRC "./src")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 

project(fun LANGUAGES CXX)

find_package(raylib REQUIRED)
include(FetchContent)
FetchContent_Declare(
  raygui
  GIT_REPOSITORY https://github.com/raysan5/raygui
  GIT_TAG master
)
FetchContent_MakeAvailable(raygui)
find_package(GLEW REQUIRED)

add_executable(fun ${SRC}/main.cpp)
target_link_libraries(fun raylib)
target_include_directories(fun PRIVATE ${raygui_SOURCE_DIR}/src/)
target_link_libraries(fun GLEW)
