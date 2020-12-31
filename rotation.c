#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH (320)
#define WINDOW_HEIGHT (240)

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

	sprite1_rect.x = (WINDOW_WIDTH - sprite1_rect.w) / 2;
	sprite1_rect.y = (WINDOW_HEIGHT - sprite1_rect.h) / 2;

	int done = 0;
	float rotation = 0;
	SDL_RendererFlip flip;
	bool flip_y = false;
	bool flip_x = false;
	while(!done) {
		flip = SDL_FLIP_NONE;

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				done = 1;
			} else if(event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.scancode) {
					case SDL_SCANCODE_KP_MINUS:
						rotation -= 10.0f;
						if(rotation < 0.0f) rotation += 360.0f;
						break;
					case SDL_SCANCODE_KP_PLUS:
						rotation += 10.0f;
						if(rotation > 360.0f) rotation -= 360.0f;
						break;
					case SDL_SCANCODE_UP:
						flip_y = false;
						break;
					case SDL_SCANCODE_DOWN:
						flip_y = true;
						break;
					case SDL_SCANCODE_LEFT:
						flip_x = false;
						break;
					case SDL_SCANCODE_RIGHT:
						flip_x = true;
						break;
					case SDL_SCANCODE_KP_0:
						rotation = 0;
						break;
					default:
						break;
				}
			}
		}

		if(flip_y) flip |= SDL_FLIP_VERTICAL;
		if(flip_x) flip |= SDL_FLIP_HORIZONTAL;

		SDL_RenderCopyEx(rend, sprite1_tex, NULL, &sprite1_rect, rotation, NULL, flip);
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







