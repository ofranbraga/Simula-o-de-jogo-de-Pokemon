
CXX = g++

CXXFLAGS = -std=c++11 -Wall -Isrc

SRCS = src/main.cpp src/pokemon.cpp


OBJS = $(SRCS:.cpp=.o)


TARGET = pokemon_simulation.exe


HEADER_DEPS = src/Pokemon.hpp


all: $(TARGET)


$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^


$(OBJS): %.o: %.cpp $(HEADER_DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	del $(subst /,\,$(OBJS)) $(subst /,\,$(TARGET)) 2>nul || (exit 0)

.PHONY: all clean
