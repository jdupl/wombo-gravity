CC=g++
CFLAGS=-W -Wall -O3 -std=c++11 -fexceptions
LDFLAGS=
EXEC=wombo

all: $(EXEC)

wombo: body.o main.o
	$(CC) -o wombo body.o main.o $(LDFLAGS)

body.o: body.cpp
	$(CC) -o body.o -c body.cpp $(CFLAGS)

main.o: main.cpp body.hpp
	$(CC) -o main.o -c main.cpp $(CFLAGS)

clean:
	rm -rf *.o
	rm $(EXEC)

mrproper: clean
	rm -rf $(EXEC)
