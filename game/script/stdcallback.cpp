#include <cstdlib>
#include <cstdio>
#include "stdcallback.h"
#include "stackitem.h"
#include "polizconst.h"
#include "runtimeexception.h"

using namespace asl;
using namespace script;

static const char *errWrongArgc = "wrong number of arguments";
static const char *errNoSuchFunction = "there is no such function available";

StdCallback::StdCallback()
{
	nextCallback = 0;
}

StdCallback::~StdCallback()
{
	delete nextCallback;
}

void StdCallback::Execute(const PolizItem &f, const String &fname, Array<String> &var, PointerArray &args, PointerArray &stack) const
{
	if (fname == "Print")
	{
		for (int j=args.Length()-1; j>=0; --j)
		{
			char *v = dynamic_cast<StackItem&>(args[j]).getValue(var).cstring();
			printf("%s",v);
			free(v);
		}
		stack.Push(PolizConst("1"));
		return;
	}
	if (fname == "PrintLine")
	{
		for (int j=args.Length()-1; j>=0; --j)
		{
			char *v = dynamic_cast<StackItem&>(args[j]).getValue(var).cstring();
			printf("%s",v);
			free(v);
		}
		printf("\n");
		stack.Push(PolizConst("1"));
		return;
	}
	if (fname == "ReadInt")
	{
		if (args.Length())
			throw RuntimeException(f,errWrongArgc);
		int v;
		scanf("%d",&v);
		stack.Push(PolizConst(String::fromInt(v)));
		return;
	}
	if (fname == "Random")
	{
		if (args.Length() != 1)
			throw RuntimeException(f,errWrongArgc);
		int v = dynamic_cast<StackItem&>(args[0]).getValue(var).toIntForce();
		if (!v) throw RuntimeException(f, "division by zero");
		v = rand()%v;
		stack.Push(PolizConst(String::fromInt(v)));
		return;
	}
	if (fname == "Min")
	{
		if (!args.Length())
			throw RuntimeException(f,errWrongArgc);
                int v = dynamic_cast<StackItem&>(args[0]).getValue(var).toIntForce();
		for (int j=1; j<args.Length(); ++j)
		{
			int vt = dynamic_cast<StackItem&>(args[j]).getValue(var).toIntForce();
			if (vt < v) v = vt;
		}
                stack.Push(PolizConst(String::fromInt(v)));
                return;
	}
        if (fname == "Max")
        {
                if (!args.Length())
                        throw RuntimeException(f,errWrongArgc);
                int v = dynamic_cast<StackItem&>(args[0]).getValue(var).toIntForce();
                for (int j=1; j<args.Length(); ++j)
                {
                        int vt = dynamic_cast<StackItem&>(args[j]).getValue(var).toIntForce();
                        if (vt > v) v = vt;
                }
                stack.Push(PolizConst(String::fromInt(v)));
                return;
        }
	if (!nextCallback) throw RuntimeException(f,errNoSuchFunction);
	nextCallback->Execute(f, fname, var, args, stack);
}

void StdCallback::setNextCallback(Callback *c)
{
	nextCallback = c;
}
