cmake_minimum_required(VERSION 3.0.0)
project(cfs_simulator VERSION 0.1.0 LANGUAGES C)
SET(CMAKE_C_COMPILER "/usr/bin/clang")
add_compile_options(-Wall -Werror -pedantic -fno-omit-frame-pointer -fsanitize=address)
add_link_options(-fno-omit-frame-pointer -fsanitize=address)

add_library(rbt SHARED rbt.c)

add_executable(cfs_simulator main.c)
target_link_libraries(cfs_simulator PRIVATE rbt)

add_executable(cfs_rbt_test rbt_test.c)
target_link_libraries(cfs_rbt_test PRIVATE rbt)
