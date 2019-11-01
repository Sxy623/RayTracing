main: main.o 
	g++ -o main main.o
main.o: main.cpp ray.h
	g++ -c main.cpp
clean:
	rm -rf *.o