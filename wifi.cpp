#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <regex>

int main() {

	using namespace std;

	// Container for all nodes
	map<int, map<string, string>> nodes;

	// Read lines from stdin
	string line;
	while (getline(cin, line)) {

		// Keys we're interested in
		const vector<string> keys = {"Address", "ESSID"};

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
	const auto vendors = oui.str();

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
		if (regex_search(s, m, regex("(([0-9a-fA-F]{2}:){2}[0-9a-fA-F]{2})"))) {

			// Swap colons for hyphens
			auto mac = m.str();

			replace(mac.begin(), mac.end(), ':', '-');

			// Search for vendor
			string vendor = "unknown";
			if (regex_search(vendors, m, regex(mac + ".*")))
				vendor = m.str();

			// Insert vendor
			nodes[node.first]["Vendor"] = vendor;
		}

		// Dump all key value pairs
		for (const auto &value : node.second)
			cout << '\t' << value.first << ": " << value.second << endl;
	}

	return 0;
}
