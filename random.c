#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

typedef struct object {
	float x;
	float y;
	float speed;
} object_t;

int WinMain(int argc, char **argv) {
	if((SDL_Init(SDL_INIT_VIDEO)) != 0){
		printf("Cannot initialize: %s\n", SDL_GetError());
		return 1;
	}	

	printf("Initialization successful\n");

	SDL_Window *win = SDL_CreateWindow("HELLO",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if(!win) {
		printf("Cannot create main window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);
	if(!rend) {
		printf("Cannot create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	time_t t;
	int c;
	srand((unsigned) time(&t));
	SDL_Rect rect;

	// sketch 1
	int x, y, w, h;
	c = 0;
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderClear(rend);
	while(c < 30) {
		// draw random rect
		w = (rand() % 40) + 10;
		h = (rand() % 40) + 10;
		x = rand() % (WINDOW_WIDTH - w);
		y = rand() % (WINDOW_HEIGHT - h);

		rect.x = x;
		rect.y = y;
		rect.w = w;
		rect.h = h;

		SDL_SetRenderDrawColor(rend, 30, 240, 128, 255);
		SDL_RenderDrawRect(rend, &rect);
		SDL_RenderPresent(rend);

		c++;
		SDL_Delay(500);
	}

	c = 0;

	// sketch2
	int step_x, step_y;
	int size_x, size_y;
	rect.x = (WINDOW_WIDTH - 50) / 2;
	rect.y = (WINDOW_HEIGHT - 50) / 2;
	rect.w = rect.h = 50;
	while(c < 30) {
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);
		
		step_x = (rand() % 3) -1;
		step_y = (rand() % 3) -1;

		step_x *= monte_carlo(0, 20);
		step_y *= monte_carlo(0, 20);

		rect.x += step_x;
		rect.y += step_y;

		SDL_SetRenderDrawColor(rend, 30, 240, 128, 255);
		SDL_RenderDrawRect(rend, &rect);
		SDL_RenderPresent(rend);
		
		c++;
		SDL_Delay(500);
	}

	printf("Bye!\n");

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

int monte_carlo(int min, int max) {
	int r1, r2;
	while(true) {
		r1 = (rand() % max) + min;
		r2 = (rand() % max) + min;
		if(r2 < r1) {
			return r1;
		}
	}
}







