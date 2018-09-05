#ifndef __ASL_POINTERARRAY
#define __ASL_POINTERARRAY

#include "array.h"
#include "cloneable.h"

namespace asl
{
	class PointerArray
	{
		Array<Cloneable*> data;
	public:
		PointerArray(int = 0);
		virtual ~PointerArray();

		Cloneable& operator[] (int);
		const Cloneable& operator[] (int) const;

		int Length() const;

		void Resize(int);
		void Push(const Cloneable&);
		Cloneable* Pop();
	};
}

#endif

