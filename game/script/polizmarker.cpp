#include "polizmarker.h"
#include "runtimeexception.h"

using namespace asl;
using namespace script;

PolizMarker::PolizMarker()
{
}

PolizMarker::PolizMarker(const PolizMarker&)
{
}

Cloneable* PolizMarker::Clone() const
{
	return new PolizMarker();
}

int PolizMarker::getAddr() const
{
	throw RuntimeException(*this, "trying to get marker's address");
}


asl::String PolizMarker::getValue(const asl::Array<asl::String>&) const
{
	throw RuntimeException(*this, "trying to get marker's value");
}


int PolizMarker::Evaluate(Array<String>&, PointerArray& s, int pos) const
{
	s.Push(*this);
	return pos+1;
}

String PolizMarker::toString() const
{
	return "Marker";
}
