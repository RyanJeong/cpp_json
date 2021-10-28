cmake_minimum_required (VERSION 3.13)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_executable(program main.cc)
target_compile_options(program PUBLIC -Wall -Werror)
target_include_directories(program PUBLIC
  ${PROJECT_SOURCE_DIR}/third_party/json/single_include/
)
