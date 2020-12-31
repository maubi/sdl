#include <stdio.h>
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

	SDL_Surface *sprite1_surf = IMG_Load("res/ship.png");
	if(!sprite1_surf) {
		printf("Cannot create surface: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Texture *sprite1_tex = SDL_CreateTextureFromSurface(rend, sprite1_surf);
	//SDL_FreeSurface(surface);
	if(!sprite1_tex) {
		printf("Cannot create texture: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Rect sprite1_rect;
	SDL_QueryTexture(sprite1_tex, NULL, NULL, &sprite1_rect.w, &sprite1_rect.h);

	sprite1_rect.x = 0;
	sprite1_rect.y = 0;

	int done = 0;
	float x, y, speed;
	speed = 0.003f;
	x = y = 0.0f;
	while(!done) {
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				done = 1;
			}
		}

		x += speed;
		y += speed;

		sprite1_rect.x = (int) ceil(x * 10.0);
		//sprite1_rect.y = (int) (y * 10.0);
		sprite1_rect.y = 0;

		SDL_RenderCopy(rend, sprite1_tex, NULL, &sprite1_rect);
		SDL_RenderPresent(rend);

		SDL_Delay(1000/60);
	}


	printf("Bye!\n");

	SDL_DestroyTexture(sprite1_tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}








