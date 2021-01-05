#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define SKETCH_1_CYCLES 1
#define SKETCH_2_CYCLES 1

typedef struct color {
	int r;
	int g;
	int b;
} color_t;

void draw_cantor(SDL_Renderer *rend, int n, int x, int y, int len);
void draw_circles(SDL_Renderer *rend, int x, int y, int radius, color_t color);
void draw_circle(SDL_Renderer *rend, int x_center, int y_center, int radius, color_t color);
void draw_squares(SDL_Renderer * rend, int x, int y, int len, color_t color);

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

	time_t t;
	int c;
	srand((unsigned) time(&t));
	SDL_Event event;

	// sketch 1
	int x, y, w, h;
	c = 0;
	while(c < SKETCH_1_CYCLES) {
		SDL_Log("Cycle: %d", c);

		SDL_PumpEvents();
		SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		draw_cantor(rend, 3, 0, 0, WINDOW_WIDTH - 1);	
		SDL_RenderPresent(rend);

		c++;
		SDL_Delay(1000);
	}

	// sketch2
	c = 0;
	while(c < SKETCH_2_CYCLES) {
		SDL_Log("Cycle: %d", c);

		color_t color = {255, 255, 255};

		SDL_PumpEvents();
		SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		draw_circles(rend, WINDOW_WIDTH / 2 - 1, WINDOW_HEIGHT / 2 - 1, WINDOW_HEIGHT / 3 - 1, color);
		SDL_RenderPresent(rend);
		SDL_Delay(1000);

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		draw_squares(rend, WINDOW_WIDTH / 2 - 1 - 200 , WINDOW_HEIGHT / 2 - 1 - 200, 400, color);
		SDL_RenderPresent(rend);
		SDL_Delay(1000);

		c++;
	}

	printf("Bye!\n");

	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}

void draw_cantor(SDL_Renderer *rend, int n, int x, int y, int len) {
	SDL_PumpEvents();
	SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

	if(len <= 1) return;

	SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
	SDL_RenderDrawLine(rend, x, y, x + len, y);

	int left_x = x;
	int left_y = y + 10;
	int left_len = (int) (len / 3.0f);

	int right_x = (int) ( x + (2.0f / 3.0f * (float) len) );
	int right_y = y + 10;
	int right_len = (int) (len / 3.0f);

	draw_cantor(rend, n, left_x, left_y, left_len);
	draw_cantor(rend, n, right_x, right_y, right_len);
}

void draw_circles(SDL_Renderer * rend, int x, int y, int radius, color_t color) {
	SDL_PumpEvents();
	SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

	draw_circle(rend, x, y, radius, color);

	color_t c;
	if(radius > 8) {
		c.r = 255; c.g = 0; c.b = 0;
		draw_circles(rend, x + radius, y, radius / 2, c);
		c.r = 0; c.g = 255; c.b = 0;
		draw_circles(rend, x - radius, y, radius / 2, c);
		c.r = 0; c.g = 0; c.b = 255;
		draw_circles(rend, x, y + radius, radius / 2, c);
		c.r = 255; c.g = 255; c.b = 0;
		draw_circles(rend, x, y - radius, radius / 2, c);

		// FIXME: do diagonals
		//draw_circles(rend, x + radius / 2, y + radius / 2, radius / 2);
		//draw_circles(rend, x + radius / 2, y - radius / 2, radius / 2);

		//draw_circles(rend, x - radius / 2, y + radius / 2, radius / 2);
		//draw_circles(rend, x - radius / 2, y - radius / 2, radius / 2);
	}
}

void draw_circle(SDL_Renderer *rend, int x0, int y0, int radius, color_t color) {
	int x = radius;
	int y = 0;

	int err = 0;
	SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, 255);
	while(x >= y) {
		SDL_RenderDrawPoint(rend, x0 + x , y0 + y);
		SDL_RenderDrawPoint(rend, x0 + y , y0 + x);
		SDL_RenderDrawPoint(rend, x0 - y , y0 + x);
		SDL_RenderDrawPoint(rend, x0 - x , y0 + y);
		SDL_RenderDrawPoint(rend, x0 - x , y0 - y);
		SDL_RenderDrawPoint(rend, x0 - x , y0 - y);
		SDL_RenderDrawPoint(rend, x0 - y , y0 - x);
		SDL_RenderDrawPoint(rend, x0 + y , y0 - x);
		SDL_RenderDrawPoint(rend, x0 + x , y0 - y);

		if(err <= 0) {
			y += 1;
			err += 2 * y + 1;
		} else {
			x -= 1;
			err -= 2 * x + 1;
		}
	}
}

void draw_squares(SDL_Renderer * rend, int x, int y, int len, color_t color) {
	SDL_PumpEvents();
	SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);

	// SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, 255); 	//multi-color
	//SDL_SetRenderDrawColor(rend, 128, 0, 128, 255);	// monochrome
	int r = rand() % 256;
	int g = rand() % 256;
	int b = rand() % 256;
	SDL_SetRenderDrawColor(rend, r, g, b, 255);	// monochrome
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = len;
	rect.h = len;
	SDL_RenderDrawRect(rend, &rect);
	//SDL_RenderFillRect(rend, &rect);

	color_t c;
	if(len > 12) {
		c.r = 255; c.g = 0; c.b = 0;
		draw_squares(rend, x + len * 3 / 4, y + len / 4, len / 2, c);	// right
		c.r = 0; c.g = 255; c.b = 0;
		draw_squares(rend, x - len  / 4, y + len / 4, len / 2, c);	// left 
		c.r = 0; c.g = 0; c.b = 255;

		draw_squares(rend, x + len / 4, y - len / 4, len / 2, c);	// up
		c.r = 255; c.g = 255; c.b = 0;
		draw_squares(rend, x + len / 4, y + len * 3 / 4, len / 2, c);	// down

		// FIXME: draw diagonals
		//draw_circles(rend, x + radius / 2, y + radius / 2, radius / 2);
		//draw_circles(rend, x + radius / 2, y - radius / 2, radius / 2);

		//draw_circles(rend, x - radius / 2, y + radius / 2, radius / 2);
		//draw_circles(rend, x - radius / 2, y - radius / 2, radius / 2);
	}
}















