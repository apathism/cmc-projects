#include "polizfunction.h"
#include "runtimeexception.h"
#include "polizmarker.h"
#include <cstdio>

using namespace asl;
using namespace script;

Callback* PolizFunction::callback = 0;

PolizFunction::PolizFunction(const String &fname)
{
	this->fname = fname;
}

PolizFunction::PolizFunction(const PolizFunction&)
{
}

Cloneable* PolizFunction::Clone() const
{
	return new PolizFunction(fname);
}

int PolizFunction::Evaluate(Array<String> &var, PointerArray &stack, int pos) const
{
	int l;
	PointerArray args;
	while ((l = stack.Length()))
	{
		Cloneable *t = stack.Pop();
		if (dynamic_cast<PolizMarker*>(t))
		{
			delete t;
			break;
		}
		args.Push(*t);
		delete t;
	}
	if (!l)
		throw RuntimeException(*this,
			"stack is empty but marker wasn't found");
	if (!callback)
		throw RuntimeException(*this,
			"callback wasn't registered");
	callback->Execute(*this, fname, var, args, stack);
	return pos+1;
}

String PolizFunction::toString() const
{
	return "Function (" + fname + ")";
}

void PolizFunction::RegisterCallback(Callback *c)
{
	Callback *temp = callback;
	callback = c;
	c->setNextCallback(temp);
}

void PolizFunction::UnregisterCallbacks()
{
	delete callback;
}
