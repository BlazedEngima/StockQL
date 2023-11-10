# Variables
CXX = g++
CXXFLAGS = -Wall -Isrc/include -std=c++17 -O3
SRC = $(wildcard src/*.cpp src/util/*.cpp)
TARGET = main.out

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

.PHONE: clean

# Clean
clean:
	rm -f $(OBJ) $(TARGET)

