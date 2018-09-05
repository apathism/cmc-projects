#ifndef __SCRIPT_POLIZMARKER
#define __SCRIPT_POLIZMARKER

#include "polizitem.h"
#include "stackitem.h"

namespace script
{
	class PolizMarker: public PolizItem, public StackItem
	{
		PolizMarker(const PolizMarker&);
	public:
		PolizMarker();
		Cloneable* Clone() const;

		int getAddr() const;
		asl::String getValue(const asl::Array<asl::String>&) const;

		int Evaluate(
			asl::Array<asl::String>&,
			asl::PointerArray&, int
		) const;

		asl::String toString() const;
	};
}

#endif
