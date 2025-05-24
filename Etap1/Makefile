COMPILER = g++
FLAGS = -std=c++17 -pthread -Wall -Wextra
EXECUTABLE = main
SRC = main.cpp

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC)
	$(COMPILER) $(FLAGS) -o $(EXECUTABLE) $(SRC)

clean:
	rm -f $(EXECUTABLE)

run: $(EXECUTABLE)
 #########replace 5  with desired number of threads#########
	./$(EXECUTABLE) 5      