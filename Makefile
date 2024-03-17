CFLAGS=-Wall -ggdb -std=c11 -pedantic `pkg-config --cflags sdl2`
LIBS=`pkg-config --libs sdl2`

tictactoe: tictactoe.c 
	$(CC) $(CFLAGS) -o tictactoe tictactoe.c $(LIBS)
