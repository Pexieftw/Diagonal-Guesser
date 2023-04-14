all:
	gcc -Isrc/Include -Lsrc/Lib -o Game main.c Header.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
	gcc -o Game main.c Header.c -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
	./Game