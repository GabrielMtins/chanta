cmake_minimum_required(VERSION 3.5)
project(CampoMinadoPSP LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(GAME_DIR "${PROJECT_SOURCE_DIR}/game")
set(ENGINE_DIR "${PROJECT_SOURCE_DIR}/engine")

file(GLOB all_SRCS
		"${ENGINE_DIR}/src/*.c"
		"${GAME_DIR}/src/*.c"
        )

add_executable(${PROJECT_NAME} ${all_SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall
    -Wextra
    -Wpedantic
)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)
pkg_search_module(SDL2_MIXER REQUIRED SDL2_mixer)

find_library(MATH_LIBRARY m)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${SDL2_INCLUDE_DIRS}
	${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
	include/
	"${GAME_DIR}/include"
	"${ENGINE_DIR}/include"
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SDL2_LIBRARIES}
	${SDL2_IMAGE_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
	${MATH_LIBRARY}
)

if(PSP)
    # Create an EBOOT.PBP file
    create_pbp_file(
        TARGET ${PROJECT_NAME}
        ICON_PATH NULL
        BACKGROUND_PATH NULL
        PREVIEW_PATH NULL
        TITLE ${PROJECT_NAME}
        VERSION 01.00
    )
endif()
