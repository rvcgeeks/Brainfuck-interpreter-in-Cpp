all: brainfuck

brainfuck: bf++.cpp
	g++ bf++.cpp -std=gnu++11 -o bf++

clean:
	rm -f bf++
