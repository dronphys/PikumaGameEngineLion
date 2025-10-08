cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(PROJECT_NAME PikumaGameEngineLion)
project(${PROJECT_NAME})
add_compile_options(/utf-8)
set(CMAKE_CXX_STANDARD 17)


add_executable(${PROJECT_NAME} src/Main.cpp
        src/Logger/Logger.h
        src/Game/Game.h
        src/Game/Game.cpp
        src/Logger/Logger.cpp
        src/Systems/MovementSystem.h
        src/ECS/ECS.cpp
        src/ECS/ECS.h
        src/Components/TransformComponent.h
        src/ECS/Registry.cpp
        src/ECS/Registry.h
        src/Components/RigidBodyComponent.h
        src/Components/SpriteComponent.h
        src/Components/AllComponents.h
        src/Systems/RenderSystem.h
        src/AssetStore/AssetStore.cpp
        src/AssetStore/AssetStore.h
        src/Components/AnimationComponent.h
        src/Systems/AnimationSystem.h
        src/Components/BoxColliderComponent.h
        src/Systems/CollisionSystem.h
        src/Systems/RenderCollisionSystem.h
        src/Events/CollisionEvent.h
        src/EventBus/EventBus.h
        src/EventBus/Event.h
        src/Systems/DamageSystem.h
        src/Systems/KeyboardMovementSystem.h
        src/Events/KeyPressedEvent.h
        src/Components/KeyboardControlledComponent.h
        src/Components/CameraFollowComponent.h
        src/Systems/CameraMovementSystem.h
        src/Components/ProjectileEmitterComponent.h
        src/Systems/ProjectileEmitSystem.h
        src/Components/HealthComponent.h
        src/Components/ProjectileComponent.h
        src/Components/LifeSpanComponent.h
        src/Components/LifeSpanComponent.h
        src/Systems/LifeSpanSystem.h
        src/misc/QuadTree.h
        src/Components/TextLabelComponent.h
        src/Systems/RenderTextSystem.h
        src/Components/LinkComponent.h
        src/Systems/LinkSystem.h
        src/Systems/HealtBarSystem.h
        src/Systems/RenderGUISystem.h
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/libs)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/SDL2/include)


set(IMGUI_SRC
        libs/imgui/imgui.cpp
        libs/imgui/imgui_draw.cpp
        libs/imgui/imgui_tables.cpp
        libs/imgui/imgui_widgets.cpp
        libs/imgui/imgui_demo.cpp
        libs/imgui/imgui_impl_sdl2.cpp
        libs/imgui/imgui_impl_sdlrenderer2.cpp
)

target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui
)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/lua
)

target_link_libraries(${PROJECT_NAME}  PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/lua/lua54.lib
)

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


file(GLOB SDL2_DLLS "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/lib/x64/*.dll")

foreach(DLL ${SDL2_DLLS})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${DLL}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endforeach()