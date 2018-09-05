#ifndef __TRACER_COLOR__
#define __TRACER_COLOR__

struct Color
{
	double R, G, B;
	Color();
	Color(double, double, double);

	Color operator+(const Color&) const;
	Color operator*(const Color&) const;
	Color operator*(double) const;
	Color& operator+=(const Color&);
	Color& operator*=(const Color&);
};

#endif
