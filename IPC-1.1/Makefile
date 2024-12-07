CXX = g++
CXXFLAGS = -Wall -Werror

SRC1 = sender.cpp
OBJ1 = sender.o
SRC2 = receiver.cpp
OBJ2 = receiver.o
TARGET1 = sender
TARGET2 = receiver

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJ1)
	$(CXX) $(CXXFLAGS) $(OBJ1) -o $(TARGET1)

$(TARGET2): $(OBJ2)
	$(CXX) $(CXXFLAGS) $(OBJ2) -o $(TARGET2)

$(OBJ1): $(SRC1)
	$(CXX) $(CXXFLAGS) -c $(SRC1) -o $(OBJ1)

$(OBJ2): $(SRC2)
	$(CXX) $(CXXFLAGS) -c $(SRC2) -o $(OBJ2)

clean:
	rm -rf $(OBJ1) $(OBJ2) $(TARGET1) $(TARGET2)

