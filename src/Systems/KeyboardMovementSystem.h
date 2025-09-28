//
// Created by kuyba on 9/26/2025.
//

#ifndef KEYBOARDMOVEMENTSYSTEM_H
#define KEYBOARDMOVEMENTSYSTEM_H
#include "../ECS/ECS.h"
#include "../Components/AllComponents.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Logger/Logger.h"
#include "SDL.h"

class KeyboardMovementSystem: public System {
public:
    KeyboardMovementSystem() {
        RequireComponent<KeyboardControlledComponent>();
        RequireComponent<SpriteComponent>();
        RequireComponent<RigidBodyComponent>();
    }

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardMovementSystem::OnKeyPressed);
    }
    void OnKeyPressed(KeyPressedEvent& event) {
        for (auto entity: GetSystemEntities()) {
            const auto keyboardControl = entity.GetComponent<KeyboardControlledComponent>();
            auto& sprite = entity.GetComponent<SpriteComponent>();
            auto& rigidBody = entity.GetComponent<RigidBodyComponent>();

            switch (event.symbol) {
                case SDLK_UP:
                    rigidBody.velocity = keyboardControl.upVelocity;
                    sprite.srcRect.y = sprite.height * 0;
                    break;
                case SDLK_RIGHT:
                    rigidBody.velocity = keyboardControl.rightVelocity;
                    sprite.srcRect.y = sprite.height * 1;
                    break;
                case SDLK_DOWN:
                    rigidBody.velocity = keyboardControl.downVelocity;
                    sprite.srcRect.y = sprite.height * 2;
                    break;
                case SDLK_LEFT:
                    rigidBody.velocity = keyboardControl.leftVelocity;
                    sprite.srcRect.y = sprite.height * 3;
                    break;

            }
        }
    }

};
#endif //KEYBOARDMOVEMENTSYSTEM_H
