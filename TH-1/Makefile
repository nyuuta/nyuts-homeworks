CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
SRC = array-summary.cpp
OBJ = array-summary.o
TARGET = array-summary

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)

