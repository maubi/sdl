#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct vector {
	float x;
	float y;	
} vector_t;

typedef struct player {
	vector_t location;
	vector_t velocity;
	vector_t acceleration;
	SDL_Texture *tex;
	float mass;
	int w;
	int h;
	int moveup;
	int movedown;
	int moveleft;
	int moveright;
} player_t;

typedef struct game {
	player_t player;
	vector_t gravity;
	vector_t wind;
	int wind_p[10];
	SDL_Window *win;
	SDL_Renderer *rend;
	bool quit;
	float drag_c;
} game_t;

int setup();
void handle_events();
void update();
void draw();
void cleanup();
void vector_add(vector_t *u, const vector_t w);
void vector_mul(vector_t *u, const float n);
void vector_div(vector_t *v, const float n);
float vector_mag(const vector_t v);
void vector_normalize(vector_t *v);
int _rand(int min, int max);
bool _float_equals(float n, float m); 
void apply_force(vector_t *acc, vector_t force, const float mass);

game_t game;

int WinMain(int argc, char **argv) {

	if(setup()) {
		return 1;
	}

	int c = 0;
	while(!game.quit) {
		SDL_Log("Cycle: %d", c);

		handle_events();
		update();
		draw();

		c++;
		SDL_Delay(1000/60);
	}

	cleanup();
	printf("Bye!\n");

	return 0;
}

