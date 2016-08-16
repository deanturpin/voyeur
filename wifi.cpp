#include <iostream>
#include <fstream>

int main() {
	using namespace std;

	ifstream i("/proc/interrupts");

	char c = i.get();

	while(i.good()) {
		cout << c;
		c = i.get();
	}

	return 0;
}
