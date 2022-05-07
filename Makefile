BIN = ./bin/seaBattle
INCLUDE = ./include
LIB = ./lib
SRC = ./src/*.cpp ./src/Menu/*.cpp
CC = g++
CFLAGS = -g
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

$(BIN): 
	clear && $(CC) -I$(INCLUDE) -L$(LIB) $(SRC) -o $(BIN) $(CFLAGS) $(LDFLAGS)

clean:
	rm -rf ./bin