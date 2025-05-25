CC = g++
CFLAGS = -Wall -Wextra -std=c++17 -pthread
LDFLAGS = -pthread

all: server client gui_client

server: main.cpp
	$(CC) $(CFLAGS) -o server main.cpp $(LDFLAGS)

client: client.cpp
	$(CC) $(CFLAGS) -o client client.cpp $(LDFLAGS)

gui_client:
	@echo "Building GUI client..."
	@cd gui_client && qmake && make
	@echo "GUI client built successfully"

clean:
	rm -f server client *.o
	@echo "Cleaning GUI client..."
	@cd gui_client && make clean || true
	@rm -f gui_client/gui_client || true

.PHONY: all clean gui_client
