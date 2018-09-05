#ifndef __ASL_MESSAGEEXCEPTION
#define __ASL_MESSAGEEXCEPTION

#include "string.h"
#include "exception.h"

namespace asl
{
	class MessageException: public Exception
	{
		String msg;
	public:
		MessageException(const String&);
		String getMessage() const;
	};
};

#endif
