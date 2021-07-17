cmake_minimum_required(VERSION 3.20)
add_compile_options(-Wall -Wextra -pedantic -Werror)
project(MHCL VERSION 0.1 DESCRIPTION "mhcl, a cryptographic library" LANGUAGES CXX)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_subdirectory(test)
add_subdirectory(src) 
add_executable (demo src/main.cpp)
target_link_libraries (demo rsa)
target_link_libraries (demo biginteger)
