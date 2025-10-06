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
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"
#define MY_PROJECT_PATH "E:/dev/PikumaGameEngine/PikumaGameEngine"

const int FPS = 120;
const int MILLISECS_PER_FRAME = 1000 / FPS;



class Game {
private:
	bool isRunning;
	bool isDebug;
	int millisecsPreviousFrame = 0;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect camera;

	std::unique_ptr<Registry> registry;
	std::unique_ptr<AssetStore> assetStore;
	std::unique_ptr<EventBus> eventBus;


	/// Temporary

	Entity player{-1};
	Entity text{-2};
	bool hasColider = true;

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
	void LoadLevel(int level);
	void LoadLevelFromFile(const std::string& fileName);

	int windowWidth;
	int windowHeight;
};

#endif