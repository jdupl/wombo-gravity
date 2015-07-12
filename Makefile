CC=g++
SOURCE=src/main/cpp
CPP_FILES=$(wildcard $(SOURCE)/*.cpp)
OBJECTS=$(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

CFLAGS=-W -Wall -Ofast -march=native -std=c++11 -fexceptions -pipe -ffast-math
LD_FLAGS=

EXEC=build/wombo
DIRS=build obj

all: directories $(EXEC)

directories: ${DIRS}

${DIRS}:
	mkdir ${DIRS}

.PHONY: all directories clean

build/wombo: $(OBJECTS) lib/json/jsoncpp.o
	$(CC) $(LD_FLAGS) -o $@ $^

obj/%.o: $(SOURCE)/%.cpp lib/json/jsoncpp.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm $(EXEC) $(OBJECTS)
