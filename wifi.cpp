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
	map<int, map<string, string>> nodes;

	while (getline(iwlist, line)) {

		// Keys we're interested in
		const vector<string> keys = {"Address", "ESSID", "Encryption key"};

		// Search for the keys
		for (const auto &key : keys) {

			static int cell = 0;

			if (line.find(key) != string::npos) {

				if (line.find("Cell") != string::npos)
					++cell;

				// Store the key/value pair
				nodes[cell][key] = line;

				// Move on if a match has been found
				continue;
			}
		}
	}

	// Read vendor file
	stringstream oui;
	oui << ifstream("/usr/share/ieee-data/oui.txt").rdbuf();

	// Dump keys
	cout << "Number of APs " << nodes.size() << endl;
	for (const auto &node : nodes) {

		// Dump values
		cout << node.first << endl;

		// String to search
		const auto s = node.second.at("Address");

		// Matches
		smatch m;

		// Extract the MAC
		if (regex_search(s, m, regex("(([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2})")))
			nodes[node.first]["Vendor"] = *m.cbegin();

		// Dump all key value pairs
		for (const auto &value : node.second)
			cout << "\t" << value.second << endl;
	}

	return 0;
}
