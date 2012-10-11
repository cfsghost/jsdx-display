#ifndef JSDX_DISPLAY_H_
#define JSDX_DISPLAY_H_

#include <v8.h>

namespace JSDXDisplay {

#define JSDX_DISPLAY_DEFINE_CONSTANT(target, name, constant)					\
	(target)->Set(v8::String::NewSymbol(name),							\
	v8::Integer::New(constant),											\
	static_cast<v8::PropertyAttribute>(v8::ReadOnly|v8::DontDelete))

	struct NodeCallback {
		v8::Persistent<v8::Object> Holder;
		v8::Persistent<v8::Function> cb;

		~NodeCallback() {
			Holder.Dispose();
			cb.Dispose();
		}
	};

}

#endif
