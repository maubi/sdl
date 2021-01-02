#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

#define POPULATION 2000
#define DNA_SIZE 6 
#define CYCLE_WAIT 700
#define CYCLES 100000
#define ALPHA_OPAQUE 255
#define ALPHA_TRANSPARENT 0
#define CELL_W 5
#define CELL_H 5
#define PADDING 5
#define COLS 20
#define MARGIN_LEFT 0 
#define TARGET_SIZE DNA_SIZE
#define MUTATION_RATE 100
#define MAX_POOL_SIZE 100000

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
void mutate(creature_t *c);
int check_solution(creature_t p[]);
void log_solution(creature_t c);

rgb_t target[TARGET_SIZE];

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
	SDL_Log("Generating target...");
	for(int i = 0; i < TARGET_SIZE; i++) {
		int n = rand() % 256;
		target[i].r = n;
		target[i].g = n;
		target[i].b = n;
	}

	SDL_Log("Generating initial population: start (Population size is: %d)", POPULATION);
	creature_t population[POPULATION];
	for(int i = 0; i < POPULATION; i++) {
		for(int j = 0; j < DNA_SIZE; j++) {
			int n;
			n = rand() % 256;
			population[i].dna[j].r = n;
			population[i].dna[j].g = n;
			population[i].dna[j].b = n;
		}
	}
	SDL_Log("Generating initial population: end");

	// game loop
	c = 0;
	SDL_SetRenderDrawColor(rend, 240, 130, 175, ALPHA_OPAQUE);
	SDL_RenderClear(rend);
	SDL_Log("Main loop: start");
	while(c < CYCLES) {
		SDL_Log("Cycle: %d", c);

		while(SDL_PollEvent(&event)) {	// need to poll for events otherwise win becomes unresponsive
		}

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
			}
		}
		SDL_RenderPresent(rend);

		calc_fitness(population);

		int solved = check_solution(population);
		if(solved >= 0) {
			SDL_Log("SOLVED AT INDEX: %d", solved);
			log_solution(population[solved]);
			break;
		}


		int pool[MAX_POOL_SIZE];
		size_t s = calc_mating_pool(population, pool);
		//log_mating_pool(pool, s);

		creature_t children[POPULATION];
		create_children(population, pool, s, children);

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
		SDL_Log("idx: %d val: %d", i, pool[i]);
	}
}

void create_children(creature_t population[], int pool[], size_t s, creature_t children[]) {
	for(int i = 0; i < POPULATION; i++) {
		int p1 = rand() % s;
		int p2 = rand() % s;

		mate(population[pool[p1]], population[pool[p2]], &(children[i]));
		mutate(&children[i]);
		children[i].fitness = 1;
	}
}

void mate(creature_t p1, creature_t p2, creature_t *child) {
	for(int i = 0; i < DNA_SIZE; i++) {
		int p = rand() % 2;
		if(p == 0) {
			child->dna[i] = p1.dna[i];
		} else if(p == 1) {
			child->dna[i] = p2.dna[i];
		} else {
		}
	}
}

void swap_population(creature_t p1[], creature_t p2[]) {
	for(int i = 0; i < POPULATION; i++) {
		p1[i] = p2[i];
	}
}

void mutate(creature_t *c) {
	for(int i = 0; i < DNA_SIZE; i++) {
		int m = rand() % 1001;		// FIXME: remove magic number
		if(m < MUTATION_RATE) {
			int n = rand() % 256;
			c->dna[i].r = n;
			c->dna[i].g = n;
			c->dna[i].b = n;
		}
	}
}

int check_solution(creature_t p[]) {
	for(int i = 0; i < POPULATION; i++) {
		if(p[i].fitness == (1 + DNA_SIZE)) {
			return i;
		}
	}
	return -1;
}

void log_solution(creature_t c) {
	SDL_Log("Target is:");
	for(int i = 0; i < TARGET_SIZE; i++) {
		SDL_Log("{%d,%d,%d}", target[i].r, target[i].g, target[i].b);
	}
	SDL_Log("Solution is:");
	for(int i = 0; i < DNA_SIZE; i++) {
		SDL_Log("{%d,%d,%d}", c.dna[i].r, c.dna[i].g, c.dna[i].b);
	}
}






