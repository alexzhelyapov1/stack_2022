all: debug

debug: clean main.o stack.o
	g++ -Wall main.o stack.o
	./a.out

clean:
	touch a.out
	touch 1.o
	rm *.o
	rm a.out

main.o: main.cpp
	g++ -Wall -c -o main.o main.cpp

stack.o: stack.cpp
	g++ -Wall -c -o stack.o stack.cpp
