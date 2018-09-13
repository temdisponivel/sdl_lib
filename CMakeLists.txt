cmake_minimum_required(VERSION 3.9)
project(sdl_game C)

set(CMAKE_C_STANDARD 99)

set(DEPS_FOLDER ${CMAKE_SOURCE_DIR}/deps)

include_directories(${DEPS_FOLDER}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(PLATAFORM x86_64)

add_executable(sdl_game main.c graphics.h maths.h maths.c graphics.c engine.h input_system.h defines.h input.h input.c)
target_link_libraries(sdl_game "-L${DEPS_FOLDER}/lib/${PLATAFORM}/ -lmingw32 -lSDL2main -lSDL2")
file(COPY "${DEPS_FOLDER}/bin/${PLATAFORM}/" DESTINATION .)
file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION .)