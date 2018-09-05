#ifndef __ASL_SYSCALLEXCEPTION
#define __ASL_SYSCALLEXCEPTION

#include "string.h"
#include "exception.h"

namespace asl
{
	class SyscallException: public Exception
	{
		int err;
		String syscall;
	public:
		SyscallException();
		SyscallException(const String&);
		String getSyscall() const;
		String getDescription() const;
		String getMessage() const;
	};
};

#endif
