#include <stdio.h>
#include <SDL2/SDL.h>
#define WINDOW_WIDTH (640)
#define WINDOW_HEIGHT (480)

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

	SDL_Surface *surface = SDL_LoadBMP("res/moka.bmp");
	if(!surface) {
		printf("Cannot create surface: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Texture *tex = SDL_CreateTextureFromSurface(rend, surface);
	SDL_FreeSurface(surface);
	if(!tex) {
		printf("Cannot create texture: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Surface *paddle = SDL_LoadBMP("res/eriri.bmp");
	if(!paddle) {
		printf("Cannot create surface: %s\n", SDL_GetError());
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Texture *paddle_tex = SDL_CreateTextureFromSurface(rend, paddle);
	SDL_FreeSurface(paddle);
	if(!paddle_tex) {
		printf("Cannot create texture: %s\n", SDL_GetError());
		SDL_DestroyTexture(tex);
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Rect dest;
	SDL_QueryTexture(tex, NULL, NULL, &dest.w, &dest.h);

	SDL_Rect sprite;
	SDL_QueryTexture(paddle_tex, NULL, NULL, &sprite.w, &sprite.h);

	int close_it = 0;
	//int speed_x = 0; // pixels/second
	int speed_y = 360; // pixels/second
	dest.x = (WINDOW_WIDTH - dest.w) / 2;
	dest.y = 0;
	sprite.x = 0;
	sprite.y = WINDOW_HEIGHT - sprite.h;
	int paddle_speed = 720;

	while(!close_it) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				close_it = 1;
			} else if(event.type == SDL_KEYDOWN) {
				if(event.key.keysym.scancode == SDL_SCANCODE_LEFT) {
					sprite.x -= paddle_speed / 60;
					sprite.x = (sprite.x < 0) ? 0 : sprite.x;
				} else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
					sprite.x += paddle_speed / 60;
					sprite.x = (sprite.x > WINDOW_WIDTH - sprite.w) ?  WINDOW_WIDTH - sprite.w : sprite.x;
				}
			}
		}

		SDL_RenderClear(rend);

		SDL_RenderCopy(rend, tex, NULL, &dest);
		SDL_RenderCopy(rend, paddle_tex, NULL, &sprite);
		SDL_RenderPresent(rend);

		dest.y += speed_y / 60;

		if(dest.y > WINDOW_HEIGHT - dest.h) {
			dest.y = WINDOW_HEIGHT - dest.h;
			speed_y = -speed_y;	
		} else if(dest.y < 0) {
			dest.y = 0;
			speed_y = -speed_y;
		}

		SDL_Delay(1000/60);
	}
	
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
