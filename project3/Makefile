CXX = g++
CXXFLAGS = -std=c++11 -Wall

SRCS = xkcdpwgen.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = xkcdpwgen

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)