int setup() {					// init game at startup
	SDL_Texture *t;

	time_t seed;
	srand((unsigned) time(&seed));

	if((SDL_Init(SDL_INIT_VIDEO)) != 0){
		SDL_Log("Cannot initialize: %s\n", SDL_GetError());
		return 1;
	}	

	SDL_Log("Initialization successful\n");

	game.win = SDL_CreateWindow("DIANA",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if(!game.win) {
		SDL_Log("Cannot create main window: %s\n", SDL_GetError());
		return 1;
	}

	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	game.rend = SDL_CreateRenderer(game.win, -1, render_flags);
	if(!game.rend) {
		SDL_Log("Cannot create renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(game.win);
		return 1;
	}

	player_t p;
	p.w = 100;
	p.h = 100;

	t = SDL_CreateTexture(
		game.rend,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_TARGET,
		p.w, p.h
	);	

	SDL_SetRenderTarget(game.rend, t);
	SDL_SetRenderDrawColor(game.rend, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderClear(game.rend);
	SDL_SetRenderTarget(game.rend, NULL);
	p.tex = t;
	p.location.x = (WINDOW_WIDTH - p.w) / 2.0f;
	//p.location.y = (WINDOW_HEIGHT - p.h) / 2.0f;
	p.location.y = WINDOW_HEIGHT - p.h;
	p.velocity.x = 0;
	p.velocity.y = 0;
	p.moveup = false;
	p.movedown = false;
	p.moveleft = false;
	p.moveright = false;
	p.mass = 2;

	game.player = p;

	game.wind.x =  0.0f;
	game.wind.y = 0.0f;

	game.wind_p[0] = 0.0f;
	game.wind_p[1] = 1.2f;
	game.wind_p[2] = 1.3f;
	game.wind_p[3] = 1.0f;
	game.wind_p[4] = 2.0f;
	game.wind_p[5] = 2.2f;
	game.wind_p[6] = 2.4f;
	game.wind_p[7] = 3.1f;
	game.wind_p[8] = 3.3f;
	game.wind_p[9] = 4.8f;

	game.drag_c = 2.0f;

	game.quit = false;
	return 0;
}

void handle_events() {			// handle game events
	SDL_PumpEvents();
	SDL_Event e;
	while(SDL_PollEvent(&e)) {
		if(e.type == SDL_QUIT) {
			game.quit = true;
		}
		if(e.type == SDL_KEYDOWN) {
			if(e.key.keysym.scancode == SDL_SCANCODE_UP) {
				game.player.moveup += 1;
			}
			if(e.key.keysym.scancode == SDL_SCANCODE_DOWN) {
				game.player.movedown += 1;
			}
			if(e.key.keysym.scancode == SDL_SCANCODE_LEFT) {
				game.player.moveleft += 1;
			}
			if(e.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
				game.player.moveright += 1;
			}
		}
		if(e.type == SDL_KEYUP) {
			if(e.key.keysym.scancode == SDL_SCANCODE_UP) {
				game.player.moveup = 0;
			}
			if(e.key.keysym.scancode == SDL_SCANCODE_DOWN) {
				game.player.movedown = 0;
			}
			if(e.key.keysym.scancode == SDL_SCANCODE_LEFT) {
				game.player.moveleft = 0;
			}
			if(e.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
				game.player.moveright = 0;
			}
		}
	}
}

void update() {					// update the state of the game
	//game.wind.x = game.wind_p[_rand(0, 10)] / 60.0f * -1;
	game.wind.x = game.wind_p[_rand(0, 10)] / 60.0f * -1 * 2;
	//game.wind.y = game.wind_p[_rand(0, 10)] / 60.0f * -1;
	game.wind.y = 0;

	vector_mul(&game.player.acceleration, 0);

	if(game.player.moveup > 0) {
		game.player.acceleration.y += -3.0f / 60.0f;
	}
	if(game.player.movedown > 0) {
		game.player.acceleration.y += 3.0f / 60.0f;
	}
	if(game.player.moveleft > 0) {
		game.player.acceleration.x += -3.0f / 60.0f;
	}
	if(game.player.moveright > 0) {
		game.player.acceleration.x += 3.0f / 60.0f;
	}

	float speed = vector_mag(game.player.velocity);
	float drag_mag = game.drag_c * speed * speed / 60.0f;
	vector_t drag = game.player.velocity;
	vector_mul(&drag, -1);
	vector_normalize(&drag);
	vector_mul(&drag, drag_mag);

	apply_force(&game.player.acceleration, drag, game.player.mass);
	apply_force(&game.player.acceleration, game.wind, game.player.mass);

	vector_add(&game.player.velocity, game.player.acceleration);
	vector_add(&game.player.location, game.player.velocity);
	
	if(game.player.location.x > (WINDOW_WIDTH - game.player.w)) { 
		game.player.location.x = WINDOW_WIDTH - game.player.w;
		game.player.velocity.x *= -1;
	} else if (game.player.location.x < 0) {
		game.player.location.x = 0;
		game.player.velocity.x *= -1;
	}
	if(game.player.location.y > (WINDOW_HEIGHT - game.player.h)) { 
		game.player.location.y = WINDOW_HEIGHT - game.player.h;
		game.player.velocity.y *= -1;
	} else if (game.player.location.y < 0) {
		game.player.location.y = 0;
		game.player.velocity.y *= -1;
	}
}

void draw() {					// render the game to screen
	SDL_SetRenderDrawColor(game.rend, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(game.rend);

	SDL_Rect r;
	r.x = game.player.location.x;
	r.y = game.player.location.y;
	r.w = game.player.w;
	r.h = game.player.h;
	SDL_RenderCopy(game.rend, game.player.tex, NULL, &r);
	
	SDL_RenderPresent(game.rend);
}

void cleanup() {
	SDL_DestroyRenderer(game.rend);
	SDL_DestroyWindow(game.win);
	SDL_Quit();
}

void vector_add(vector_t *u, const vector_t v) {
	u->x += v.x;
	u->y += v.y;
}

void vector_mul(vector_t *u, const float n) {
	u->x *= n;
	u->y *= n;
}

void vector_div(vector_t *v, const float n) {
	if(n == 0) return;
	v->x /= n;
	v->y /= n;
}

float vector_mag(const vector_t v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

void vector_normalize(vector_t *v) {
	float m = vector_mag(*v);
	if(m == 0) return;
	vector_div(v, m);
}

int _rand(int min, int max) {
	return ( rand() % (max - min) ) + min;
}

bool _float_equals(float n, float m) {
	float precision = 0.000001;
	return ( ( fabs(n) - fabs(m) ) < precision);
}

void apply_force(vector_t *acc, vector_t force, const float mass) {
	vector_div(&force, mass);
	vector_add(acc, force);	
}


