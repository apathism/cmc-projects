#ifndef __SCRIPT_POLIZVAR
#define __SCRIPT_POLIZVAR

#include "stackitem.h"
#include "polizitem.h"

namespace script
{
	class PolizVar: public PolizItem, public StackItem
	{
		int vid;
		PolizVar(const PolizVar&);
	public:
		Cloneable* Clone() const;
		int getAddr() const;
		asl::String getValue(const asl::Array<asl::String>&) const;
		PolizVar(int);
		int Evaluate(
			asl::Array<asl::String>&,
			asl::PointerArray&, int
		) const;

		asl::String toString() const;
	};
}

#endif
