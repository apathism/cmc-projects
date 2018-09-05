#ifndef __SCRIPT_POLIZOPERATION
#define __SCRIPT_POLIZOPERATION

#include "polizitem.h"

namespace script
{
	class PolizOperation: public PolizItem
	{
		asl::String sign;
		PolizOperation(const PolizOperation&);

	public:
		PolizOperation(const asl::String&);
		Cloneable* Clone() const;

		int Evaluate(
			asl::Array<asl::String>&,
			asl::PointerArray&, int
		) const;

		asl::String toString() const;
	};
}

#endif
