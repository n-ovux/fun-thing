cmake_minimum_required(VERSION 3.15..3.31)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) 
include(FetchContent)

project(fun LANGUAGES CXX C)
add_executable(fun ./src/main.cpp)
target_compile_options(fun PRIVATE -Wno-narrowing) # Required for raygui

# Add Raylib
find_package(raylib REQUIRED)
target_link_libraries(fun raylib)

# Add Raygui
FetchContent_Declare(
  raygui
  GIT_REPOSITORY https://github.com/raysan5/raygui
  GIT_TAG master
)
FetchContent_MakeAvailable(raygui)
target_include_directories(fun PRIVATE ${raygui_SOURCE_DIR}/src/)

# Add serialib
FetchContent_Declare(
  serialib
  GIT_REPOSITORY https://github.com/imabot2/serialib
  GIT_TAG master
)
FetchContent_MakeAvailable(serialib)
add_library(serialib STATIC ${serialib_SOURCE_DIR}/lib/serialib.cpp)
target_link_libraries(fun serialib)
target_include_directories(fun PRIVATE ${serialib_SOURCE_DIR}/lib/)

