if (APPLE AND CMAKE_SYSTEM_PROCESSOR STREQUAL arm64)
  set(CMAKE_C_COMPILER "/usr/bin/clang")
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
else() # WIP
endif()