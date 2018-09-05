#include "polizvar.h"
#include "polizmarker.h"
#include "runtimeexception.h"

using namespace asl;
using namespace script;

PolizVar::PolizVar(const PolizVar&)
{
}

PolizVar::PolizVar(int vid)
{
	this->vid = vid;
}

String PolizVar::getValue(const Array<String> &v) const
{
	return v[vid];
}

int PolizVar::getAddr() const
{
	return vid;
}

Cloneable* PolizVar::Clone() const
{
	return new PolizVar(vid);
}

int PolizVar::Evaluate(Array<String> &var, PointerArray &stack, int pos) const
{
	int l, vid = this->vid;
	while ((l = stack.Length()))
	{
		StackItem* v = dynamic_cast<StackItem*>(stack.Pop());
		if (dynamic_cast<PolizMarker*>(v))
		{
			delete v;
			break;
		}
		if (vid == -1)
		{
			delete v;
			continue;
		}
		Array<String> ps = var[vid].Split(',');
		int id = v->getValue(var).toIntForce();
		if (id < 0)
			throw RuntimeException(*this, "wrong array index");
		if (ps.Length() <= id)
		{
			var[vid] = "";
			for (int j=0; j<ps.Length(); ++j)
			{
				int nv;
				if (!ps[j].toInt(nv)) nv = -1;
				var[vid] = var[vid] + String::fromInt(nv) + ",";
			}
			for (int j=ps.Length(); j<=id; ++j)
			{
				var.Push(String());
				var[vid] = var[vid] + String::fromInt(var.Length()-1);
				if (j != id) var[vid] = var[vid] + ",";
			}
			vid = var.Length()-1;
		}
		else if (!ps[id].toInt(vid))
			vid = -1;
		delete v;
	}
	if (!l) throw RuntimeException(*this,
		"stack is empty, but marker was not found");
	stack.Push(PolizVar(vid));
	return pos+1;
}

String PolizVar::toString() const
{
	return "Variable (ID = " + String::fromInt(vid) + ")";
}
