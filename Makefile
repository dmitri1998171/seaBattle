BIN = ./bin/seaBattle
INCLUDE = ./include
LIB = ./lib
SRC = ./src/main.cpp
EXT = ./src/UI.cpp ./src/Menu.cpp ./src/RectButton.cpp ./src/SpriteButton.cpp
CC = g++
CFLAGS = 
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

$(BIN): 
	clear && $(CC) -I$(INCLUDE) -L$(LIB) $(SRC) $(EXT) -o $(BIN) $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf ./bin