# First target (parsum) is the one automatically run
parsum: main.o
	g++ -std=c++11 -pthread -Wall -g -o parsum main.o

main.o: main.cpp
	g++ -std=c++11 -g -c -pthread main.cpp
