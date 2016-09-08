.SILENT:

clean:
	rm -f *.o

# Compile and run
%.o: %.cpp
	echo Compile
	clang++ -Wall -Wextra -pedantic -pedantic-errors -std=c++1z -o $@ $<

all: wifi.o

run: all
	echo Run
	iwlist wlp1s0 scan | ./wifi.o
	# cat scan2.txt | ./wifi.o
