#ifndef __SCRIPT_STDCALLBACK
#define __SCRIPT_STDCALLBACK

#include "callback.h"

namespace script
{
	class StdCallback: public Callback
	{
		Callback* nextCallback;
	public:
		StdCallback();
		void Execute(const PolizItem&, const asl::String&, asl::Array<asl::String>&, asl::PointerArray&, asl::PointerArray&) const;
		void setNextCallback(Callback*);
		virtual ~StdCallback();
	};
}

#endif
