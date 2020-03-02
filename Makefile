all: global.o client.o CA.o main.o
	g++ main.o client.o CA.o global.o -o main -lgmpxx -lgmp
	rm *.o

main.o: main.cpp client.h CA.h
	g++ -c main.cpp

client.o: client.cpp client.h global.o
	g++ -c client.cpp

CA.o: CA.cpp CA.h global.o
	g++ -c CA.cpp

global.o: global.cpp global.h
	g++ -c global.cpp
