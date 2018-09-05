#ifndef __SCRIPT_RUNTIMEEXCEPTION
#define __SCRIPT_RUNTIMEEXCEPTION

#include "asl/messageexception.h"
#include "polizitem.h"

namespace script
{
	class RuntimeException: public asl::MessageException
	{
	public:
		RuntimeException(const PolizItem&, const asl::String&);
	};
}

#endif
