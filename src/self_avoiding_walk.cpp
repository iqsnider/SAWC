#include "walk.h"
#include <cstdlib>
#include <cmath>
#include <vector>
#include <random>
#include <ctime>
#include <SDL2/SDL.h>

// basically a smoothing operator
double RandomWalk::random_angle_directed(const std::vector<std::vector<double>>& vec) {
  if (vec.size() < 2) {
      return random_angle();
  }

  double dx = vec.back()[0] - vec[vec.size()-2][0];
  double dy = vec.back()[1] - vec[vec.size()-2][1];
  double prev_angle = atan2(dy, dx);

  static std::mt19937 gen(std::random_device{}());
  std::uniform_real_distribution<> half_dis(-M_PI/2.0, M_PI/2.0);

  // Random offset in [-π/2, +π/2]
  double offset = half_dis(gen);

  // Allowed angle = forward half-plane around prev_angle
  double theta = prev_angle + offset;

  // Normalize into [0, 2π)
  auto norm = [](double a) {
      const double TWO_PI = 2.0*M_PI;
      a = fmod(a, TWO_PI);
      if (a < 0) a += TWO_PI;
      return a;
  };
  return norm(theta);
}

// Self-avoiding logic. When a new node is calculated, intersection is checked for all previous segments.
std::vector<std::vector<int>> RandomWalk::self_avoiding_walk(SDL_Window* window) {
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
    double theta = random_angle_directed(coordinates);
    double x = prev_x + segment_length*cos(theta);
    double y = prev_y + segment_length*sin(theta);
    double r = sqrt(x*x + y*y);
    bool self_avoided = false;
    bool stuck = false; // flag for convergence, segment is too tightly wound
    int stuck_limit = 1000000; // limit for number of trials before concluding that segment is stuck
    // constrain added points to the circle boundary
    while (r >= radius && iter >= 0 && iter <= 2) {
      theta = random_angle_directed(coordinates);
      x = prev_x + segment_length*cos(theta);
      y = prev_y + segment_length*sin(theta);
      r = sqrt(x*x + y*y);
    }
    // constrain added points by the self-avoiding intersection condition
    if (iter > 2) {
      int fails = 0;
      while (self_avoided == false) {
	// constrain added points to the circle boundary
	while (r >= radius) {
	  theta = random_angle_directed(coordinates);
	  x = prev_x + segment_length*cos(theta);
	  y = prev_y + segment_length*sin(theta);
	  r = sqrt(x*x + y*y);
	}

	// handle vertical segments
	bool cur_vertical = (x == prev_x);
	// slope and intercept
	double cur_A = cur_vertical ? 0.0 : (y - prev_y)/(x - prev_x);
	double cur_B = prev_y - cur_A*prev_x;
	for (int k = 0; k < iter - 1; k++) {
	  double test_x1 = coordinates[k][0];
	  double test_y1 = coordinates[k][1];
	  double test_x2 = coordinates[k+1][0];
	  double test_y2 = coordinates[k+1][1];

	  // handle vertical segments safely
	  bool test_vertical = (test_x1 == test_x2);

	  // slope and intercept
	  double test_A = test_vertical ? 0.0 : (test_y2 - test_y1)/(test_x2 - test_x1);
	  double test_B = test_y1 - test_A*test_x1;
	
	  // if slopes are equal, skip (parallel lines)
	  if (!cur_vertical && !test_vertical && fabs(test_A - cur_A) < 1e-9) {
	    self_avoided = true;
	    continue;
	  }

	  // calculate intersection point
	  double intersection_x, intersection_y;
	  if (cur_vertical) {
	      intersection_x = x;
	      intersection_y = test_A * x + test_B;
	  } else if (test_vertical) {
	      intersection_x = test_x1;
	      intersection_y = cur_A * intersection_x + cur_B;
	  } else {
	      intersection_x = (cur_B - test_B)/(test_A - cur_A);
	      intersection_y = cur_A * intersection_x + cur_B;
	  }

	  // check if intersection lies within both segments
	  if (intersection_x >= std::min(test_x1,test_x2) &&
	      intersection_x <= std::max(test_x1,test_x2) &&
	      intersection_y >= std::min(test_y1,test_y2) &&
	      intersection_y <= std::max(test_y1,test_y2)) {
	      self_avoided = false;
	      break;
	  } else {
	    self_avoided = true;
	  }
	}
	if (self_avoided == false) {
	  theta = random_angle_directed(coordinates);
	  x = prev_x + segment_length*cos(theta);
	  y = prev_y + segment_length*sin(theta);
	  r = sqrt(x*x + y*y);
	  fails++;
	  if (fails == stuck_limit) {
	    stuck = true;
	  }
	}
	if (stuck == true) {
	  break;
	}
       }
     }

    if (stuck == true) {
      break;
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
    for (auto i = 0u; i < points.size(); i++) {
      int color_val = i*255/points.size();
      int val_color = points.size()*255/i;
      SDL_SetRenderDrawColor(renderer, color_val, color_val, 255, 255);
      SDL_RenderDrawPoint(renderer, points[i][0], points[i][1]);
      if (i != 0) {
	SDL_RenderDrawLine(renderer, points[i-1][0], points[i-1][1], points[i][0], points[i][1]);
      }
    }
    SDL_RenderPresent(renderer);
  }
}
