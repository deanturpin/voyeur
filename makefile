clean:
	rm -f *.o

# Compile and run
%.o: %.cpp
	clang++ -Wall -Wextra -pedantic -pedantic-errors -std=c++1z -o $@ $<

all: wifi.o

run: all
	iwlist wlp1s0 scan | ./wifi.o # < scan1.txt
