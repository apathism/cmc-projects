#ifndef __GRAPHICS_COLOR__
#define __GRAPHICS_COLOR__

#define LocalColor Color __clr

class Color
{
	float color[4];
public:
	Color(float, float, float, float);
	~Color();
};

#endif
