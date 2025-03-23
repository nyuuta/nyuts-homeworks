TARGET = kill
CXX = g++
CXXFLAGS = -Wall -Werror
SRC = main.cpp
OBJ = main.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)

$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -rf $(OBJ) $(TARGET)
