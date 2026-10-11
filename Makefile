CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -pedantic -O2
TARGET := library_app
SOURCE := src/main.cpp

.PHONY: all run clean test
all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

test:
	$(CXX) $(CXXFLAGS) tests/test_library.cpp -o test_library
	./test_library

clean:
	rm -f $(TARGET) test_library
