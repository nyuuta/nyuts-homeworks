all: program

program: main.o complexNumber.o sort.o
	g++ main.o complexNumber.o sort.o -o program

main.o: main.cpp complexNumber.h sort.h
	g++ -c main.cpp -o main.o

complexNumber.o: complexNumber.cpp complexNumber.h
	g++ -c complexNumber.cpp -o complexNumber.o

sort.o: sort.cpp sort.h
	g++ -c sort.cpp -o sort.o

clean:
	rm -f *.o program
