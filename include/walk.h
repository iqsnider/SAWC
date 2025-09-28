#pragma once

#include <vector>
#include <SDL2/SDL.h>

class RandomWalk {
public:
  double radius;
  double segment_length;
  int number_of_steps;
  RandomWalk(double r, double l, int steps);
  void render_walk(SDL_Event event, SDL_Window* window, SDL_Renderer* renderer);
private:
  double random_pair();
  std::vector<std::vector<int>> basic_walk(SDL_Window* window);
};

