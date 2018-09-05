#ifndef __ASL_EXCEPTION
#define __ASL_EXCEPTION

#include "string.h"

namespace asl
{
	class Exception
	{
	public:
		virtual String getMessage() const = 0;
	};
};

#endif
