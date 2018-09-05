#ifndef __SCRIPT_POLIZCONST
#define __SCRIPT_POLIZCONST

#include "stackitem.h"
#include "polizitem.h"

namespace script
{
	class PolizConst: public PolizItem, public StackItem
	{
		asl::String value;
		PolizConst(const PolizConst&);
	public:
		asl::Cloneable* Clone() const;

		int getAddr() const;
		asl::String getValue(const asl::Array<asl::String>&) const;
		PolizConst(const asl::String&);

		int Evaluate(
			asl::Array<asl::String>&,
			asl::PointerArray&, int
		) const;

		asl::String toString() const;
		virtual ~PolizConst();
	};
}

#endif
