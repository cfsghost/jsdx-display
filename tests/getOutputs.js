var Display = require('../');

var display = new Display;

var outputs = display.getOutputs();

for (var index in outputs) {
	var output = outputs[index];

	console.log('[' + output.name + ']');
	console.log('connection: ' + output.connection);
	console.log('enabled: ' + output.enabled);

	if (output.enabled) {
		console.log('current mode: ');
		console.log(output.mode);
	}

	if (output.modes.length) {
		console.log('modes: ');
		for (var j in output.modes) {
			console.log('\t' + output.modes[j].name + 'x' + output.modes[j].refresh);
		}
	}
}

display.uninit();
