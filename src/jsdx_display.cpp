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

	static Handle<Value> GetScreenWidth(const Arguments& args)
	{
		HandleScope scope;
		XWindowAttributes windowattr;

		/* Get current display and screen */
		Display *disp = XOpenDisplay(NULL);
		int screen = DefaultScreen(disp);
		XGetWindowAttributes(disp, RootWindow(disp, screen), &windowattr);

		return scope.Close(Integer::New(windowattr.width));
	}

	static Handle<Value> GetScreenHeight(const Arguments& args)
	{
		HandleScope scope;
		XWindowAttributes windowattr;

		/* Get current display and screen */
		Display *disp = XOpenDisplay(NULL);
		int screen = DefaultScreen(disp);
		XGetWindowAttributes(disp, RootWindow(disp, screen), &windowattr);

		return scope.Close(Integer::New(windowattr.height));
	}

	static void init(Handle<Object> target) {
		HandleScope scope;

		NODE_SET_METHOD(target, "getScreenWidth", GetScreenWidth);
		NODE_SET_METHOD(target, "getScreenHeight", GetScreenHeight);
	}

	NODE_MODULE(jsdx_display, init);
}
