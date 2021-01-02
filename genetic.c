#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

#define POPULATION 360
#define DNA_SIZE 10
#define CYCLE_WAIT 1500
#define CYCLES 10
#define ALPHA_OPAQUE 255
#define ALPHA_TRANSPARENT 0
#define CELL_W 10
#define CELL_H 10
#define PADDING 10
#define COLS 6
#define MARGIN_LEFT 75 

typedef struct object {
	float x;
	float y;
	float speed;
} object_t;

typedef struct rgb {\
	int r;
	int g;
	int b;
} rgb_t;

typedef rgb_t dna_t[DNA_SIZE];

int WinMain(int argc, char **argv) {
	if((SDL_Init(SDL_INIT_VIDEO)) != 0){
		printf("Error - initialization failed: %s\n", SDL_GetError());
		return 1;
	}	

	SDL_Log("Initialization: done");

	SDL_Window *win = SDL_CreateWindow("HELLO",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if(!win) {
		SDL_Log("Error - cannot create main window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer *rend = SDL_CreateRenderer(win, -1, render_flags);
	if(!rend) {
		SDL_Log("Error - cannot create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	time_t t;
	int c;
	srand((unsigned) time(&t));
	SDL_Rect rect;
	SDL_Event event;

	// init
	SDL_Log("Generating initial population: start (Population size is: %d)", POPULATION);
	dna_t population[POPULATION];
	for(int i = 0; i < POPULATION; i++) {
		for(int j = 0; j < DNA_SIZE; j++) {
			int r, g, b;
			r = rand() % 256;
			g = rand() % 256;
			b = rand() % 256;
			population[i][j].r = r;
			population[i][j].g = g;
			population[i][j].b = b;
		}
	}
	SDL_Log("Generating initial population: end");

	// game loop
	c = 0;
	SDL_SetRenderDrawColor(rend, 0, 0, 0, ALPHA_OPAQUE);
	SDL_RenderClear(rend);
	SDL_Log("Main loop: start");
	while(c < CYCLES) {
		SDL_Log("Cycle: %d", c);
		while(SDL_PollEvent(&event)) {	// need to poll for events otherwise win becomes unresponsive
		}


		SDL_Log("Drawing creatures: start");
		for(int i = 0; i < POPULATION; i++) {
			int r = i / COLS;											// row
			int c = i - r * COLS;										// column
			int x = MARGIN_LEFT + c * (CELL_W * DNA_SIZE + PADDING);	// screen x
			int y = r * CELL_H;											// screen y
			SDL_Log("x: %d", x);
			for(int j = 0; j < DNA_SIZE; j++) {
				SDL_Rect rect;
				rect.w = CELL_W;
				rect.h = CELL_H;
				rect.x = x;
				rect.y = y;
				rgb_t gene = population[i][j];
				SDL_SetRenderDrawColor(rend, gene.r, gene.g, gene.b, ALPHA_OPAQUE);
				SDL_RenderFillRect(rend, &rect);
				x += CELL_W;
			}
		}
		SDL_Log("Drawing creatures: done");

		// TODO: solution found: quit loop

		// TODO: solution not found: REPRODUCTION

			// TODO: calculate fitness of each creature

			// TODO: create mating pool

			// TODO: crossover

			// TODO: mutation

		SDL_RenderPresent(rend);

		c++;
		SDL_Log("Sleeping %d milliseconds: start", CYCLE_WAIT);
		SDL_Delay(CYCLE_WAIT);
		SDL_Log("Sleeping %d milliseconds: done", CYCLE_WAIT);
	}

	SDL_Log("Main loop: end\n");

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	SDL_Log("Bye!");
	return 0;
}
