#include "walk.h"
#include <cstdlib>
#include <cmath>
#include <vector>
#include <random>
#include <ctime>
#include <SDL2/SDL.h>

RandomWalk::RandomWalk(double r, double l, int steps) {
  radius = r;
  segment_length = l;
  number_of_steps = steps;
}

// return random (r, theta) pair
double RandomWalk::random_pair(){
  std::random_device rd;
  std::mt19937 gen(rd());
  const double TWO_PI = 2.0 * M_PI;
  std::uniform_real_distribution<> dis(0.0, TWO_PI);

  double random_theta = dis(gen);
  
  return random_theta;
}

std::vector<std::vector<int>> RandomWalk::basic_walk(SDL_Window* window) {
  int windowWidth, windowHeight;
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);
  int centerX = windowWidth/2;
  int centerY = windowHeight/2;
  // do the random walk
  // initialize:
  double prev_x = 0;
  double prev_y = 0;
  std::vector<std::vector<int>> points;
  for (int iter = 0; iter < number_of_steps; iter++) {
    double theta = random_pair();
    double x = prev_x + segment_length*cos(theta);
    double y = prev_y + segment_length*sin(theta);
    double r = sqrt(x*x + y*y);
    while (r >= radius) {
      theta = random_pair();
      x = prev_x + segment_length*cos(theta);
      y = prev_y + segment_length*sin(theta);
      r = sqrt(x*x + y*y);
    }

    // convert points to screen coordinates
    int i = static_cast<int>(x + centerX);
    int j = static_cast<int>(y + centerY);
    
    points.push_back({i, j});

    // each point is calculated in relation to the previous point --> walk
    prev_x = x;
    prev_y = y;
  }

  return points;
}

// A random walk where the next point is defined as a radius r and angle theta from the previous point. Non-avoiding except for boundaries.
void RandomWalk::render_basic_walk(SDL_Event event, SDL_Window* window, SDL_Renderer* renderer){
  int windowWidth, windowHeight;
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);
  int centerX = windowWidth/2;
  int centerY = windowHeight/2;

  std::vector<std::vector<int>> points = basic_walk(window);

  bool running = true;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) running = false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderDrawPoint(renderer, centerX, centerY);
    
    // draw circle boundary
    for (int i = 0; i < windowWidth; i++) {
      for (int j = 0; j < windowHeight; j++) {
	double x = static_cast<double>(i - centerX);
	double y = static_cast<double>(j - centerY);
	double r = sqrt(x*x + y*y);
	if (r > radius - radius/100 && r < radius + radius/100) {
	  SDL_RenderDrawPoint(renderer, i, j);
	}
      }
    }

    // draw random walk points
    for (int i = 0; i < number_of_steps; i++) {
      int color_val = i*255/number_of_steps;
      int val_color = number_of_steps*255/i;
      SDL_SetRenderDrawColor(renderer, color_val, color_val, color_val, 255);
      SDL_RenderDrawPoint(renderer, points[i][0], points[i][1]);
      if (i != 0) {
	SDL_RenderDrawLine(renderer, points[i-1][0], points[i-1][1], points[i][0], points[i][1]);
      }
    }
    SDL_RenderPresent(renderer);
  }
}

std::vector<std::vector<int>> RandomWalk::self_avoiding_walk(SDL_Window* window) {
  int windowWidth, windowHeight;
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);
  int centerX = windowWidth/2;
  int centerY = windowHeight/2;
  // do the random walk
  // initialize:
  double prev_x = 0;
  double prev_y = 0;
  std::vector<std::vector<int>> points;
  for (int iter = 0; iter < number_of_steps; iter++) {
    double theta = random_pair();
    double x = prev_x + segment_length*cos(theta);
    double y = prev_y + segment_length*sin(theta);
    double r = sqrt(x*x + y*y);
    while (r >= radius) {
      theta = random_pair();
      x = prev_x + segment_length*cos(theta);
      y = prev_y + segment_length*sin(theta);
      r = sqrt(x*x + y*y);
    }

    // convert points to screen coordinates
    int i = static_cast<int>(x + centerX);
    int j = static_cast<int>(y + centerY);
    
    points.push_back({i, j});

    // each point is calculated in relation to the previous point --> walk
    prev_x = x;
    prev_y = y;
  }

  return points;
}

// A random walk where the next point is defined as a radius r and angle theta from the previous point.
void RandomWalk::render_self_avoiding_walk(SDL_Event event, SDL_Window* window, SDL_Renderer* renderer){
  int windowWidth, windowHeight;
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);
  int centerX = windowWidth/2;
  int centerY = windowHeight/2;

  std::vector<std::vector<int>> points = self_avoiding_walk(window);

  bool running = true;

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) running = false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderDrawPoint(renderer, centerX, centerY);
    
    // draw circle boundary
    for (int i = 0; i < windowWidth; i++) {
      for (int j = 0; j < windowHeight; j++) {
	double x = static_cast<double>(i - centerX);
	double y = static_cast<double>(j - centerY);
	double r = sqrt(x*x + y*y);
	if (r > radius - radius/100 && r < radius + radius/100) {
	  SDL_RenderDrawPoint(renderer, i, j);
	}
      }
    }

    // draw random walk points
    for (int i = 0; i < number_of_steps; i++) {
      int color_val = i*255/number_of_steps;
      int val_color = number_of_steps*255/i;
      SDL_SetRenderDrawColor(renderer, color_val, color_val, color_val, 255);
      SDL_RenderDrawPoint(renderer, points[i][0], points[i][1]);
      if (i != 0) {
	SDL_RenderDrawLine(renderer, points[i-1][0], points[i-1][1], points[i][0], points[i][1]);
      }
    }
    SDL_RenderPresent(renderer);
  }
}
