#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

namespace utl {

	using namespace std;

	// Run something on the command line and return the output
	stringstream command(const string &c) {

		const string file = "/tmp/blah.txt";

		// Contruct full command
		system((c + " > " + file).c_str());

		// Read all of the buffer
		stringstream s;
		s << ifstream(file).rdbuf();

		return s;
	}
}

int main() {

	using namespace std;

	// AP scan
	stringstream iwlist = utl::command("iwlist wlp1s0 scan");

	string line;
	map<int, vector<string>> nodes;

	while (getline(iwlist, line)) {

		// Keys we're interested in
		const vector<string> keys = {"Address", "ESSID", "Encryption key"};

		// Search for the keys
		for (const auto &k : keys) {

			static int cell = 0;

			if (line.find(k) != string::npos) {

				if (line.find("Cell") != string::npos)
					++cell;

				nodes[cell].push_back(line);
				continue;
			}
		}
	}

	// Dump keys
	cout << "Number of APs " << nodes.size() << endl;
	for (const auto &n : nodes) {

		cout << n.first << endl;

		for (const auto &k : n.second)
			cout << "\t" << k << endl;
	}

	return 0;
}
