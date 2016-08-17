clean:
	rm -f *.o

# Compile and run
%.o: %.cpp
	clang++ -Wall -Wextra -pedantic -pedantic-errors -std=c++1z -o $@ $<

all: wifi.o

run: all
	./wifi.o
	
