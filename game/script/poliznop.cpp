#include "poliznop.h"
#include "polizconst.h"

using namespace asl;
using namespace script;

PolizNop::PolizNop()
{
}

PolizNop::PolizNop(const PolizNop&)
{
}

Cloneable* PolizNop::Clone() const
{
	return new PolizNop();
}

int PolizNop::Evaluate(Array<String>&, PointerArray &stack, int pos) const
{
	stack.Push(PolizConst("NaN"));
	return pos+1;
}

String PolizNop::toString() const
{
	return "Nothing";
}
