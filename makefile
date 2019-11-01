main: main.o 
	g++ -o main main.o
main.o: main.cpp vec3.h
	g++ -c main.cpp
clean:
	rm -rf *.o