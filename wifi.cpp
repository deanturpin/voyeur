#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

namespace node {

	using namespace std;

	class node {

		public:

		string mac;
		string essid;
	};

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

	// MIME type
	cout << "Content-Type: text/plain\n\n" << endl;

	// Currently connected AP
	const string filename = "/tmp/blah.txt";
	system(string("iwgetid > " + filename).c_str());

	string ap;
	getline(ifstream(filename), ap);
	cout << "Current AP " << ap << endl;

	// Interrogate self
	node::node self;
	stringstream mac;
	mac << ifstream("/sys/class/net/wlp1s0/address").rdbuf();
	self.mac = mac.str();;
	self.essid = ap;

	// Create container for all nodes
	vector<node::node> nodes;
	nodes.push_back(self);

	cout << "Number of nodes " << nodes.size() << endl;
	for (const auto &n : nodes)
		cout << "\t" << n.mac << " " << n.essid << endl;

	// "ip route",
	// "ping -w 1 8.8.8.8",
	// "ip neighbour",
	// "nc -vz 0.0.0.0 1-20000 2>&1 | grep succeeded",
	// "iwlist wlp1s0 scan | grep ESSID",

	cout << "Read OUI... " << endl;
	stringstream oui;
	oui << ifstream("/usr/share/ieee-data/oui.txt").rdbuf();
	cout << "done" << endl;

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

	vector<string> keys = {
		"ESSID",
		"Encryption key"
	};

	// Search for other keys
	for (const auto &k : keys)
		if (regex_search(iwlist.c_str(), m, regex(k + ":.*"))) {
			cout << *m.cbegin() << endl;
		}

	// Rescan (just the AP names)
	const auto rescan = node::command("iwlist wlp1s0 scan | grep ESSID");
	cout << rescan << endl;

	return 0;
}
