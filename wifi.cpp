#include <iostream>
#include <fstream>
#include <vector>

int main() {
	using namespace std;

	// MIME type
	cout << "Content-Type: text/plain\n\n" << endl;

	// Define some interesting files
	vector<string> files = {
		"/sys/class/net/wlp1s0/address"
	};

	// And read them
	for (const auto &f : files) {

		cout << ifstream(f).rdbuf();
	}

	// Define some interesting commands
	vector<string> command = {
		"iwlist wlp1s0 scan",
		"date",
		"iwgetid",
		"ip route",
		"ping -w 1 8.8.8.8",
		"ip neighbour",
		"nc -vz 0.0.0.0 1-20000 2>&1 | grep succeeded",
	};

	// And run them
	for (const auto &c : command) {

		const string file =  "/tmp/blah.txt";
		string _c = c + " > " + file;
		system (_c.c_str());

		cout << ifstream(file).rdbuf();
		cout << endl;
	}

	return 0;
}
