#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

namespace node {

	using namespace std;

	class node {

		public:

		string MAC;
		string ESSID;
	};
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
	// string mac = ifstream("/sys/class/net/wlp1s0/address").rdbuf();
	self.MAC = "blah";
	self.ESSID = ap;

	// Create container for all nodes
	vector<node::node> nodes;
	nodes.push_back(self);

	cout << "Number of nodes " << nodes.size() << endl;

	// "ip route",
	// "ping -w 1 8.8.8.8",
	// "ip neighbour",
	// "nc -vz 0.0.0.0 1-20000 2>&1 | grep succeeded",
	// "iwlist wlp1s0 scan | grep ESSID",

	const string file = "/tmp/blah.txt";
	const string command = "iwlist wlp1s0 scan > " + file;

	// Get all visiable access points
	cout << "AP scan..." << endl;
	system(command.c_str());
	stringstream s;
	s << ifstream(file).rdbuf();

	// Extraction of several sub-matches
	cmatch m;

	auto _s = s.str();
	// string _s = "I am an ESSID...";
	regex_search(_s.c_str(), m, regex("ESSID:\"(.*)\""));

	cout << "Number of matches " << m.size() << endl;
	for (const auto &_m : m)
		cout << _m << endl;

	// cout << s.str() << endl;

	return 0;
}
