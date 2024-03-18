CFLAGS=-Wall -ggdb -std=c11 -pedantic `pkg-config --cflags sdl2 SDL2_gfx` 
LIBS=`pkg-config sdl2 SDL2_gfx --libs`

tictactoe: tictactoe.c 
	$(CC) $(CFLAGS) -o tictactoe tictactoe.c $(LIBS)
