#include <v8.h>
#include <node.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xlib.h>
#include <pthread.h>
#include <ev.h>

#include "jsdx_display.hpp"

namespace JSDXDisplay {
 
	using namespace node;
	using namespace v8;

	static void init(Handle<Object> target) {
		HandleScope scope;

//		NODE_SET_METHOD(target, "x11", ClutterInit);
//		ADCAWrapper::Initialize(target);
	}

	NODE_MODULE(jsdx_display, init);
}
