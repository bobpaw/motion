#include <cmath>

#include <SDL.h>

#include "constants.h"

#ifndef MOTION_SHAPES_H
#define MOTION_SHAPES_H
static int DrawCircle_pts(SDL_Renderer* renderer, int cx, int cy, int x, int y) {
	SDL_RenderDrawPoint(renderer, cx + x, cy + y);
	SDL_RenderDrawPoint(renderer, cx + x, cy - y);
	SDL_RenderDrawPoint(renderer, cx - x, cy + y);
	SDL_RenderDrawPoint(renderer, cx - x, cy - y);
	SDL_RenderDrawPoint(renderer, cx + y, cy + x);
	SDL_RenderDrawPoint(renderer, cx + y, cy - x);
	SDL_RenderDrawPoint(renderer, cx - y, cy + x);
	SDL_RenderDrawPoint(renderer, cx - y, cy - x);
	return 0;
}

int DrawCircle(SDL_Renderer* renderer, int cx, int cy, int r) {
  int x = r, y = 0, dx = 1 - 2 * r, dy  = 1, RE = 0;
  while (x >= y) {
		DrawCircle_pts(renderer, cx, cy, x, y);
    ++y;
		RE += dy;
		dy += 2;
		if (2 * RE + dx > 0) {
			--x;
			RE += dx;
			dx += 2;
		}
  }
  return 0;
}
#endif // MOTION_SHAPES_H