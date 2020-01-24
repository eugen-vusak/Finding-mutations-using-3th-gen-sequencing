CC		:= g++
C_FLAGS := -std=c++17 -Wall -Wextra -g -O3

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
MAKE_BIN := if not exist $(BIN) mkdir $(BIN)
else
EXECUTABLE	:= main
MAKE_BIN :=  mkdir -p $(BIN)
endif

all: dir $(BIN)/$(EXECUTABLE)

dir: 
	$(MAKE_BIN)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/*/*.cpp modules/cJSON/cJSON*.c
	$(CC) $(C_FLAGS) -I$(INCLUDE) -Imodules/cJSON -L$(LIB) $^ -o $@ $(LIBRARIES)