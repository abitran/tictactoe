#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>

#define S_WIDTH 800
#define S_HEIGHT 600

#define ROWS 3 
#define COLS 3

#define CELLW 266
#define CELLH 200

int sdlchk(int);

void *sdlpt(void *);

void render_grid(SDL_Renderer *);

int main(int argc, char **argv) {
  sdlchk((SDL_Init(SDL_INIT_VIDEO)));

  SDL_Window *window =
      sdlpt(SDL_CreateWindow("Tictactoe", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, S_WIDTH, S_HEIGHT, 0));

  Uint32 render_flags = SDL_RENDERER_ACCELERATED;

  SDL_Renderer *renderer = sdlpt(SDL_CreateRenderer(window, -1, render_flags));

  int quit = 0;
  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          quit = 1;
        break;
      }
    }

    sdlchk(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255));
    sdlchk(SDL_RenderClear(renderer));

    render_grid(renderer);

    SDL_RenderPresent(renderer);

  }


  return 0;
}

int sdlchk(int code) {
  if (code != 0) {
    fprintf(stderr, "SDL error: %s\n", SDL_GetError());
    exit(1);
  }

  return code;
}

void *sdlpt(void *ptr) {
  if (ptr == NULL) {
    fprintf(stderr, "SDL error: %s\n", SDL_GetError());
    exit(1);
  }
  return ptr;
}

void render_grid(SDL_Renderer *renderer) {

  sdlchk(SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255));
  for (int x = 0; x < ROWS; x++) {
    sdlchk(SDL_RenderDrawLine(renderer, 
                       x * CELLW, 
                       0, 
                       x * CELLW, S_HEIGHT));

  }
  for (int y = 0; y < COLS; y++) {
    sdlchk(SDL_RenderDrawLine(renderer, 
                              0, 
                              y * CELLH,
                              S_WIDTH, 
                              y * CELLH));
  }
}



