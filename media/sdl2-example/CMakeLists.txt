cmake_minimum_required (VERSION 3.0)
project (sdl2-example)

FILE(GLOB_RECURSE sdl2-example_SRC src/*.cpp)
FILE(GLOB_RECURSE sdl2-example_HEADERS src/*.h)


add_executable(sdl2-example ${sdl2-example_SRC} ${sdl2-example_HEADERS})

target_link_libraries(sdl2-example pthread)
target_link_libraries(sdl2-example SDL2)
target_link_libraries(sdl2-example SDL2_image)
target_link_libraries(sdl2-example SDL2_ttf)
target_link_libraries(sdl2-example SDL2_mixer)

add_custom_command(TARGET sdl2-example POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   $<TARGET_FILE:sdl2-example> ${CMAKE_CURRENT_BINARY_DIR}/../USB/sdl2-example.elf)