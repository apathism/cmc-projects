#ifndef __SCRIPT_POLIZJUMP
#define __SCRIPT_POLIZJUMP

#include "polizitem.h"

namespace script
{
	class PolizJump: public PolizItem
	{
		PolizJump(const PolizJump&);
	public:
		PolizJump();
		Cloneable* Clone() const;

		int Evaluate(
			asl::Array<asl::String>&,
			asl::PointerArray&, int
		) const;

		asl::String toString() const;
	};
}

#endif
