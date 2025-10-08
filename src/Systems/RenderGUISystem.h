//
// Created by kuyba on 10/8/2025.

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl2.h>
#include <imgui/imgui_impl_sdlrenderer2.h>
#include <SDL.h>
#include "../Components/AllComponents.h"
#ifndef RENDERGUISYSTEM_H
#define RENDERGUISYSTEM_H

class RenderGUISystem: public System {
public:
    RenderGUISystem() = default;
    void Update(Registry& registry, SDL_Renderer* renderer) {

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (ImGui::Begin("Spawn entities")) {
            static int enemyPosX = 0;
            static int enemyPosY = 0;
            ImGui::InputInt("x Position", &enemyPosX, 10);
            ImGui::InputInt("y Position", &enemyPosY, 10);

            SDL_Color green = {0,255,0};
            if (ImGui::Button("Create enemy")){
                Entity enemy = registry.CreateEntity();
                enemy.Group("enemies");
                // add components
                enemy.AddComponent<TransformComponent>(
                     glm::vec2(enemyPosX,enemyPosY)
                    ,glm::vec2(2.0, 2.0)
                    ,0.0);
                enemy.AddComponent<SpriteComponent>("truck-image",32,32,1);
                enemy.AddComponent<RigidBodyComponent>(glm::vec2(0.0,0.0));
                enemy.AddComponent<BoxColliderComponent>(64,64);
                enemy.AddComponent<ProjectileEmitterComponent>(200, 1000, 2000,1);
                enemy.AddComponent<HealthComponent>(100);
                Entity hpLabelEnemy = registry.CreateEntity();
                hpLabelEnemy.Group("hpBar");
                hpLabelEnemy.AddComponent<TextLabelComponent>("", "charriot-font-12", green);
                hpLabelEnemy.AddComponent<TransformComponent>(
                    glm::vec2(50.0,50.0),
                    glm::vec2(1.0, 1.0),
                    0.0,
                    false);
                hpLabelEnemy.AddComponent<LinkComponent>(glm::vec2(30,-10), enemy);
            }
        }
        ImGui::End();


        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    }

};
#endif //RENDERGUISYSTEM_H
