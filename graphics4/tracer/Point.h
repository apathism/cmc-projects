#ifndef __TRACER_POINT__
#define __TRACER_POINT__

struct Point
{
	double x, y, z;
	Point();
	Point(double, double, double);
	Point operator+(const Point&) const;
	Point operator-(const Point&) const;
	Point operator*(double) const;
	double ScalarMul(const Point&) const;
	Point VectorMul(const Point&) const;
	Point Rotate(const Point&, double angle) const;
	double Abs() const;
	void Normalize();
};

typedef Point Vector;

#endif
