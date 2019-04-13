CXX ?= g++

SRC = $(wildcard src/*.cc)
OBJ := $(addsuffix .o,$(basename $(SRC)))
INC = -I src/
BIN = trbt

export CPPFLAGS

CXXFLAGS := $(CXXFLAGS) -std=c++17 -Wall -Wextra -pedantic -Weffc++ $(INC) 

$(BIN): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean run clang gcc
clean:
	rm -f $(OBJ) $(BIN)

run: $(BIN)
	./$(BIN)

