CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -pthread
LDFLAGS = -pthread

all: server client

server: main.cpp
	$(CC) $(CFLAGS) -o server main.cpp $(LDFLAGS)

client: client.cpp
	$(CC) $(CFLAGS) -o client client.cpp $(LDFLAGS)

clean:
	rm -f server client *.o

.PHONY: all clean
