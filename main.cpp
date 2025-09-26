#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>

struct Vec {
	double x;
	double y;
};

int main() {
	srand(time(0));

	Vec loc;
	loc.x = rand();
	loc.y = rand();

	std::cout << loc.x << std::endl;
	std::cout << loc.y << std::endl;

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	bool running = true;
	SDL_Event event;

	while (running) {
        	while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) running = false;
        	}
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        for (int w = 0; w < 100; w++) {
		for (int h = 0; h < 100; h++) {
			int dx = 50 - w;
			int dy = 50 - h;
			if ((dx*dx + dy*dy) <= (50*50)) {
				SDL_RenderDrawPoint(renderer, 350 + dx, 250 + dy);
			}
		}
        }
        
        SDL_RenderPresent(renderer);
    	}
    
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
