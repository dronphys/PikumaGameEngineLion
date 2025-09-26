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
    KeyboardMovementSystem() = default;

    void SubscribeToEvents(std::unique_ptr<EventBus>& eventBus) {
        eventBus->SubscribeToEvent<KeyPressedEvent>(this, &KeyboardMovementSystem::OnKeyPressed);
    }
    void OnKeyPressed(KeyPressedEvent& event) {

        Logger::Log((std::string("Keyboard button was pressed: ") + SDL_GetKeyName(event.symbol)).c_str());

    }

};
#endif //KEYBOARDMOVEMENTSYSTEM_H
