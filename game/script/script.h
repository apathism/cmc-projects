#ifndef __SCRIPT_SCRIPT
#define __SCRIPT_SCRIPT

#include "asl/string.h"
#include "callback.h"

namespace script
{
	class Script
	{
		int fd;
	public:
		Script(const asl::String&);
		void Execute();
		void RegisterCallback(Callback*);
	};
}

#endif
