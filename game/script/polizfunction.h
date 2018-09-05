#ifndef __SCRIPT_POLIZFUNCTION
#define __SCRIPT_POLIZFUNCTION

#include "polizitem.h"
#include "callback.h"

namespace script
{
	class PolizFunction: public PolizItem
	{
		static Callback *callback;

		asl::String fname;
		PolizFunction(const PolizFunction&);
	public:
		PolizFunction(const asl::String&);
		Cloneable* Clone() const;
		asl::String toString() const;

		int Evaluate(
			asl::Array<asl::String>&,
			asl::PointerArray&, int
		) const;

		static void RegisterCallback(Callback*);
		static void UnregisterCallbacks();
	};
}

#endif
