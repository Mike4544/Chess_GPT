CPP = g++
FILES-CPP = src/*.cpp
FILES-C = src/*.cpp
LIBS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
FLAGS = -Wall -Wextra -Winit-self -I includes -L libs/SDL2

release: 
	$(CPP) -o bin/release/chess-gpt $(FILES-CPP) -s -O3 $(FLAGS) $(LIBS)

debug: 
	$(CPP) -o bin/debug/chess-gpt-debug $(FILES-CPP) -g -DDEBUG $(FLAGS) $(LIBS)


