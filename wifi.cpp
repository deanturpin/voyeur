#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

int main() {
	using namespace std;

	// MIME type
	cout << "Content-Type: text/plain\n\n" << endl;

	// Currently connected AP
	{
		const string filename = "/tmp/blah.txt";
		system(string("iwgetid > " + filename).c_str());

		string ap;
		getline(ifstream(filename), ap);
		cout << ap << endl;

		cout << regex_match(ap, regex(".*ESSID.*")) << endl;
		cout << (ap.find("ESSID") != string::npos) << endl;
	}

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
		// "ip route",
		// "ping -w 1 8.8.8.8",
		"ip neighbour",
		// "nc -vz 0.0.0.0 1-20000 2>&1 | grep succeeded",
		// "iwlist wlp1s0 scan",
	};

	// And run them
	for (const auto &c : command) {

		// Construct full command
		const string file =  "/tmp/blah.txt";
		const string _c = c + " > " + file;

		// Call it
		system (_c.c_str());

		// Print the output
		cout << ifstream(file).rdbuf() << endl;
	}

	return 0;
}
