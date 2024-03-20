#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>

#define S_WIDTH 800
#define S_HEIGHT 600

#define ROWS 3
#define COLS 3

#define CELLW 266
#define CELLH 200

typedef enum { RUNNING = 0, PLAYER_X_WON, PLAYER_O_WON, TIED, QUIT } State;

typedef enum { EMPTY = 0, PLAYER_X, PLAYER_O } Player;

typedef struct {
  int grid[ROWS * COLS];
  Player player;
  State state;
} Game;

int sdlchk(int);

void *sdlpt(void *);

void render_game(SDL_Renderer *, const Game *);

void selectsq(Game *, int, int);

void render_running(SDL_Renderer *, const Game *);

void render_end(SDL_Renderer *, const Game *, const SDL_Color *);

void render_grid(SDL_Renderer *, const int *, const SDL_Color *,
                 const SDL_Color *);

void render_x(SDL_Renderer *, int, int, const SDL_Color *);
void render_o(SDL_Renderer *, int, int, const SDL_Color *);

void play(Game *, int, int);

void reset(Game *);

void switch_player(Game *);

void game_over(Game *);

int count_cells(const int *, int);

int check_player_won(Game *, int);

const SDL_Color PLAYER_X_COLOR = {.r = 255, .g = 0, .b = 0};
const SDL_Color PLAYER_O_COLOR = {.r = 0, .g = 0, .b = 255};
const SDL_Color TIED_COLOR = {.r = 130, .g = 130, .b = 130};

int main(int argc, char **argv) {

  // Initial state of the game.
  Game game = {
      .grid = {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
      .player = PLAYER_X,
      .state = RUNNING};

  sdlchk((SDL_Init(SDL_INIT_VIDEO)));

  SDL_Window *window = sdlpt(SDL_CreateWindow(
      "Tictactoe", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, S_WIDTH,
      S_HEIGHT, SDL_WINDOW_SHOWN));

  Uint32 render_flags = SDL_RENDERER_ACCELERATED;

  SDL_Renderer *renderer = sdlpt(SDL_CreateRenderer(window, -1, render_flags));

  SDL_Event event;
  while (game.state != QUIT) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        game.state = QUIT;
        break;

      case SDL_MOUSEBUTTONDOWN:
        selectsq(&game, event.button.y / CELLW, event.button.x / CELLW);
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

void render_grid(SDL_Renderer *renderer, const int *grid,
                 const SDL_Color *player_x_color,
                 const SDL_Color *player_o_color) {

  sdlchk(SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255));
  for (int x = 0; x < ROWS; x++) {
    sdlchk(SDL_RenderDrawLine(renderer, x * CELLW, 0, x * CELLW, S_HEIGHT));
  }
  for (int y = 0; y < COLS; y++) {
    sdlchk(SDL_RenderDrawLine(renderer, 0, y * CELLH, S_WIDTH, y * CELLH));
  }

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      switch (grid[i * ROWS + j]) {
      case PLAYER_X:
        render_x(renderer, i, j, player_x_color);
        break;

      case PLAYER_O:
        render_o(renderer, i, j, player_o_color);
        break;
      default: {
        }
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
    render_end(renderer, game, &PLAYER_O_COLOR);
    break;
  case PLAYER_X_WON:
    render_end(renderer, game, &PLAYER_X_COLOR);
    break;
  case TIED:
    render_end(renderer, game, &TIED_COLOR);
    break;
  default:
    break;
  }
}

void render_running(SDL_Renderer *renderer, const Game *game) {

  render_grid(renderer, game->grid, &PLAYER_X_COLOR, &PLAYER_O_COLOR);
}

void render_end(SDL_Renderer *renderer, const Game *game,
                const SDL_Color *color) {
  render_grid(renderer, game->grid, color, color);
}

void selectsq(Game *game, int row, int col) {
  if (game->state == RUNNING)
    play(game, row, col);
  else
    reset(game);
}

void play(Game *game, int row, int col) {
  if (game->grid[row * ROWS + col] == EMPTY) {
    game->grid[row * ROWS + col] = game->player;
    switch_player(game);
    game_over(game);
  }
}

void switch_player(Game *game) {
  if (game->player == PLAYER_X)
    game->player = PLAYER_O;
  else if (game->player == PLAYER_O)
    game->player = PLAYER_X;
}

void reset(Game *game) {
  game->player = PLAYER_X;
  game->state = RUNNING;
  for (int i = 0; i < ROWS * COLS; i++)
    game->grid[i] = EMPTY;
}

int count_cells(const int *grid, int cell) {

  int counter = 0;
  for (int i = 0; i < ROWS * COLS; i++) {
    if (grid[i] == cell)
      counter++;
  }

  return counter;
}

void game_over(Game *game) {
  if (check_player_won(game, PLAYER_X))
    game->state = PLAYER_X_WON;
  else if (check_player_won(game, PLAYER_O))
    game->state = PLAYER_O_WON;
  else if (count_cells(game->grid, EMPTY) == 0)
    game->state = TIED;
}

int check_player_won(Game *game, int player) {
  int row_count = 0, col_count = 0;
  int diag1_count = 0, diag2_count = 0;

  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (game->grid[i * ROWS + j] == player)
        row_count++;
      if (game->grid[j * COLS + i] == player)
        col_count++;
    }
    if (row_count >= ROWS || col_count >= COLS)
      return 1;

    row_count = 0;
    col_count = 0;

    if (game->grid[i * ROWS + i] == player)
      diag1_count++;

    if (game->grid[i * COLS + COLS - i - 1] == player)
      diag2_count++;
  }

  return diag1_count >= ROWS || diag2_count >= COLS;
}

void render_x(SDL_Renderer *renderer, int row, int col,
              const SDL_Color *color) {

  const float hatzi_box = fmin(CELLW, CELLH) * 0.25;
  const float center_x = CELLW * 0.5 + col * CELLW;
  const float center_y = CELLH * 0.5 + row * CELLH;
  thickLineRGBA(renderer, center_x - hatzi_box, center_y - hatzi_box,
                center_x + hatzi_box, center_y + hatzi_box, 20, color->r,
                color->g, color->b, 255);
  thickLineRGBA(renderer, center_x + hatzi_box, center_y - hatzi_box,
                center_x - hatzi_box, center_y + hatzi_box, 20, color->r,
                color->g, color->b, 255);
}

void render_o(SDL_Renderer *renderer, int row, int col,
              const SDL_Color *color) {

  const float hatzi_box = fmin(CELLW, CELLH) * 0.4;
  const float center_x = CELLW * 0.5 + col * CELLW;
  const float center_y = CELLH * 0.5 + row * CELLH;
  filledCircleRGBA(renderer, center_x, center_y, hatzi_box, color->r, color->g,
                   color->b, 255);

  filledCircleRGBA(renderer, center_x, center_y, hatzi_box - 12, 0, 0, 0, 255);
}
