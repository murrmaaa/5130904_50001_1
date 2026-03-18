CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -I. -I./include
TARGET = main

SRCS = main.cpp rectangle.cpp isosceles_trapezoid.cpp composite_shape.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

