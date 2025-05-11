cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(PROJECT_NAME PikumaGameEngineLion)
project(${PROJECT_NAME})
add_compile_options(/utf-8)

add_executable(${PROJECT_NAME} src/Main.cpp
        src/Logger/Logger.h
        src/Game/Game.h
        src/Game/Game.cpp
        src/Logger/Logger.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include)

set(SDL2_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/x64")

find_library(SDL2_LIBRARY
        NAMES SDL2
        PATHS ${SDL2_LIB_DIR}
        NO_DEFAULT_PATH
)

find_library(SDL2MAIN_LIBRARY
        NAMES SDL2main
        PATHS ${SDL2_LIB_DIR}
        NO_DEFAULT_PATH
)

find_library(SDL2IMAGE_LIBRARY
        NAMES SDL2_image
        PATHS ${SDL2_LIB_DIR}
        NO_DEFAULT_PATH
)

find_library(SDL2TTF_LIBRARY
        NAMES SDL2_ttf
        PATHS ${SDL2_LIB_DIR}
        NO_DEFAULT_PATH
)

find_library(SDL2MIXER_LIBRARY
        NAMES SDL2_mixer
        PATHS ${SDL2_LIB_DIR}
        NO_DEFAULT_PATH
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ${SDL2_LIBRARY}
        ${SDL2MAIN_LIBRARY}
        ${SDL2IMAGE_LIBRARY}
        ${SDL2TTF_LIBRARY}
        ${SDL2MIXER_LIBRARY}
)


file(GLOB SDL2_DLLS "${CMAKE_CURRENT_SOURCE_DIR}/bin/*.dll")

foreach(DLL ${SDL2_DLLS})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endforeach()