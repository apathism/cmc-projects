#ifndef __SCRIPT_CALLBACK
#define __SCRIPT_CALLBACK

#include "asl/string.h"
#include "asl/pointerarray.h"
#include "polizitem.h"
#include "stackitem.h"

namespace script
{
	class Callback
	{
	public:
		virtual void Execute(const PolizItem&, const asl::String&,
			asl::Array<asl::String>&, asl::PointerArray&, asl::PointerArray&) const = 0;
		virtual void setNextCallback(Callback*) = 0;
		virtual ~Callback();
	};
}

#endif
