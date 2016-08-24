#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

namespace node {

	using namespace std;

	// A thing on the network
	class node {

		const string mac;
		const string essid;

		public:
			node(const string &m, const string &e)
				: mac(m)
				, essid(e)
			{}

			void print() const {
				cout << "\t" << mac << " " << essid << endl;
			}

	};

	// Run something on the command line and return the output
	string command(const string &c) {

		const string file = "/tmp/blah.txt";

		system((c + " > " + file).c_str());
		stringstream s;
		s << ifstream(file).rdbuf();

		return s.str();
	}
}

int main() {
	using namespace std;

	// Currently connected AP
	string ap = node::command("iwgetid");
	cout << "Current AP:" << ap << endl;

	// Interrogate self
	stringstream mac;
	mac << ifstream("/sys/class/net/wlp1s0/address").rdbuf();

	// Create container for all nodes
	vector<node::node> nodes;
	nodes.push_back(node::node(mac.str(), ap));

	cout << "Number of nodes " << nodes.size() << endl;
	for (const auto &n : nodes)
		n.print();

	// Vendor lookup
	stringstream oui;
	oui << ifstream("/usr/share/ieee-data/oui.txt").rdbuf();

	// AP scan
	const string iwlist = node::command("iwlist wlp1s0 scan");

	// Extraction of several sub-matches
	cmatch m;

	// Search for vendor
	if (regex_search(iwlist.c_str(), m, regex("Address: (.*)"))) {

		cmatch mcopy (m);
		const string mac = mcopy[1];

		cout << "mac: " << mac << endl;
		string vendor = "no vendor";

		if (regex_search(oui.str().c_str(), m, regex("XEROX(.*)"))) {

			cmatch mcopy2 (m);
			vendor = to_string(mcopy2.size());
		}

		cout << "vendor: " << vendor << endl;
	}

	/*
	vector<string> keys = {
		"ESSID",
		"Encryption key"
	};

	// Search for other keys
	for (const auto &k : keys)
		if (regex_search(iwlist.c_str(), m, regex(k + ":.*"))) {
			cout << *m.cbegin() << endl;
		}
	*/

	// http://192.168.0.10:1400/status/controllers
	// http://192.168.0.10:1400/status

	// Rescan (just the AP names)
	cout << "------------------------------" << endl;
	cout << node::command("iwlist wlp1s0 scan | grep ESSID") << endl;
	cout << "ping: " << node::command("ping -w 1 8.8.8.8 1> /dev/null && echo true || echo false") << endl;
	// cout << node::command("ip route") << endl;
	cout << node::command("ip neighbour") << endl;
	cout << node::command("curl github.com && echo github yes || echo github no") << endl;
	cout << node::command("nc -vz 0.0.0.0 1-20000 2>&1 | grep succeeded") << endl;
	cout << node::command("hcitool scan") << endl;

	return 0;
}
