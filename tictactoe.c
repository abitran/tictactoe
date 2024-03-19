#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>

#define S_WIDTH 800
#define S_HEIGHT 600

#define ROWS 3 
#define COLS 3

#define CELLW 266
#define CELLH 200

typedef enum{
  RUNNING=0,  
  PLAYER_X_WON, 
  PLAYER_O_WON, 
  TIED, 
  QUIT 
}State;

typedef enum {
  EMPTY=0,
  PLAYER_X,
  PLAYER_O
}Player;

typedef struct {
  int grid[ROWS * COLS];
  Player player;
  State state;
}Game;

int sdlchk(int);

void *sdlpt(void *);

void render_game(SDL_Renderer *, const Game *);

void selectsq(Game *, int, int);

void render_running(SDL_Renderer *, const Game *);

void render_grid(SDL_Renderer *, const int *, 
                 const SDL_Color *, const SDL_Color *);


void render_x(SDL_Renderer *, int, int, const SDL_Color *);
void render_o(SDL_Renderer *, int, int, const SDL_Color *);

const SDL_Color PLAYER_X_COLOR = { .r=255, .g=0, .b=0};
const SDL_Color PLAYER_O_COLOR = { .r=0, .g=0, .b=255};

int main(int argc, char **argv) {
 
  // Initial state of the game.
  Game game = {
      .grid = {PLAYER_X, EMPTY, EMPTY, 
               EMPTY, EMPTY, EMPTY, 
              EMPTY, EMPTY, EMPTY},
      .player=PLAYER_X,
      .state=RUNNING
  };

  sdlchk((SDL_Init(SDL_INIT_VIDEO)));

  SDL_Window *window =
      sdlpt(SDL_CreateWindow("Tictactoe", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, S_WIDTH, S_HEIGHT, SDL_WINDOW_SHOWN));

  Uint32 render_flags = SDL_RENDERER_ACCELERATED;

  SDL_Renderer *renderer = sdlpt(SDL_CreateRenderer(window, -1, render_flags));

  while (game.state != QUIT) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          game.state = QUIT;
          break;

        case SDL_MOUSEBUTTONDOWN:
          selectsq(&game, event.button.x / CELLW, event.button.y / CELLH);
          break;
      }
    }

    sdlchk(SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255));
    sdlchk(SDL_RenderClear(renderer));
    render_game(renderer, &game);
    SDL_RenderPresent(renderer);

  }

  SDL_DestroyWindow(window);
  SDL_Quit();


  return EXIT_SUCCESS;
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

void render_grid(SDL_Renderer *renderer, 
                 const int *grid, 
                 const SDL_Color *player_x_color, 
                 const SDL_Color *player_o_color) {

  sdlchk(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255));
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

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      switch (grid[i*ROWS + j]) {
        case PLAYER_X:
          render_x(renderer, i, j, player_x_color);
          break;

        case PLAYER_O:
          render_o(renderer, i, j, player_o_color);
          break;
        default:{}
        
      }
    }
  }
}

void render_game(SDL_Renderer *renderer, const Game *game) {

  switch (game->state) {
    case RUNNING:
      render_running(renderer, game);
      break;
    case PLAYER_O_WON:
      break;
    case PLAYER_X_WON:
      break;
    case TIED:
      break;
    default:
      break;
  }

}

void render_running(SDL_Renderer *renderer, const Game *game) {

  render_grid(renderer, game->grid, &PLAYER_X_COLOR, 
              &PLAYER_O_COLOR);
  
}

void selectsq(Game *game, int row, int col) {


}

void render_x(SDL_Renderer *renderer, int row, int col, const SDL_Color *color) {

  const float hatzi_box = fmin(CELLW, CELLH) * 0.25;
  const float center_x = CELLW * 0.5 + col * CELLW;
  const float center_y = CELLH * 0.5 + row * CELLH;
  thickLineRGBA(renderer, 
                center_x - hatzi_box, 
                center_y - hatzi_box, 
                center_x + hatzi_box, 
                center_y + hatzi_box, 
                20, color->r, color->g, 
                color->b, 255);
  thickLineRGBA(renderer, 
                center_x + hatzi_box, 
                center_y - hatzi_box, 
                center_x - hatzi_box, 
                center_y + hatzi_box, 
                20, color->r, color->g, 
                color->b, 255);
}

void render_o(SDL_Renderer *renderer, int row, int col, const SDL_Color *color){

}


