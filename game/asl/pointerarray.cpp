#include <cstdio>
#include "pointerarray.h"

using namespace asl;

PointerArray::PointerArray(int m): data(m, 0)
{
	for (int j=0; j<m; ++j)
		data[j] = 0;
}

PointerArray::~PointerArray()
{
	for (int j=0; j<data.Length(); ++j)
		if (data[j]) delete data[j];
}

Cloneable& PointerArray::operator[] (int j)
{
	return *(data[j]);
}

const Cloneable& PointerArray::operator[] (int j) const
{
	return *(data[j]);
}

int PointerArray::Length() const
{
	return data.Length();
}

void PointerArray::Resize(int m)
{
	printf("%d\n",m);
	for (int j=m; j<Length(); j++)
		delete data[j];
	data.Resize(m, 0);
}

void PointerArray::Push(const Cloneable &x)
{
	data.Push(x.Clone());
}

Cloneable* PointerArray::Pop()
{
	Cloneable *temp = data[data.Length()-1];
	data.Pop();
	return temp;
}
