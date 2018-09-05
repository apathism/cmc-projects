#include "Image.h"
#include <cstring>

Image::Image(const Image&)
{
}

Image::Image(int x, int y)
{
	width = x;
	height = y;
	data = new Color[x*y];
}

Image::~Image()
{
	delete data;
}

void Image::setPixel(int x, int y, const Color &color)
{
	data[y * width + x] = color;
}

const Color& Image::getPixel(int x, int y) const
{
	return data[y * width + x];
}

int Image::Width() const
{
	return width;
}

int Image::Height() const
{
	return height;
}

void Image::makeBlack()
{
	memset(data, 0, sizeof(Color) * width * height);
}
