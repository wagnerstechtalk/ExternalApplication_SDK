cmake_minimum_required(VERSION 3.0)
project(hello-world)

add_executable(hello-world src/main.cpp)

add_custom_command(TARGET hello-world POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   $<TARGET_FILE:hello-world> ${CMAKE_CURRENT_BINARY_DIR}/../USB/hello-world.elf)
