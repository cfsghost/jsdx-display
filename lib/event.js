var Event = module.exports = function(disp) {
	var self = this;

	self.disp = disp;
	self.eventHandlers = {
		ScreenChange: []
	};
};

Event.prototype.init = function() {
	var self = this;

	self.disp._display.eventDispatch(function(e) {
		var args = Array.prototype.slice.call(arguments);
		self.eventHandlers[e].forEach(function(handler, index, handlers) {
			handler.apply(self.disp, args);
		});
	});
};

Event.prototype.add = function(eventName, handler) {
	var self = this;

	if (self.eventHandlers.hasOwnProperty(eventName)) {
		if (self.eventHandlers[eventName].indexOf(handler) == -1)
			self.eventHandlers[eventName].push(handler);
	}
};

Event.prototype.del = function(eventName, handler) {
	var self = this;

	if (self.eventHandlers.hasOwnProperty(eventName)) {
		var index = self.eventHandlers[eventName].indexOf(handler);
		if (index != -1)
			self.eventHandlers[eventName].splice(index, 1);
	}
};
