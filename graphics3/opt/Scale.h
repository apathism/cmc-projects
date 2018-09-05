#ifndef __GRAPHICS_SCALE__
#define __GRAPHICS_SCALE__

#include "Transformation.h"

class Scale: public Transformation
{
	Scale(const Scale&);
public:
	Scale(float, float, float);
};

#endif
