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

	// Interrogate self
	// stringstream mac;
	// mac << ifstream("/sys/class/net/wlp1s0/address").rdbuf();

	// Vendor lookup
	// stringstream oui;
	// oui << ifstream("/usr/share/ieee-data/oui.txt").rdbuf();

	// AP scan
	stringstream iwlist = utl::command("iwlist wlp1s0 scan");

	string line;
	map<string, vector<string>> nodes;

	while (getline(iwlist, line)) {

		// Keys we're interested in
		const vector<string> keys = {"Address", "ESSID"};

		static string essid = "default";
		// cout << "--- " << line << endl;

		// Search for the keys
		for (const auto &k : keys) {

			if (line.find(k) != string::npos) {
				cout << "+++" << line << endl;
				continue;
			}

			nodes[essid].push_back(line);
		}
	}

	// Dump keys
	cout << "Number of APs " << nodes.size() << endl;
	/*
	for (const auto &n : nodes) {

		cout << endl << n.first << endl;

		for (const auto &k : n.second)
			cout << "\t" << k << endl;
	}
	*/

	// cout << (n.first.find("Address") != string::npos ? "\n" : "") << n.second << endl;

	// Check vendors

	// http://192.168.0.10:1400/status/controllers
	// http://192.168.0.10:1400/status

	// Rescan (just the AP names)
	// cout << "------------------------------" << endl;
	// cout << utl::command("iwlist wlp1s0 scan | grep ESSID") << endl;
	// cout << "ping: " << utl::command("ping -w 1 8.8.8.8 1> /dev/null && echo true || echo false") << endl;
	// cout << utl::command("ip route") << endl;
	// cout << utl::command("ip neighbour") << endl;
	// cout << utl::command("curl github.com && echo github yes || echo github no") << endl;
	// cout << utl::command("nc -vz 0.0.0.0 1-20000 2>&1 | grep succeeded") << endl;
	// cout << utl::command("hcitool scan") << endl;

	return 0;
}
