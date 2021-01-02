#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#define WINDOW_WIDTH (800)
#define WINDOW_HEIGHT (600)

#define CYCLES 1000
#define CYCLE_WAIT 50

int WinMain(int argc, char **argv) {
	if((SDL_Init(SDL_INIT_VIDEO)) != 0){
		printf("Error - initialization failed: %s\n", SDL_GetError());
		return 1;
	}	

	TTF_Init();

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
	int w, h, wb, hb, wi, hi;
	bool done = false;

	// init
	TTF_Font *consola = TTF_OpenFont("fonts/consola.ttf", 30);
	TTF_Font *consolab = TTF_OpenFont("fonts/consolab.ttf", 30);
	TTF_Font *consolai = TTF_OpenFont("fonts/consolai.ttf", 30);
	SDL_Color text_fg = {255, 255, 128};
	SDL_Surface *hello_surf = TTF_RenderText_Solid(consola, "Close the world. Open the next.", text_fg);
	SDL_Surface *hellob_surf = TTF_RenderText_Solid(consolab, "Close the world. Open the next.", text_fg);
	SDL_Surface *helloi_surf = TTF_RenderText_Solid(consolai, "Close the world. Open the next.", text_fg);
	SDL_Texture *hello_tex = SDL_CreateTextureFromSurface(rend, hello_surf);
	SDL_Texture *hellob_tex = SDL_CreateTextureFromSurface(rend, hellob_surf);
	SDL_Texture *helloi_tex = SDL_CreateTextureFromSurface(rend, helloi_surf);
	SDL_QueryTexture(hello_tex, NULL, NULL, &w, &h);
	SDL_QueryTexture(hellob_tex, NULL, NULL, &wb, &hb);
	SDL_QueryTexture(helloi_tex, NULL, NULL, &wi, &hi);
	//SDL_Rect dst = {(WINDOW_WIDTH - w) / 2, (WINDOW_HEIGHT - h) / 2, w, h};
	SDL_Rect dst = {(WINDOW_WIDTH - w) / 2, 0, w, h};
	SDL_Rect dstb = {(WINDOW_WIDTH - w) / 2, 80, wb, hb};
	SDL_Rect dsti = {(WINDOW_WIDTH - w) / 2, 160, wi, hi};

	// game loop
	SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
	SDL_RenderClear(rend);
	SDL_RenderCopy(rend, hello_tex, NULL, &dst);
	SDL_RenderCopy(rend, hellob_tex, NULL, &dstb);
	SDL_RenderCopy(rend, helloi_tex, NULL, &dsti);
	SDL_RenderPresent(rend);

	c = 0;
	SDL_Log("Main loop: start");
	while(!done) {
		while(SDL_PollEvent(&event)) {	// need to poll for events otherwise win becomes unresponsive
			if(event.type == SDL_QUIT) {
				done = true;
			}
		}

		c++;
		SDL_Delay(CYCLE_WAIT);
	}

	SDL_Log("Main loop: end\n");

	SDL_FreeSurface(hello_surf);
	SDL_FreeSurface(hellob_surf);
	SDL_FreeSurface(helloi_surf);
	SDL_DestroyTexture(hello_tex);
	SDL_DestroyTexture(hellob_tex);
	SDL_DestroyTexture(helloi_tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	TTF_CloseFont(consola);
	TTF_CloseFont(consolab);
	TTF_CloseFont(consolai);
	TTF_Quit();
	SDL_Quit();
	SDL_Log("Bye!");
	return 0;
}







