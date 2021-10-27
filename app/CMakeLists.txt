cmake_minimum_required (VERSION 3.13)

add_executable(program main.cc)
target_compile_options(program PUBLIC -Wall -Werror)
target_include_directories(program PUBLIC
  ${PROJECT_SOURCE_DIR}/third_party/json/single_include/
)
