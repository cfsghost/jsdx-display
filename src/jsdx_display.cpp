#include <v8.h>
#include <node.h>
#include <X11/extensions/Xrandr.h>
#include <X11/Xlib.h>

#include "jsdx_display.hpp"

#if USE_X11
#include "x11.hpp"
#endif

namespace JSDXDisplay {
 
	using namespace node;
	using namespace v8;

	NodeCallback *event_dispatch_cb = NULL;

	static Handle<Value> DisplayInit(const Arguments& args)
	{
		HandleScope scope;

		X11::Init();

		return Undefined();
	}

	static Handle<Value> DisplayUninit(const Arguments& args)
	{
		HandleScope scope;

		X11::Uninit();

		return Undefined();
	}

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

	static Handle<Value> EventDispatch(const Arguments& args)
	{
		HandleScope scope;

		if (args[0]->IsFunction()) {
			if (event_dispatch_cb) {
				event_dispatch_cb->Holder.Dispose();
				event_dispatch_cb->cb.Dispose();
			} else {
				event_dispatch_cb = new NodeCallback();
			}

			/* Set function */
			event_dispatch_cb->Holder = Persistent<Object>::New(args.Holder());
			event_dispatch_cb->cb = Persistent<Function>::New(Handle<Function>::Cast(args[0]));
		}

		return args.This();
	}

	static void init(Handle<Object> target) {
		HandleScope scope;

		NODE_SET_METHOD(target, "displayInit", DisplayInit);
		NODE_SET_METHOD(target, "displayUninit", DisplayUninit);
		NODE_SET_METHOD(target, "getScreenWidth", GetScreenWidth);
		NODE_SET_METHOD(target, "getScreenHeight", GetScreenHeight);
		NODE_SET_METHOD(target, "eventDispatch", EventDispatch);
	}

	NODE_MODULE(jsdx_display, init);
}
