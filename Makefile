# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -Isrc/include -std=c++17 -O3
SRC = $(wildcard src/*.cpp src/util/*.cpp)
TARGET = main.out

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONY: clean rmdb

# Clean
clean:
	rm $(OBJ) $(TARGET)
	rm data/db/*

rmdb:
	rm data/db/*

