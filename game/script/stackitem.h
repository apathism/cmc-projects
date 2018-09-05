#ifndef __SCRIPT_STACKITEM
#define __SCRIPT_STACKITEM

#include "asl/string.h"
#include "asl/array.h"
#include "asl/cloneable.h"

namespace script
{
	class StackItem: public virtual asl::Cloneable
	{
	public:
		virtual int getAddr() const = 0;
		virtual asl::String getValue(const asl::Array<asl::String>&) const = 0;
		virtual ~StackItem();
	};
}

#endif

