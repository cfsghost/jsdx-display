var _display = require('../build/Release/jsdx_display');

var Display = module.exports = function() {
};

Display.prototype.getScreenWidth = function() {
	return _display.getScreenWidth();
};

Display.prototype.getScreenHeight = function() {
	return _display.getScreenHeight();
};
