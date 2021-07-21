cmake_minimum_required(VERSION 3.20)
add_compile_options(-Wall -Wextra -pedantic -Werror)
project(MHCL VERSION 0.1 DESCRIPTION "mhcl, a cryptographic library" LANGUAGES CXX)

set(CMAKE_MODULE_PATH "/mnt/v/r3dapple.github.io/encryption/mhcl")
find_package(GMP REQUIRED)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -pg -fprofile-arcs -ftest-coverage")
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "-pg")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_subdirectory(test)
add_subdirectory(src) 
add_executable(demo src/main.cpp)
target_link_libraries(demo rsa biginteger gmp)
