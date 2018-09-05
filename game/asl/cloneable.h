#ifndef __ASL_CLONEABLE
#define __ASL_CLONEABLE

namespace asl
{
	class Cloneable
	{
	public:
		virtual Cloneable* Clone() const = 0;
		virtual ~Cloneable();
	};
}

#endif

