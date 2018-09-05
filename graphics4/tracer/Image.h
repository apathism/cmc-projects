#ifndef __TRACER_IMAGE__
#define __TRACER_IMAGE__

#include "Color.h"

class Image
{
	int width, height;
	Color *data;
	Image(const Image&);
public:
	Image(int, int);
	void setPixel(int, int, const Color&);
	const Color& getPixel(int, int) const;
	int Width() const;
	int Height() const;
	~Image();
	void makeBlack();
};

#endif
