#include "walk.h"
#include <SDL2/SDL.h>
#include <iostream>

int main() {
  srand(time(0));

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window* window = SDL_CreateWindow("Circle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600,  SDL_WINDOW_ALWAYS_ON_TOP);
  SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Event event;

  RandomWalk new_walk(200, 20, 2000);
  new_walk.render_walk(event, window, renderer);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
