$(document).ready(function() {

	// Initialise an array of nodes
	var nodes = [];

	// Defaults
	var myIp = "";
	var root = "http://localhost"
	var wanAvailable = false;
	var activeColour = "lime";
	var essId = "Not connected";
	var ipAddr = "IP addr pending";
	var accessPoints = "Scanning access points...";

	// Render the page using information in the node array
	function renderPage() {

		// Clear the page
		$("body").empty();

		// Server stats
		$("body").append("<p>")
		$("body").append(new Date() + "<br>");
		$("body").append("servrip: <span style='color:" + activeColour + "'>" + myIp + "</span><br>");
		$("body").append("network: <span style='color:" + (wanAvailable ? activeColour : "red") + "'>WAN</span> <-> <span style='color:pink'>" + essId + "</span>");
		$("body").append("</p><hr>")

		// ARP table heading
		$("body").append("<p id='arp'>" 
				+ "<span>mac</span>"
				+ "<span>vendor</span>"
				+ "<span>ip</span>"
				+ "<span>ms</span>"
				+ "<span>open ports</span>"
				+ "</p>");

		// Sort nodes by vendor
		nodes.sort(function(a, b) {
			return a.vendor > b.vendor;
		});

		// ARP table
		$.each(nodes, function(index, node) {

			var active = node.responseTime == 0 ? "red" : activeColour;

			$("body").append("<p id='arp'>" 
					+ "<span style='color: yellow'>" + node.mac + "</span>"
					+ "<span style='color: orange'>" + node.vendor + "</span>"
					+ "<span style='color: " + active + "'>" + node.ip + "</span>"
					+ "<span style='color: cyan'>" + node.responseTime + "</span>"
					+ "<span style='color: magenta'>" + node.openPorts + "</span>"
					+ "</p>");
		});

		// IP addr
		$("body").append("<br><hr><pre>" + ipAddr + "</pre>");

		// Access points
		$("body").append("<br><hr><pre>" + accessPoints + "</pre>");

		// Favicon
		$("link#favicon").attr("href", wanAvailable ? "../icons/active.png" : "../icons/inactive.png");
	};

	// Take an ARP table as a parameter
	// Extract the MACs and IPs and populate the nodes array
	function processArp(table) {

		// Process one line at a time
		var lines = table.split("\n");

		$.each(lines, function(index, line) {

			// Create a new node
			var n = new Node("00:00:00:00:00:00", "0.0.0.0", "");

			// Look for a MAC
			var mac = /([a-fA-F0-9:]{17})/.exec(line);

			if (mac) {

				n.mac = mac.pop();

				// Look for an IP
				var ip = /^([0-9\.]{7,})/.exec(line);
				if (ip)
					n.ip = ip.pop();

				// Check node doesn't already exist
				var nodeExists = false;

				for (var i = 0; i < nodes.length; ++i) {
					if (nodes[i].mac === n.mac) {
						nodeExists = true;
						break;
					}
				}

				if (nodeExists == false) {

					// Store node
					nodes.push(n);

					// Request vendor and port scan
					$.ajax({ url: root + "/cgi-bin/mac?" + n.mac });
					$.ajax({ url: root + "/cgi-bin/portscan?" + n.ip });
				}
			}
		});
	};

	function processMyIp(ip) {
		myIp = ip;
	};

	// AJAX complete handler
	$(document).ajaxComplete(function(event, request, settings) {

		if (request.responseText == undefined)
			console.log("response is undefined, url " + settings.url);

		// If there's "ignore" anywhere in the URL don't do anything
		else if (settings.url.match("ignore")) {
			// Don't do anything with these pings
		}
			
		// Access point scan
		else if (settings.url.match("apscan"))
			accessPoints = request.responseText;

		// ARP
		else if (settings.url.match("howdy"))
			processArp(request.responseText);

		// My IP
		else if (settings.url.match("myip"))
			processMyIp(request.responseText);

		// ESSID
		else if (settings.url.match("iw")) {

			// Extract the AP
			var ap = /"(.*)"/.exec(request.responseText);

			// Set page title to current AP
			if (ap) {
				ap = ap.pop();

				essId = ap;
				$(document).prop("title", ap);
			}
		}

		// ip route
		else if (settings.url.match("ipaddr"))
			ipAddr = request.responseText;

		// Ping
		else if (settings.url.match("ping")) {

			// Is it internal or external?
			if (settings.url.match("external")) {

				// Check if it was a good one and update status
				wanAvailable = request.responseText.match("1 received");

			} else {

				// Extract IP
				var ip = /\?([0-9\.]{7,})/.exec(settings.url).pop();

				// Extract time
				var time = /time=(.*) ms/.exec(request.responseText);

				// Find the node and update response time
				for (var i = 0; i < nodes.length; ++i) {
					if (nodes[i].ip === ip) {
						nodes[i].responseTime = (time == null ? 0 : time.pop());
						break;
					}
				}
			}
		}

		// Vendor
		else if (settings.url.match("mac")) {

			// Extract MAC from request
			var mac = /[0-9a-zA-Z:]{17}/.exec(settings.url).pop();

			// Find the node and update vendor
			for (var i = 0; i < nodes.length; ++i) {

				if (nodes[i].mac === mac) {
					nodes[i].vendor = request.responseText.split(")").pop();
					break;
				}
			}
		}

		// Port scan
		else if (settings.url.match("portscan")) {

			// Split response into lines
			var lines = request.responseText.split("\n");

			// Extract IP
			var ip = /\?([0-9\.]{7,})/.exec(settings.url).pop();

			$.each(lines, function(index, line) {

				var port = /([0-9]+) port/.exec(line);

				if (port) {

					// Find the node and update response time
					for (var i = 0; i < nodes.length; ++i) {
						if (nodes[i].ip === ip) {
							nodes[i].openPorts += port.pop() + ",";
							break;
						}
					}
				}
			});
		}
	});

	// Ping a range of IPs on the subnet to help populate the ARP table
	function pingRange(ip) {

		// Extract the subnet
		var subnet = /[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\./.exec(ip);

		// Ping a range of hosts
		for (var host = 1; host < 31; ++host)
			$.ajax({ url: root + "/cgi-bin/ping?" + subnet + host + "&ignore", cache: false });
	}

	////////////////////////////////////////		
	// Entry point
	////////////////////////////////////////		

	// Render page to get started
	renderPage();

	// Things that happen quite a lot
	setInterval(function() {

		renderPage();

		// Request ARP table and server IP
		$.ajax({ url: root + "/cgi-bin/howdy", cache: false });

		// Ping each node
		$.each(nodes, function(index, node) {
			$.ajax({ url: root + "/cgi-bin/ping?" + node.ip, cache: false });
		});

		// Ping an external thing (for favicon updates)
		$.ajax({ url: root + "/cgi-bin/ping?8.8.8.8&external", cache: false });

		// Request current AP
		$.ajax({ url: root + "/cgi-bin/iw", cache: false });

	}, 5000);

	// Things that happen less often
	setInterval(function() {

		// Request current access point and scan others
		$.ajax({ url: root + "/cgi-bin/myip", cache: false });
		$.ajax({ url: root + "/cgi-bin/ipaddr", cache: false });
		$.ajax({ url: root + "/cgi-bin/apscan", cache: false });

		// if (myIp != "")
			// pingRange(myIp);

	}, 30000);
});
