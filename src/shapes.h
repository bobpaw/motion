#include <cmath>

#include <SDL.h>

#include "constants.h"

#ifndef MOTION_SHAPES_H
#define MOTION_SHAPES_H
int DrawCircle(SDL_Renderer* renderer, int x, int y, double r) {
	for (int i = 0; i < 360; i += 10) {
		SDL_RenderDrawPoint(renderer, int(x + r * std::cos(i * M_PI / 180.0)), int(y + r * std::sin(i * M_PI / 180.0)));
	}
	return 1;
}
#endif // MOTION_SHAPES_H