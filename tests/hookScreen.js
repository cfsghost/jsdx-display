var Display = require('../');

var display = new Display;

display.on('ScreenChange', function(e) {
	console.log('ScreenChange');
});
