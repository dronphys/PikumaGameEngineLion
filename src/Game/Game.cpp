#include "Game.h"
#include <iostream>
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "spdlog/common.h"
#include <memory>


Game::Game() {
	isRunning = false;
	registry = std::make_unique<Registry>();
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

	windowWidth = displayBounds.w / 2;
	windowHeight = displayBounds.h / 2;

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




void Game::Setup() {
	// create entity
	Entity tank = registry->CreateEntity();
	// add components
	tank.AddComponent<TransformComponent>(
		 glm::vec2(10.0,30.0)
		,glm::vec2(0.0, 1.0)
		,0.0
		);

	tank.AddComponent<RigidBodyComponent>(glm::vec2(5.0,0.0));

	Entity player = registry->CreateEntity();
	player.AddComponent<TransformComponent>(
		 glm::vec2(10.0,30.0)
		,glm::vec2(0.0, 1.0)
		,0.0
		);
	player.AddComponent<RigidBodyComponent>(glm::vec2(5.0,0.0));
	player.RemoveComponent<TransformComponent>();
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

	// store current frame time;
	millisecsPreviousFrame = SDL_GetTicks();

}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	// TODO:: Render all game objects here
	
	SDL_Surface* surface = IMG_Load(MY_PROJECT_PATH"/assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	// destination rectangle to paste the texture.


	SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}