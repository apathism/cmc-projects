#include "runtimeexception.h"

using namespace asl;
using namespace script;

RuntimeException::RuntimeException(const PolizItem &pi, const String &s):
	MessageException(
		"On the poliz item of type \"" + pi.toString() + "\": " + s
	)
{
}

