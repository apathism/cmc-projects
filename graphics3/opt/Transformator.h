#ifndef __GRAPHICS_TRANSFORMATOR__
#define __GRAPHICS_TRANSOFRMATOR__

#include "Transformation.h"

#define Positioning Transformator __tr;__tr

class Transformator
{
	Transformator(const Transformator&);
public:
	Transformator();
	const Transformator& operator<< (const Transformation&) const;
	~Transformator();
};

#endif
