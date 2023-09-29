CC     = g++
SOURCE = main.cpp Board.cpp
OBJS   = main.o Board.o
HEADER = Board.h
OUT    = 15puzzle
FLAGS  = -g -Wall -std=c++11

# g++ -Wall -pedantic -std=c++11 main.cpp Board.cpp -o 15puzzle

all: $(OBJS)
	$(CC) $(FLAGS) -o $(OUT) $(SOURCE)

main.o: main.cpp Board.h
	$(CC) $(FLAGS) -c main.cpp

Board.o: Board.cpp Board.h
	$(CC) $(FLAGS) -c Board.cpp

clean:
	rm -f $(OBJS) $(OUT)