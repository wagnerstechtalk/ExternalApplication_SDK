cmake_minimum_required (VERSION 3.0)
project (hello-wtt)

FILE(GLOB_RECURSE hello-wtt_SRC src/*.cpp)
FILE(GLOB_RECURSE hello-wtt_SRC_HEADERS src/*.h)


add_executable(hello-wtt ${hello-wtt_SRC} ${hello-wtt_SRC_HEADERS})

target_link_libraries(hello-wtt pthread)
target_link_libraries(hello-wtt SDL2)
target_link_libraries(hello-wtt SDL2_image)
target_link_libraries(hello-wtt SDL2_ttf)

add_custom_command(TARGET hello-wtt POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   $<TARGET_FILE:hello-wtt> ${CMAKE_CURRENT_BINARY_DIR}/../USB/hello-wtt.elf)
