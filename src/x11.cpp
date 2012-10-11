#include <v8.h>
#include <node.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/extensions/Xrandr.h>

#include "jsdx_display.hpp"
#include "x11.hpp"

namespace JSDXDisplay {

	using namespace v8;
	using namespace node;

	extern NodeCallback *event_dispatch_cb;

	Display *display;
	XEvent x11event;

	/* Thread for X11 event dispatcher */
	static uv_async_t uv_x11event_notifier;
	static uv_check_t uv_x11event_checker;
	static uv_timer_t timer_handle;

	/* XRandr extension */
	int rr_major_version;
	int rr_minor_version;
	int xrandr_event_base;
	int xrandr_error_base;

	void X11::EventChecker(uv_check_t* handle, int status)
	{
		if (XPending(display)) {
			uv_async_send(&uv_x11event_notifier);
		}

		uv_timer_start(&timer_handle, timer_cb, 0, 1);
	}

	void X11::EventCallback(uv_async_t *handle, int status)
	{
		HandleScope scope;

		/* Handle all events */
		while(XPending(display)) {
			XNextEvent(display, &x11event);

			switch(x11event.type - xrandr_event_base) {
			case RRScreenChangeNotify:

				if (event_dispatch_cb) {

					TryCatch try_catch;
					/* Prepare arguments */
					Local<Value> argv[1] = {
						Local<Value>::New(String::New("ScreenChange"))
					};

					/* Call callback function */
					event_dispatch_cb->cb->Call(event_dispatch_cb->Holder, 1, argv);

					if (try_catch.HasCaught())
						FatalException(try_catch);
				}
				break;
			}
		}
	}

	void X11::timer_cb(uv_timer_t* handle, int status)
	{
	}

	void X11::Init()
	{
		display = XOpenDisplay(NULL);

		/* XRandr extension */
		XRRQueryVersion(display, &rr_major_version, &rr_minor_version);
		if (rr_major_version < 1 || (rr_major_version == 1 && rr_minor_version < 2)) {
			printf("RANDR extension is too old (must be at least 1.2)\n");
			return;
		}

		XRRSelectInput(display, DefaultRootWindow(display), RRScreenChangeNotifyMask);
		XRRQueryExtension(display, &xrandr_event_base, &xrandr_error_base);

		/* Listen to X Server */
		uv_check_init(uv_default_loop(), &uv_x11event_checker);
		uv_check_start(&uv_x11event_checker, EventChecker);
		uv_async_init(uv_default_loop(), &uv_x11event_notifier, EventCallback);
		uv_timer_init(uv_default_loop(), &timer_handle);
	}

	void X11::Uninit()
	{
		uv_check_stop((uv_check_t *)&uv_x11event_checker);
		uv_close((uv_handle_t *)&uv_x11event_checker, NULL);
		uv_close((uv_handle_t *)&uv_x11event_notifier, NULL);
	}
}
