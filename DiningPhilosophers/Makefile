CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
SRC = main.cpp
OBJ = main.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)
