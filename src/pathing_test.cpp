#include "walk.h"
#include <cstdlib>
#include <cmath>
#include <vector>
#include <random>
#include <ctime>
#include <SDL2/SDL.h>

// angle testing
// double RandomWalk::random_angle_test(std::vector<std::vector<double>> vec) {
//   std::random_device rd;
//   std::mt19937 gen(rd());
//
//   double dx = vec[vec.size()-1][0] - vec[vec.size()-2][0];
//   double dy = vec[vec.size()-1][1] - vec[vec.size()-2][1];
//   double theta_operator = atan2(dy/dx);
//
//   const double TWO_PI = 2.0 * M_PI;
//   std::uniform_real_distribution<> dis(theta_operator, TWO_PI - theta_operator);
//   double random_theta = dis(gen);
//
//   return random_theta;
// }

double RandomWalk::random_angle_test(const std::vector<std::vector<double>>& vec) {
  if (vec.size() < 2) {
    return random_angle();
  }
  std::random_device rd;
  std::mt19937 gen(rd());
  const double TWO_PI = 2.0 * M_PI;

  double dx = vec[vec.size()-1][0] - vec[vec.size()-2][0];
  double dy = vec[vec.size()-1][1] - vec[vec.size()-2][1];
  double prev_angle = atan2(dy, dx);


  // Allowed range is prev_angle + 90 degrees --> prev_angle + 270 degrees
  double min_angle = prev_angle + M_PI/2.0;
  double max_angle = prev_angle + 3.0*M_PI/2.0;

  // Normalize into [0, 2pi)
  auto norm = [TWO_PI](double a) {
      while (a < 0) a += TWO_PI;
      while (a >= TWO_PI) a -= TWO_PI;
      return a;
  };

  min_angle = norm(min_angle);
  max_angle = norm(max_angle);

  // If min < max, one clean interval
  // If wrap-around, need to choose from [0,max] or [min,2pi]
  std::uniform_real_distribution<> dis(0.0, 1.0);
  double r = dis(gen);

  double theta;
  if (min_angle < max_angle) {
      std::uniform_real_distribution<> dist(min_angle, max_angle);
      theta = dist(gen);
  } else {
      // Wrapped case: pick in [0,max] or [min,2pi]
      if (r < 0.5) {
	  std::uniform_real_distribution<> dist(0.0, max_angle);
	  theta = dist(gen);
      } else {
	  std::uniform_real_distribution<> dist(min_angle, TWO_PI);
	  theta = dist(gen);
      }
  }

  return theta;
}
// random walk pathing constraint testing
std::vector<std::vector<int>> RandomWalk::test_walk(SDL_Window* window) {
  int windowWidth, windowHeight;
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);
  int centerX = windowWidth/2;
  int centerY = windowHeight/2;
  // do the random walk
  // initialize:
  double prev_x = 0;
  double prev_y = 0;
  std::vector<std::vector<double>> coordinates; // double vector for storing raw math values
  std::vector<std::vector<int>> points;
  coordinates.push_back({prev_x, prev_y});
  points.push_back({static_cast<int>(centerX), static_cast<int>(centerY)});
  for (int iter = 0; iter < number_of_steps; iter++) {
    double theta = random_angle_test(coordinates);
    double x = prev_x + segment_length*cos(theta);
    double y = prev_y + segment_length*sin(theta);
    double r = sqrt(x*x + y*y);
    // constrain added points to the circle boundary
    while (r >= radius) {
      theta = random_angle_test(coordinates);
      x = prev_x + segment_length*cos(theta);
      y = prev_y + segment_length*sin(theta);
      r = sqrt(x*x + y*y);
    }
    // convert points to screen coordinates
    int i = static_cast<int>(x + centerX);
    int j = static_cast<int>(y + centerY);
    
    points.push_back({i, j});
    coordinates.push_back({x, y});

    // each point is calculated in relation to the previous point --> walk
    prev_x = x;
    prev_y = y;
  }

  return points;
}

// A random walk where the next point is defined as a radius r and angle theta from the previous point.
void RandomWalk::render_test_walk(SDL_Event event, SDL_Window* window, SDL_Renderer* renderer){
  int windowWidth, windowHeight;
  SDL_GetWindowSize(window, &windowWidth, &windowHeight);
  int centerX = windowWidth/2;
  int centerY = windowHeight/2;

  std::vector<std::vector<int>> points = test_walk(window);

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
    for (int i = 0; i < points.size(); i++) {
      // int color_val = i*255/points.size();
      // int val_color = points.size()*255/i;
      // SDL_SetRenderDrawColor(renderer, color_val, color_val, color_val, 255);
      SDL_RenderDrawPoint(renderer, points[i][0], points[i][1]);
      if (i != 0) {
	SDL_RenderDrawLine(renderer, points[i-1][0], points[i-1][1], points[i][0], points[i][1]);
      }
    }
    SDL_RenderPresent(renderer);
  }
}
