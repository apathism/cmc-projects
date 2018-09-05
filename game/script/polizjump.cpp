#include "polizjump.h"
#include "stackitem.h"

using namespace asl;
using namespace script;

PolizJump::PolizJump(const PolizJump&)
{
}

PolizJump::PolizJump()
{
}

Cloneable* PolizJump::Clone() const
{
	return new PolizJump();
}

int PolizJump::Evaluate(Array<String>& v, PointerArray& s, int pos) const
{
	Cloneable *t1 = s.Pop();
	StackItem *t2 = dynamic_cast<StackItem*>(t1);
	int npos = t2->getValue(v).toIntForce();
	delete t1;
	return npos;
}

String PolizJump::toString() const
{
	return "Jump";
}
