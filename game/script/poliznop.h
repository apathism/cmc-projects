#ifndef __SCRIPT_POLIZNOP
#define __SCRIPT_POLIZNOP

#include "polizitem.h"

namespace script
{
	class PolizNop: public PolizItem
	{
		PolizNop(const PolizNop&);
	public:
		PolizNop();
		Cloneable* Clone() const;

		int Evaluate(
			asl::Array<asl::String>&,
			asl::PointerArray&, int
		) const;

		asl::String toString() const;

	};
}

#endif
