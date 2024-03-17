#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>

#define S_WIDTH 800
#define S_HEIGHT 600

int sdlchk(int);

void *sdlpt(void *);

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

    sdlchk(SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255));
    sdlchk(SDL_RenderClear(renderer));

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
