#ifndef __SCRIPT_POLIZIFJUMP
#define __SCRIPT_POLIZIFJUMP

#include "polizitem.h"

namespace script
{
	class PolizIfJump: public PolizItem
	{
		PolizIfJump(const PolizIfJump&);
	public:
		PolizIfJump();
		Cloneable* Clone() const;

		int Evaluate(
			asl::Array<asl::String>&,
			asl::PointerArray&, int
		) const;

		asl::String toString() const;
	};
}

#endif
