BIN = ./bin/seaBattle
INCLUDE = ./include
LIB = ./lib
SRC = ./src/main.cpp
CC = g++
CFLAGS = 
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

$(BIN): 
	clear && $(CC) -I$(INCLUDE) -L$(LIB) $(SRC) -o $(BIN) $(LDFLAGS)

clean:
	rm -rf ./bin