#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)
#define SKETCH_1_CYCLES 50
#define SKETCH_2_CYCLES 10000

typedef struct object {
	float x;
	float y;
	float speed;
} object_t;

int monte_carlo(int min, int max);

typedef struct line {
	int x1;
	int y1;
	int x2;
	int y2;
} line_t;

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
	SDL_Event event;

	// sketch 1
	int x, y, w, h;
	c = 0;
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderClear(rend);
	while(c < SKETCH_1_CYCLES) {
		while(SDL_PollEvent(&event)) {	// need to poll for events otherwise win becomes unresponsive
		}

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
	line_t lines[SKETCH_2_CYCLES];
	while(c < SKETCH_2_CYCLES) {
		while(SDL_PollEvent(&event)) {		// need to poll the event queue otherwise win becomes unresponsive
		}

		SDL_Log("Cycle: %d", c);

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);
		
		step_x = (rand() % 3) -1;	// choose a random direction
		step_y = (rand() % 3) -1;	// choose a random direction

		step_x *= monte_carlo(0, 70);	// choose a random length
		step_y *= monte_carlo(0, 70);	// choose a random length

		int new_x = rect.x + step_x;
		int new_y = rect.y + step_y;
		new_x = (new_x > WINDOW_WIDTH - 50) ? WINDOW_WIDTH - 50 : new_x; 
		new_x = (new_x < 0) ? 0 : new_x; 
		new_y = (new_y > WINDOW_HEIGHT - 50) ? WINDOW_HEIGHT - 50 : new_y; 
		new_y = (new_y < 0) ? 0 : new_y; 

		lines[c].x1 = rect.x;
		lines[c].y1 = rect.y;
		lines[c].x2 = new_x;
		lines[c].y2 = new_y;

		SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
		for(int i = 0; i <= c; i++) {
			SDL_RenderDrawLine(rend, lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
		}

		rect.x = new_x;
		rect.y = new_y;

		SDL_SetRenderDrawColor(rend, 30, 240, 128, 255);
		SDL_RenderDrawRect(rend, &rect);
		SDL_RenderPresent(rend);
		
		c++;
		SDL_Delay(100);
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
		if(r2 > r1) {
			return r1;
		}
	}
}







