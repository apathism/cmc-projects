#ifndef __SCRIPT_POLIZCLEAN
#define __SCRIPT_POLIZCLEAN

#include "polizitem.h"

namespace script
{
	class PolizClean: public PolizItem
	{
		PolizClean(const PolizClean&);
	public:
		PolizClean();
		Cloneable* Clone() const;

		int Evaluate(
			asl::Array<asl::String>&,
			asl::PointerArray&, int
		) const;

		asl::String toString() const;
	};
}

#endif
