#ifndef __SCRIPT_POLIZITEM
#define __SCRIPT_POLIZITEM

#include "asl/array.h"
#include "asl/pointerarray.h"
#include "asl/string.h"
#include "asl/cloneable.h"

namespace script
{
	class PolizItem: public virtual asl::Cloneable
	{
	public:
		virtual int Evaluate(
			asl::Array<asl::String>&,
			asl::PointerArray&, int
		) const = 0;

		virtual asl::String toString() const = 0;
		virtual ~PolizItem();
	};
}

#endif
