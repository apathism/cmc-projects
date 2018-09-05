#include "polizconst.h"

using namespace asl;
using namespace script;

PolizConst::PolizConst(const PolizConst&)
{
}

PolizConst::PolizConst(const String &v)
{
	value = v;
}

String PolizConst::getValue(const Array<String>&) const
{
	return value;
}

int PolizConst::getAddr() const
{
	return -1;
}

Cloneable* PolizConst::Clone() const
{
	return new PolizConst(value);
}

int PolizConst::Evaluate(Array<String>&, asl::PointerArray &stack, int pos) const
{
	stack.Push(*this);
	return pos+1;
}

String PolizConst::toString() const
{
	return "Constant (" + value + ")";
}

PolizConst::~PolizConst()
{
}
