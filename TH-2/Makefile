CXX = g++
CXXFLAGS = -Wall -Werror
TARGET = main
SRC = main.cpp
OBJ = main.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(TARGET)
	
$(OBJ): $(SRC)
	$(CXX) $(CXXFLAGS) -c $(SRC) -o $(OBJ)
clean:
	rm -f $(OBJ) $(TARGET)
