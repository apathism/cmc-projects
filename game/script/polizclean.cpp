#include "polizclean.h"

using namespace asl;
using namespace script;

PolizClean::PolizClean(const PolizClean&)
{
}

PolizClean::PolizClean()
{
}

Cloneable* PolizClean::Clone() const
{
	return new PolizClean();
}

int PolizClean::Evaluate(Array<String>&, PointerArray &s, int pos) const
{
	s.Pop();
	return pos+1;
}

String PolizClean::toString() const
{
	return "Clean";
}
