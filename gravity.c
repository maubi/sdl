#include <stdio.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH (320)
#define WINDOW_HEIGHT (600)	// 10 pixel = 1m (60m)

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

	//SDL_Surface *sprite1_surf = SDL_LoadBMP("res/sprite1.bmp");
	SDL_Surface *sprite1_surf = IMG_Load("res/sprite1.png");
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

	int done = 0;
	sprite1_rect.x = (WINDOW_WIDTH - sprite1_rect.w) / 2;
	sprite1_rect.y = 0;
	const float G = 9.80665f;	// m/s^2
	float speed_y = 0.0f;
	int c = 0;
	float total_displ = 0.0f;
	while(!done) {
		c++;

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				done = 1;
			} 
		}

		float final_y = speed_y + G * 0.01f;
		float displ_y = speed_y * 0.01f + 0.5f * G * pow(0.01f,2.0f);
		int dp = (int) (round(displ_y * 10.0f * 10.0f));

		if(sprite1_rect.y + dp >= (WINDOW_HEIGHT - sprite1_rect.h)) {
			break;
		}

		total_displ += displ_y;
		speed_y = final_y;

		sprite1_rect.y = sprite1_rect.y + dp;

		printf("[%d] final_y: %3.3f, displ_y: %3.3f, dp: %d\n", c, final_y, displ_y, dp);

		SDL_RenderCopy(rend, sprite1_tex, NULL, &sprite1_rect);
		SDL_RenderPresent(rend);

		SDL_Delay(1000/100);
	}

	printf("\n");
	printf("total frames = %d\n", c);
	printf("final speed = %3.3f\n", speed_y);
	printf("total displ = %3.3f\n", total_displ);

	SDL_DestroyTexture(sprite1_tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}







