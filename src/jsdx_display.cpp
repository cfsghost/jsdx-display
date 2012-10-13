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

	static Handle<Value> GetOutputCount(const Arguments& args)
	{
		HandleScope scope;
		XRRScreenResources *res;

		/* Get current display */
		Display *disp = XOpenDisplay(NULL);

		res = XRRGetScreenResources(disp, DefaultRootWindow(disp));

		return scope.Close(Integer::New(res->noutput));
	}

	static Handle<Value> GetOutputs(const Arguments& args)
	{
		HandleScope scope;

		int i;
		XRRScreenResources *res;
		XRROutputInfo *output_info = NULL;
		XRRCrtcInfo *crtc_info = NULL;
		Local<Array> outputs = Array::New();
		Local<Object> o;

		/* Get current display */
		Display *disp = XOpenDisplay(NULL);

		res = XRRGetScreenResources(disp, DefaultRootWindow(disp));
		for (i = 0; i < res->noutput; ++i) {
			output_info = XRRGetOutputInfo(disp, res, res->outputs[i]);

			o = Object::New();
			o->Set(String::New("name"), String::New(output_info->name));

			/* Connected or not */
			switch(output_info->connection) {
			case RR_Connected:
				o->Set(String::New("connection"), String::New("connected"));
				break;
			case RR_Disconnected:
				o->Set(String::New("connection"), String::New("disconnected"));
				break;
			default:
				o->Set(String::New("connection"), String::New("unknown"));
			}

			/* Get offset */
			if (res->ncrtc > i) {
				crtc_info = XRRGetCrtcInfo(disp, res, res->crtcs[i]);

				o->Set(String::New("offset_x"), Integer::New(crtc_info->x));
				o->Set(String::New("offset_y"), Integer::New(crtc_info->y));
			}

			outputs->Set(i, o);
		}

		return scope.Close(outputs);
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
		NODE_SET_METHOD(target, "getOutputCount", GetOutputCount);
		NODE_SET_METHOD(target, "getOutputs", GetOutputs);
		NODE_SET_METHOD(target, "getScreenWidth", GetScreenWidth);
		NODE_SET_METHOD(target, "getScreenHeight", GetScreenHeight);
		NODE_SET_METHOD(target, "eventDispatch", EventDispatch);
	}

	NODE_MODULE(jsdx_display, init);
}
