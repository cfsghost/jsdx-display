var _display = require('../build/Release/jsdx_display');
var Event = require('./event');

var Display = module.exports = function() {
	var self = this;

	self._display = _display;
	self.event = new Event(self);

	_display.displayInit();
	self.event.init();
};

Display.prototype.uninit = function() {
	var self = this;

	self._display.displayUninit();
};

Display.prototype.getOutputCount = function() {
	return _display.getOutputCount();
};

Display.prototype.getOutputs = function() {
	return _display.getOutputs();
};

Display.prototype.getScreenWidth = function() {
	return _display.getScreenWidth();
};

Display.prototype.getScreenHeight = function() {
	return _display.getScreenHeight();
};

Display.prototype.on = function(eventName, callback) {
	this.event.add(eventName, callback);

	return this;
};

Display.prototype.off = function(eventName, callback) {
	this.event.del(eventName, callback);

	return this;
};
