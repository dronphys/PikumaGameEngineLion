#include "Game.h"
#include <iostream>
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "spdlog/common.h"
#include <memory>
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/RenderCollisionSystem.h"
#include "../Systems/CameraMovementSystem.h"
#include "../Systems/ProjectileEmitSystem.h"
#include "../Components/AllComponents.h"
#include "../Events/KeyPressedEvent.h"
#include <fstream>
#include <sstream>

#include "../Components/HealthComponent.h"
#include "../Events/KeyPressedEvent.h"
#include "../Systems/KeyboardMovementSystem.h"
#include "../Systems/LifeSpanSystem.h"

Game::Game() {
	isRunning = false;
	isDebug = false;
	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
	eventBus = std::make_unique<EventBus>();
	Logger::Log("Game constructor called!");
}

Game::~Game() {
	Logger::Log("Game destructor called!");
}


void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)	{
		std::cerr<< "Error initializing SDL." << std::endl;
		return;
	}

	SDL_Rect displayBounds;
	if (SDL_GetDisplayBounds(0, &displayBounds) != 0) {
		std::cerr << "SDL_GetDisplayBounds Error: " << SDL_GetError() << std::endl;
		return;
	}

	windowWidth = 1800;//displayBounds.w ;
	windowHeight = 960;//displayBounds.h ;

	camera.x = 0;
	camera.y = 0;
	camera.w = windowWidth;
	camera.h = windowHeight;


	std::cout << windowWidth << std::endl;
	window = SDL_CreateWindow(
		"My Game", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS
	);
	if (!window) {
		Logger::Err("Error creating SDL window.");
		return;
	}
	renderer = SDL_CreateRenderer(
		window, 
		-1, 
		SDL_RENDERER_ACCELERATED);// | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer) {
		Logger::Err("Error creating SDL renderer.");
		return;
	}

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
}
void Game::LoadLevelFromFile(const std::string& fileName) {
	// open file
	int textureColNumbers = 10;
	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumCols = 0;
	int mapNumRows = 0;

	std::ifstream file(fileName);
	std::string line;
	std::vector<std::vector<int>> levelMap;

	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string token;
		std::vector<int> raw;

		while (std::getline(ss, token, ',')) {
			int value = std::stoi(token); // convert string -> int
			raw.push_back(value);
		}
		levelMap.push_back(raw);
	}
	mapNumCols = levelMap[0].size();
	mapNumRows = levelMap.size();


	// creating entities
	for (auto y = 0; y < mapNumRows; y++) {
		for (auto x = 0; x < mapNumCols; x++) {
			// TODO change this later. Only walid for texture with 10 cols.
			int srcRectY = (levelMap[y][x] / textureColNumbers) * tileSize;
			int srcRectX = (levelMap[y][x] % textureColNumbers) * tileSize;

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(
				glm::vec2(x*tileScale*tileSize, y*tileScale*tileSize),
				glm::vec2(tileScale, tileScale),
				0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image",tileSize, tileSize,0, false, srcRectX, srcRectY);
		}
	}

}

void Game::LoadLevel(int level) {

	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<RenderCollisionSystem>();
	registry->AddSystem<DamageSystem>();
	registry->AddSystem<KeyboardMovementSystem>();
	registry->AddSystem<CameraMovementSystem>();
	registry->AddSystem<ProjectileEmitSystem>();
	registry->AddSystem<LifeSpanSystem>();

	assetStore->AddTexture(renderer,"tank-image", "../assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer,"truck-image", "../assets/images/truck-ford-right.png");
	assetStore->AddTexture(renderer, "tilemap-image", "../assets/tilemaps/jungle.png");
	assetStore->AddTexture(renderer,"chopper-image", "../assets/images/chopper-spritesheet.png");
	assetStore->AddTexture(renderer,"radar-image", "../assets/images/radar.png");
	assetStore->AddTexture(renderer,"bullet-image", "../assets/images/bullet.png");
	LoadLevelFromFile("../assets/tilemaps/jungle.map");

	Entity tank = registry->CreateEntity();
	// add components
	tank.AddComponent<TransformComponent>(
		 glm::vec2(100.0,00.0)
		,glm::vec2(2.0, 2.0)
		,0.0);
	tank.AddComponent<SpriteComponent>("tank-image",32,32,2);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(00.0,0.0));
	tank.AddComponent<BoxColliderComponent>(64,64);
	tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(-200.0,00.0), 2000, 1000);
	tank.AddComponent<HealthComponent>(100);

	Entity truck = registry->CreateEntity();
	// add components
	truck.AddComponent<TransformComponent>(
		 glm::vec2(250.0,00.0)
		,glm::vec2(2.0, 2.0)
		,0.0);
	truck.AddComponent<SpriteComponent>("truck-image",32,32,1);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0,0.0));
	truck.AddComponent<BoxColliderComponent>(64,64);
	truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(100.0,00.0), 1000, 2000);
	truck.AddComponent<HealthComponent>(100);

	// Choper is a player
	Entity chopper = registry->CreateEntity();
	// add components
	chopper.AddComponent<TransformComponent>(
		 glm::vec2(600.0,00.0)
		,glm::vec2(2.0, 2.0)
		,0.0);
	chopper.AddComponent<SpriteComponent>("chopper-image",32,32,1);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0,0.0));
	chopper.AddComponent<AnimationComponent>(2,10);
	chopper.AddComponent<KeyboardControlledComponent>(
		glm::vec2(0.0, -100.0),
		glm::vec2(100.0, 0.0),
		glm::vec2(0.0, 100.0),
		glm::vec2(-100.0, 0.0));
	chopper.AddComponent<CameraFollowComponent>();
	chopper.AddComponent<HealthComponent>(100);

	Entity radar = registry->CreateEntity();
	// add components
	radar.AddComponent<TransformComponent>(
		 glm::vec2(1400.0,0.0)
		,glm::vec2(1.0, 1.0)
		,0.0);
	radar.AddComponent<SpriteComponent>("radar-image",64,64,1, true);
	radar.AddComponent<AnimationComponent>(8,5);

}

void Game::Setup() {
	LoadLevel(1);
}

void Game::Run() {
	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}

void Game::ProcessInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
				if (sdlEvent.key.keysym.sym == SDLK_d) {
					isDebug = !isDebug;
				}
				break;
		}
	}
}

void Game::Update() {

	// we waste here time, untill time of the frame has passed
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0f;

	// store current frame time;
	millisecsPreviousFrame = SDL_GetTicks();

	// Reset all event handlers for current frame
	eventBus->Reset();

	// Subscription to all systems
	registry->GetSystem<DamageSystem>().SubscribeToEvents(eventBus);
	registry->GetSystem<KeyboardMovementSystem>().SubscribeToEvents(eventBus);


	//Ask registry to update a movement system
	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update(eventBus);
	registry->GetSystem<CameraMovementSystem>().Update(camera);
	registry->GetSystem<ProjectileEmitSystem>().Update(*registry);
	registry->GetSystem<LifeSpanSystem>().Update();

	registry->Update();
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	registry->GetSystem<RenderSystem>().Update(renderer,assetStore,camera);
	if (isDebug) {
		registry->GetSystem<RenderCollisionSystem>().Update(renderer,camera);
	}
	SDL_RenderPresent(renderer);
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}