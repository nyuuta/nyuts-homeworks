CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
TARGET = program
SOURCE = read-file.cpp

all: $(TARGET)

$(TARGET): $(SOURCE:.cpp=.o)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o
