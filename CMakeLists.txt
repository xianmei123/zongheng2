cmake_minimum_required(VERSION 3.8)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

project(zongheng LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(include)
add_subdirectory(src)