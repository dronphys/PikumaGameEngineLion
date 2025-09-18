#include "Game.h"
#include <iostream>
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "spdlog/common.h"
#include <memory>
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Components/AllComponents.h"
#include <fstream>
#include <sstream>

Game::Game() {
	isRunning = false;
	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
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

	windowWidth = displayBounds.w ;
	windowHeight = displayBounds.h ;

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
void Game::LoadLevelFromFile(std::string fileName) {
	// open file
	std::ifstream file(fileName);

	std::string line;
	std::vector<std::vector<int>> level;
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string token;
		std::vector<int> numbers;

		while (std::getline(ss, token, ',')) {
			int value = std::stoi(token); // convert string -> int
			numbers.push_back(value);
		}
		level.push_back(numbers);
	}
	int y_text = 0, x_text = 0;
	int y_map = 0, x_map = 0;
	for (auto vec: level) {
		for (auto el: vec) {
			y_text = el/10;
			x_text = el%10;

			std::cout << y_text << ", " << x_text << ": " << el << std::endl;
			Entity terrain = registry->CreateEntity();
			terrain.AddComponent<TransformComponent>(
				glm::vec2(static_cast<double>(x_map*32), static_cast<double>(y_map*32)),
				glm::vec2(1.0, 1.0),
				0.0);
			terrain.AddComponent<SpriteComponent>("tilemap-image",32,32, x_text*32, y_text*32);
			x_map++;
		}
		std::cout << std::endl;
		y_map++;
		x_map = 0;

	}



}

void Game::LoadLevel(int level) {
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();

	assetStore->AddTexture(renderer,"tank-image", "../assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer,"truck-image", "../assets/images/truck-ford-right.png");
	assetStore->AddTexture(renderer, "tilemap-image", "../assets/tilemaps/jungle.png");

	int tileSize = 32;
	double tileScale = 2.0;
	int mapNumCols = 25;
	int mapNumRows = 20;
	std::fstream mapFile;
	mapFile.open("../assets/tilemaps/jungle.map");

	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(
				glm::vec2(x*tileScale*tileSize, y*tileScale*tileSize),
				glm::vec2(tileScale, tileScale),
				0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image",tileSize, tileSize, srcRectX, srcRectY);
		}
	}
	mapFile.close();


	// TODO
	// Load  the timemap ./assets/tilemaps/jungle.png
	// we need to load the file ./assets/tilemaps/jungle.map
	// use idea of source rectangle.
	// Consider creating one entity per tile

	// create entity


	Entity tank = registry->CreateEntity();
	// add components
	tank.AddComponent<TransformComponent>(
		 glm::vec2(300.0,00.0)
		,glm::vec2(3.0, 3.0)
		,45.0
		);
	tank.AddComponent<SpriteComponent>("tank-image",32,32);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(-40.0,5.0));


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
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
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

	//Ask registry to update a movement system
	registry->GetSystem<MovementSystem>().Update(deltaTime);
	// store current frame time;
	millisecsPreviousFrame = SDL_GetTicks();

	registry->Update();
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	registry->GetSystem<RenderSystem>().Update(renderer,assetStore);

	SDL_RenderPresent(renderer);
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}