#ifndef JSDX_JSDX_DISPLAY_X11_H_
#define JSDX_JSDX_DISPLAY_X11_H_

namespace JSDXDisplay {

	namespace X11 {

		void Init();
		void Uninit();
		void EventChecker(uv_check_t* handle, int status);
		void EventCallback(uv_async_t *handle, int status);
		void timer_cb(uv_timer_t* handle, int status);
	}
}

#endif
