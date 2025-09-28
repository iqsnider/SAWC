#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SDL2/SDL.h>

struct Vec {
	double x;
	double y;
};

class RandomWalk {
public:
  double radius;
  double segment_length;
  int number_of_steps;
  RandomWalk(double r, double l, double steps);
  void walk(SDL_Event event, SDL_Window* window, SDL_Renderer* renderer);
private:
  double random_pair();
};

RandomWalk::RandomWalk(double r, double l, double steps) {
  radius = r;
  segment_length = l;
  number_of_steps = steps;
}

// return random (r, theta) pair
double RandomWalk::random_pair(){
  return 0;
}

// A random walk where the next point is defined as a radius r and angle theta from the previous point. Non-avoiding except for boundaries.
void RandomWalk::walk(SDL_Event event, SDL_Window* window, SDL_Renderer* renderer){
  int windowWidth, windowHeight;
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);
  int centerX = windowWidth/2;
  int centerY = windowHeight/2;

  bool running = true;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) running = false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderDrawPoint(renderer, centerX, centerY);
    
    for (int i = 0; i < windowWidth; i++) {
      for (int j = 0; j < windowHeight; j++) {
	double x = static_cast<double>(i - centerX);
	double y = static_cast<double>(j - centerY);
	double r = sqrt(x*x + y*y);
	if (r > radius - 1 && r < radius + 1) {
	  SDL_RenderDrawPoint(renderer, i, j);
	}
      }
    }

    SDL_RenderPresent(renderer);
  }
}

int main() {
  srand(time(0));

  Vec loc;
  loc.x = rand();
  loc.y = rand();

  std::cout << loc.x << std::endl;
  std::cout << loc.y << std::endl;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("Circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,  SDL_WINDOW_ALWAYS_ON_TOP);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Event event;


  RandomWalk new_walk(200, 1, 10);
  new_walk.walk(event, window, renderer);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
