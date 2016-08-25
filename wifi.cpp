#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

namespace node {

	using namespace std;

	// Run something on the command line and return the output
	stringstream command(const string &c) {

		const string file = "/tmp/blah.txt";

		system((c + " > " + file).c_str());
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
	stringstream iwlist = node::command("iwlist wlp1s0 scan");

	string line;
	map<string, vector<string>> nodes;

	while (getline(iwlist, line)) {

		// Keys we're interested in
		const vector<string> keys = {"Address", "ESSID"};

		static string essid = "default";

		// Search for the keys
		cmatch m;
		for (const auto &k : keys)
			if (regex_search(line.c_str(), m, regex(k + ".*"))) {

				const string t = *m.cbegin();

				if (t.find("Address"))
					essid = t;

				nodes[essid].push_back(t);
			}
	}

	// Dump keys
	for (const auto &n : nodes) {

		cout << endl << n.first << endl;
		for (const auto &k : n.second)
			cout << "\t" << k << endl;
	}

	// cout << (n.first.find("Address") != string::npos ? "\n" : "") << n.second << endl;

	// Check vendors

	// http://192.168.0.10:1400/status/controllers
	// http://192.168.0.10:1400/status

	// Rescan (just the AP names)
	// cout << "------------------------------" << endl;
	// cout << node::command("iwlist wlp1s0 scan | grep ESSID") << endl;
	// cout << "ping: " << node::command("ping -w 1 8.8.8.8 1> /dev/null && echo true || echo false") << endl;
	// cout << node::command("ip route") << endl;
	// cout << node::command("ip neighbour") << endl;
	// cout << node::command("curl github.com && echo github yes || echo github no") << endl;
	// cout << node::command("nc -vz 0.0.0.0 1-20000 2>&1 | grep succeeded") << endl;
	// cout << node::command("hcitool scan") << endl;

	return 0;
}
