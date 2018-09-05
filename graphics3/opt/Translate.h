#ifndef __GRAPHICS_TRANSLATE__
#define __GRAPHICS_TRANSLATE__

#include "Transformation.h"

class Translate: public Transformation
{
	Translate(const Translate&);
public:
	Translate(float, float, float);
};

#endif
