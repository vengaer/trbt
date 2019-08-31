CXX ?= g++

SRC = $(wildcard tests/*.cc)
OBJ := $(addsuffix .o,$(basename $(SRC)))
INC = -I include/
BIN = trbt

export CPPFLAGS

CXXFLAGS := $(CXXFLAGS) -std=c++17 -Wall -Wextra -pedantic -Weffc++ $(INC) 

$(BIN): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

.PHONY: clean run locked
clean:
	rm -f $(OBJ) $(BIN)

run: $(BIN)
	./$(BIN)

locked: CPPFLAGS += -D TRBT_LOCK_ITERS
locked: $(BIN)
