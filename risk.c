#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

#define RISKS 100
#define CYCLES 1000
#define CYCLE_WAIT 100
#define INITIAL_MONEY 500

typedef struct player {
	int money;
	int (*strategy) (int min, int max);
} player_t;

int minimize(int min, int max);
int maximize(int min, int max);
int proportionize(int min, int max);
int minimize_ex(int min, int max);

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
	int risks[RISKS];
	for(int i = 0; i < RISKS; i++) {
		risks[i] = i;
	}

	player_t minimizer, maximizer, proportional, minimizer_ex;

	minimizer.money = INITIAL_MONEY;
	maximizer.money = INITIAL_MONEY;
	proportional.money = INITIAL_MONEY;
	minimizer_ex.money = INITIAL_MONEY;
	minimizer.strategy = minimize;
	maximizer.strategy = maximize;
	proportional.strategy = proportionize;
	minimizer_ex.strategy = minimize_ex;
	
	// game loop
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderClear(rend);
	c = 0;
	SDL_Log("Main loop: start");
	int choice, n;
	while(c < CYCLES) {
		while(SDL_PollEvent(&event)) {	// need to poll for events otherwise win becomes unresponsive
		}

		choice = (*(minimizer.strategy))(0, RISKS);	
		n = rand() % RISKS;
		if(n > choice) {
			minimizer.money += risks[choice];
		} else {
			minimizer.money -= risks[choice];
		}

		choice = (*(maximizer.strategy))(0, RISKS);	
		n = rand() % RISKS;
		if(n > choice) {
			maximizer.money += risks[choice];
		} else {
			maximizer.money -= risks[choice];
		}

		choice = (*(proportional.strategy))(0, RISKS);	
		n = rand() % RISKS;
		if(n > choice) {
			proportional.money += risks[choice];
		} else {
			proportional.money -= risks[choice];
		}

		choice = (*(minimizer_ex.strategy))(0, RISKS);	
		n = rand() % RISKS;
		if(n > choice) {
			minimizer_ex.money += risks[choice];
		} else {
			minimizer_ex.money -= risks[choice];
		}

		SDL_Log("[%d] Minimizer: %d Maximizer: %d Proportional: %d Minimizer_ex: %d", c, minimizer.money, maximizer.money, proportional.money, minimizer_ex.money);

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

int minimize(int min, int max) {
	return rand() % ((max - min) * 50 / 100) + min;
}

int maximize(int min, int max) {
	return max - (rand() % ((max - min) * 50 / 100) + min);
}

int proportionize(int min, int max) {
	while(true) {
		int r1 = rand() % (max - min) + min;
		int r2 = rand() % (max - min) + min;
		if(r2 > r1 ) {
			return r1;
		}
	}
}

int minimize_ex(int min, int max) {
	return rand() % ((max - min) * 10 / 100) + min;
}
