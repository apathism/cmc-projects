#ifndef __GRAPHICS_IMAGELOADER__
#define __GRAPHICS_IMAGELOADER__

#include "Image.h"

class ImageLoader
{
	ImageLoader();
public:
	static bool Load(const char*, Image&);
};

#endif
