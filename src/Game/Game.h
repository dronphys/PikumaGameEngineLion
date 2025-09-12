#ifndef GAME_H
#define GAME_H
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <glm/glm.hpp>
#include "../ECS/ECS.h"
#include "../ECS/Registry.h"
#include <memory>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

#define MY_PROJECT_PATH "E:/dev/PikumaGameEngine/PikumaGameEngine"

const int FPS = 120;
const int MILLISECS_PER_FRAME = 1000 / FPS;



class Game {
private:
	bool isRunning;
	int millisecsPreviousFrame = 0;
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::unique_ptr<Registry> registry;

public:
	Game();
	~Game();
	void Initialize();
	void Run();
	void Setup();
	void ProcessInput();
	void Update();
	void Render();
	void Destroy();

	int windowWidth;
	int windowHeight;
};

#endif