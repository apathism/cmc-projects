#ifndef __GRAPHICS_ROTATE__
#define __GRAPHICS_ROTATE__

#include "Transformation.h"

class Rotate: public Transformation
{
	Rotate(const Rotate&);
public:
	Rotate(float, float, float, float);
};

#endif
