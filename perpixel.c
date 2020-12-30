#include <stdio.h>
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

	//SDL_Surface *sprite2_surf = SDL_LoadBMP("res/sprite2.bmp");
	SDL_Surface *sprite2_surf = IMG_Load("res/sprite2.png");
	if(!sprite2_surf) {
		printf("Cannot create surface: %s\n", SDL_GetError());
		SDL_DestroyTexture(sprite1_tex);
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Texture *sprite2_tex = SDL_CreateTextureFromSurface(rend, sprite2_surf);
	if(!sprite2_tex) {
		printf("Cannot create texture: %s\n", SDL_GetError());
		SDL_DestroyTexture(sprite1_tex);
		SDL_DestroyRenderer(rend);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	SDL_Rect sprite2_rect;
	SDL_QueryTexture(sprite2_tex, NULL, NULL, &sprite2_rect.w, &sprite2_rect.h);

	int done = 0;
	sprite1_rect.x = 0;
	sprite1_rect.y = WINDOW_HEIGHT - sprite1_rect.h;
	sprite2_rect.x = (WINDOW_WIDTH - sprite2_rect.w) / 2;
	sprite2_rect.y = WINDOW_HEIGHT - sprite2_rect.h;

	while(!done) {
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderClear(rend);

		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				done = 1;
			} else if(event.type == SDL_KEYDOWN) {	// FIXME
				switch(event.key.keysym.scancode) {
					case SDL_SCANCODE_LEFT:
						sprite1_rect.x -= 10;
						break;
					case SDL_SCANCODE_RIGHT:
						sprite1_rect.x += 10;
						break;
					case SDL_SCANCODE_UP:
						sprite2_rect.y -= 10;
						break;
					case SDL_SCANCODE_DOWN:
						sprite2_rect.y += 10;
						break;
					default:
						break;
				}
			}
		}

		SDL_Rect intersect;

		if(SDL_IntersectRect(&sprite1_rect, &sprite2_rect, &intersect)) {
			//printf(">>>>>> INTERSECT <<<<\n");
			int x, y;
			int collision = 0;
			for(y = intersect.y; y < intersect.h + intersect.y; y++) {
				for(x = intersect.x; x < intersect.w + intersect.x; x++) {
					int x1 = x - sprite1_rect.x;
					int y1 = y - sprite1_rect.y;
					int x2 = x - sprite2_rect.x;
					int y2 = y - sprite2_rect.y;

					Uint32 *pixels1 = (Uint32 *) sprite1_surf->pixels;
					Uint32 *pixels2 = (Uint32 *) sprite2_surf->pixels;

				//	printf("x: %d, y: %d\n", x, y);
				//	printf("sp1.x: %d, sp1.y: %d\n", sprite1_rect.x, sprite1_rect.y);
					//printf("%d %d %d %d\n", x1, y1, x2, y2);

					Uint32 *p1 = pixels1 + y1 * sprite1_surf->w + x1;
					Uint32 point1 = *p1;
					Uint32 *p2 = pixels2 + y2 * sprite2_surf->w + x2;
					Uint32 point2 = *p2;

					//printf("point1:%u, point2: %u\n", point1, point2);

					Uint8 red1, green1, blue1, alpha1, red2, green2, blue2, alpha2;
					SDL_GetRGBA(point1, sprite1_surf->format, &red1, &green1, &blue1, &alpha1);
					SDL_GetRGBA(point2, sprite2_surf->format, &red2, &green2, &blue2, &alpha2);

					//printf("a1: %u, a2: %u\n", alpha1, alpha2);

					if(alpha1 != 0 && alpha2 != 0) {
						collision = 1;
					}
				}
			}
			if(collision) {
				SDL_SetRenderDrawColor(rend, 255, 255, 0, 255);
				SDL_RenderFillRect(rend, &intersect);
				//printf("collision!\n");
			} else {
				SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
				SDL_RenderFillRect(rend, &intersect);
			}
			//printf("---------\n");
		}

		SDL_RenderCopy(rend, sprite1_tex, NULL, &sprite1_rect);
		SDL_RenderCopy(rend, sprite2_tex, NULL, &sprite2_rect);
		SDL_RenderPresent(rend);

		SDL_Delay(1000/60);
	}


	printf("Bye!\n");

	SDL_DestroyTexture(sprite1_tex);
	SDL_DestroyTexture(sprite2_tex);
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}






