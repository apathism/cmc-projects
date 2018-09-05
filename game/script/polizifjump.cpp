#include "polizifjump.h"
#include "stackitem.h"

using namespace asl;
using namespace script;

PolizIfJump::PolizIfJump(const PolizIfJump&)
{
}

PolizIfJump::PolizIfJump()
{
}

Cloneable* PolizIfJump::Clone() const
{
	return new PolizIfJump();
}

int PolizIfJump::Evaluate(Array<String> &v, PointerArray &s, int pos) const
{
	Cloneable* t1 = s.Pop();
	StackItem *t2 = dynamic_cast<StackItem*>(t1);
	int npos = t2->getValue(v).toIntForce();
	delete t1;

	t1 = s.Pop();
	t2 = dynamic_cast<StackItem*>(t1);
	String sval = t2->getValue(v);
	delete t1;

	int val;
	if (sval.toInt(val) && (!val))
		return npos;
	return pos+1;
}

String PolizIfJump::toString() const
{
	return "IfJump";
}
