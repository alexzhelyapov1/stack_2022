all: debug

debug: clean main.o stack.o test.o
	g++ -g -fsanitize=address,leak -Wall main.o stack.o test.o
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

test.o: test.cpp
	g++ -Wall -c -o test.o test.cpp
