var readline = require('readline');

const rl = readline.createInterface({ input: process.stdin });

// Read one line at a time
rl.on('line', (line) => {

	// Extract time
	// time = /[^ ]+/.exec(line)
	// if (time)
		// console.log(time.pop())

	// Extract end points
	endPoints = /([^ ]+) > ([^ ]+):/.exec(line)

	if (endPoints)
		console.log(endPoints[1] + "\t" + endPoints[2])
})
