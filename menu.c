#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SKETCH_1_CYCLES 10
#define SKETCH_2_CYCLES 1

int menu(SDL_Renderer *rend);
void restore_screen(SDL_Renderer *rend, SDL_Surface *sshot);
SDL_Surface *save_screen(SDL_Renderer *rend);

int WinMain(int argc, char **argv) {
/*
	SDL_version compiled;
	SDL_version linked;

	SDL_VERSION(&compiled);
	SDL_GetVersion(&linked);
	printf("We compiled against SDL version %d.%d.%d ...\n",
       	compiled.major, compiled.minor, compiled.patch);
	printf("But we are linking against SDL version %d.%d.%d.\n",
		linked.major, linked.minor, linked.patch);
*/

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

	// sketch 1
	c = 0;
	bool done = false;
	while(!done) {
		SDL_Log("Cycle: %d", c);

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);
		SDL_RenderPresent(rend);

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_KEYUP) {
				if(event.key.keysym.scancode == SDL_SCANCODE_P) {
					if(menu(rend)) {
					}
				}
			} else if(event.type == SDL_QUIT) {
				done = true;
			}
		}

		c++;
		SDL_Delay(1000 / 100);
	}

	printf("Bye!\n");

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

int menu(SDL_Renderer *rend) {
	const int w = 400;
	const int h = 200;
	SDL_Rect rect;
	SDL_Event event;

	// draws menu
	rect.x = (WINDOW_WIDTH - w) / 2;
	rect.y = (WINDOW_HEIGHT - h) / 2;
	rect.w = w;
	rect.h = h;
	SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
	SDL_RenderDrawRect(rend, &rect);
	SDL_RenderPresent(rend);

	// wait for user action
	while(true) {
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_MOUSEBUTTONUP) {
				if(event.button.button == SDL_BUTTON_LEFT) {
					SDL_Point p;	
					p.x = event.button.x;
					p.y = event.button.y;
					if(SDL_PointInRect(&p, &rect)) {
						return 1;
					}
				}
			}
		}
		SDL_Delay(50);
	}
}

SDL_Surface *save_screen(SDL_Renderer *rend) {
	// take a screenshot
	SDL_Surface *sshot = SDL_CreateRGBSurfaceWithFormat(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);

	if(sshot == NULL) {
		SDL_Log("Screenshot failed");
	}

	if(SDL_RenderReadPixels(rend, NULL, SDL_PIXELFORMAT_RGBA32, sshot->pixels, sshot->pitch)) {
		SDL_Log("Cannot read pixels");
	}


	return sshot;
}

void restore_screen(SDL_Renderer *rend, SDL_Surface *sshot) {
	// restore screenshot
	SDL_Texture *sshot_tex = SDL_CreateTextureFromSurface(rend, sshot);
	SDL_RenderCopy(rend, sshot_tex, NULL, NULL);
	SDL_RenderPresent(rend);
}






