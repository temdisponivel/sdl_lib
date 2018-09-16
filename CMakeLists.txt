cmake_minimum_required(VERSION 3.9)
project(sdl_game C)

set(CMAKE_C_STANDARD 99)

set(DEPS_FOLDER ${CMAKE_SOURCE_DIR}/deps)

include_directories(${DEPS_FOLDER}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/engine)

set(PLATAFORM x86_64)

if(STATIC_LINKING)
    
    set(LINK_FLAGS "-L${DEPS_FOLDER}/lib/${PLATAFORM}/static -lmingw32 -lSDL2main -lSDL2 -lSDL2_mixer -lSDL2_ttf -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc")
    
else()
    
    set(LINK_FLAGS "-L${DEPS_FOLDER}/lib/${PLATAFORM}/dinamic -lmingw32 -lSDL2main -lSDL2.dll -lSDL2_mixer.dll -lSDL2_ttf.dll")
    
endif()

add_executable(sdl_game main.c engine/graphics.h engine/maths.h engine/maths.c engine/graphics.c engine/engine.h engine/input_system.h engine/defines.h engine/input.h engine/input.c engine/time.h engine/time.c engine/physics.h engine/physics.c engine/audio.h engine/audio.c engine/gui.h engine/gui.c engine/video.h engine/video.c engine/engine.c pong.c)
target_link_libraries(sdl_game ${LINK_FLAGS})

file(COPY "${DEPS_FOLDER}/bin/${PLATAFORM}/required_dlls/" DESTINATION .)
file(COPY "${CMAKE_SOURCE_DIR}/data" DESTINATION .)

if(NOT STATIC_LINKING)
    file(COPY "${DEPS_FOLDER}/bin/${PLATAFORM}/dinamic_dlls/" DESTINATION .)
endif()

