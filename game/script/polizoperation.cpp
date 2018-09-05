#include <cstdio>
#include "polizoperation.h"
#include "polizconst.h"
#include "stackitem.h"
#include "asl/messageexception.h"

using namespace asl;
using namespace script;

PolizOperation::PolizOperation(const PolizOperation&)
{
}

PolizOperation::PolizOperation(const String &sign)
{
	this->sign = sign;
}

Cloneable* PolizOperation::Clone() const
{
	return new PolizOperation(sign);
}


int PolizOperation::Evaluate(Array<String>& v, PointerArray& s, int pos) const
{
	Cloneable *ui1 = s.Pop();
	StackItem *si1 = dynamic_cast<StackItem*>(ui1);
	if (sign == "-u")
		s.Push(PolizConst(String::fromInt(-si1->getValue(v).toIntForce())));
	else if (sign == "!u")
		s.Push(PolizConst(String::fromInt(!si1->getValue(v).toIntForce())));
	else
	{
		Cloneable *ui2 = s.Pop();
		StackItem *si2 = dynamic_cast<StackItem*>(ui2);
		if (sign == ":=")
		{
			int addr = si2->getAddr();
			if (0<=addr && addr<v.Length())
			{
				v[addr] = si1->getValue(v);
				s.Push(*si1);
			}
			else
				s.Push(PolizConst("NaN"));
		}
		else
		{
			String v1 = si1->getValue(v);
			String v2 = si2->getValue(v);
			if (sign == "=")
				s.Push(PolizConst((v1 == v2) ? "1" : "0"));
			else if (sign == "!=")
				s.Push(PolizConst((v1 != v2) ? "1" : "0"));
			else
			{
				int i2 = v1.toIntForce();
				int i1 = v2.toIntForce();
				if (sign == "+")
					s.Push(PolizConst(String::fromInt(i1+i2)));
				else if (sign == "-")
					s.Push(PolizConst(String::fromInt(i1-i2)));
				else if (sign == "*")
					s.Push(PolizConst(String::fromInt(i1*i2)));
				else if (sign == "/")
				{
					if (!i2)
						s.Push(PolizConst("NaN"));
					else
						s.Push(PolizConst(String::fromInt(i1/i2)));
				}
				else if (sign == "&")
					s.Push(PolizConst(String::fromInt(i1&i2)));
				else if (sign == "|")
					s.Push(PolizConst(String::fromInt(i1|i2)));
				else if (sign == "<")
					s.Push(PolizConst((i1 < i2) ? "1" : "0"));
				else if (sign == "<=")
					s.Push(PolizConst((i1 <= i2) ? "1" : "0"));
				else if (sign == ">")
					s.Push(PolizConst((i1 > i2) ? "1" : "0"));
				else if (sign == ">=")
					s.Push(PolizConst((i1 >= i2) ? "1" : "0"));
				else
					throw new MessageException("Internal error: unknown sign");
			}
		}
		delete ui2;
	}
	delete ui1;
	return pos+1;
}

String PolizOperation::toString() const
{
	return "Operation " + sign;
}
