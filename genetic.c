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
#define CYCLE_WAIT 100
#define CYCLES 10000
#define ALPHA_OPAQUE 255
#define ALPHA_TRANSPARENT 0
#define CELL_W 10
#define CELL_H 10
#define PADDING 10
#define COLS 6
#define MARGIN_LEFT 75 
#define TARGET_SIZE DNA_SIZE

typedef struct rgb {
	int r;
	int g;
	int b;	
} rgb_t;

typedef struct creature {
	rgb_t dna[DNA_SIZE];
	float fitness;
} creature_t;

void calc_fitness(creature_t population[]);
size_t calc_mating_pool(creature_t population[], int pool[]);
void log_mating_pool(int pool[], size_t size);
void create_children(creature_t population[], int pool[], size_t s, creature_t children[]);
void mate(creature_t p1, creature_t p2, creature_t *child);
void swap_population(creature_t p1[], creature_t p2[]);

rgb_t target[TARGET_SIZE] = {
	{255, 232, 210}
	,{124, 40, 29}
	,{0, 60, 90}
	,{230, 230, 228}
	,{35, 7, 50}
	,{160, 130, 20}
	,{10, 20, 30}
	,{80, 243, 100}
	,{112, 242, 20}
	,{1, 2, 243}
};

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
	creature_t population[POPULATION];
	for(int i = 0; i < POPULATION; i++) {
		for(int j = 0; j < DNA_SIZE; j++) {
			int r, g, b;
			r = rand() % 256;
			g = rand() % 256;
			b = rand() % 256;
			population[i].dna[j].r = r;
			population[i].dna[j].g = g;
			population[i].dna[j].b = b;
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

		//SDL_Log("Drawing creatures: start");
		for(int i = 0; i < POPULATION; i++) {
			int r = i / COLS;											// row
			int c = i - r * COLS;										// column
			int x = MARGIN_LEFT + c * (CELL_W * DNA_SIZE + PADDING);	// screen x
			int y = r * CELL_H;											// screen y
			for(int j = 0; j < DNA_SIZE; j++) {
				SDL_Rect rect;
				rect.w = CELL_W;
				rect.h = CELL_H;
				rect.x = x;
				rect.y = y;
				rgb_t gene = population[i].dna[j];
				SDL_SetRenderDrawColor(rend, gene.r, gene.g, gene.b, ALPHA_OPAQUE);
				SDL_RenderFillRect(rend, &rect);
				x += CELL_W;
				//SDL_Log("i: %d r: %d g: %d b: %d", i, gene.r, gene.g, gene.b);
			}
		}
		SDL_RenderPresent(rend);
		//SDL_Log("Drawing creatures: done");

		// TODO: solution found: quit loop

			calc_fitness(population);

			int pool[10000];
			size_t s = calc_mating_pool(population, pool);
			log_mating_pool(pool, s);

			creature_t children[POPULATION];
			create_children(population, pool, s, children);

			// TODO: mutation

			swap_population(population, children);

		c++;
		SDL_Delay(CYCLE_WAIT);
	}

	SDL_Log("Main loop: end\n");

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	SDL_Log("Bye!");
	return 0;
}

void calc_fitness(creature_t population[]) {
	for(int i = 0; i < POPULATION; i++) {

		int fitness = 1;
		for(int j = 0; j < DNA_SIZE; j++) {
			if(	population[i].dna[j].r == target[j].r &&
				population[i].dna[j].g == target[j].g &&
				population[i].dna[j].b == target[j].b) {
				fitness++;
			}
		}
		population[i].fitness = fitness;
		//SDL_Log("i:%d f: %d", i, fitness);
	}
}

size_t calc_mating_pool(creature_t population[], int pool[]) {
	int k = 0;
	for(int i = 0; i < POPULATION; i++) {
		for(int j = 0; j < population[i].fitness; j++) {
			pool[k] = i;
			k++;
		}
	}
	return k;
}

void log_mating_pool(int pool[], size_t size) {
	for(int i = 0; i < size; i++) {
		//SDL_Log("idx: %d val: %d", i, pool[i]);
	}
}

void create_children(creature_t population[], int pool[], size_t s, creature_t children[]) {
	for(int i = 0; i < POPULATION; i++) {
		int p1 = rand() % s;
		int p2 = rand() % s;

		mate(population[pool[p1]], population[pool[p2]], &(children[i]));
		children[i].fitness = 1;
	}
}

void mate(creature_t p1, creature_t p2, creature_t *child) {
	//rgb_t dna_1[] = p1.dna;
	//rgb_t dna_2[] = p2.dna;

	for(int i = 0; i < DNA_SIZE; i++) {
		int p = rand() % 2;
		if(p == 0) {
			child->dna[i] = p1.dna[i];
		} else if(p == 1) {
			child->dna[i] = p2.dna[i];
		} else {
			SDL_Log(">>>> ERROR!");
		}
	}
}

void swap_population(creature_t p1[], creature_t p2[]) {
	for(int i = 0; i < POPULATION; i++) {
		p1[i] = p2[i];
	}
}



