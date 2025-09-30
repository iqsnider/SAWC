#pragma once

#include <vector>
#include <SDL2/SDL.h>

class RandomWalk {
public:
  double radius;
  double segment_length;
  int number_of_steps;
  RandomWalk(double r, double l, int steps);
  void render_basic_walk(SDL_Event event, SDL_Window* window, SDL_Renderer* renderer);
  void render_self_avoiding_walk(SDL_Event event, SDL_Window* window, SDL_Renderer* renderer);
  void render_test_walk(SDL_Event event, SDL_Window* window, SDL_Renderer* renderer);
private:
  double random_angle();
  double random_angle_directed(std::vector<std::vector<double>> vec);
  double random_angle_test(const std::vector<std::vector<double>>& vec);
  std::vector<std::vector<int>> basic_walk(SDL_Window* window);
  std::vector<std::vector<int>> self_avoiding_walk(SDL_Window* window);
  std::vector<std::vector<int>> test_walk(SDL_Window* window);
};

