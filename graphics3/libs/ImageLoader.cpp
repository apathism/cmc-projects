/* Koryabkin Ivan, 2011 */
#include "ImageLoader.h"

#include <cstdio>
#include <cstdlib>

const int bmp_size_fileheader = 14;
const int bmp_size_infoheader = 40;
const int bmp_color_bits = 24;

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

WORD& getWORD(void *data)
{
	return *((WORD*)(data));
}

DWORD& getDWORD(void *data)
{
	return *((DWORD*)(data));
}

bool readFileHeader(FILE* f, int &offset)
{
	BYTE header[bmp_size_fileheader];
	if (fseek(f, 0, SEEK_SET))
		return false;
	if (fread(header, bmp_size_fileheader, 1, f) != 1)
		return false;
	if (header[0] != 'B' || header[1] != 'M')
		return false;
	offset = getDWORD(header + 10);
	if (fread(header, 4, 1, f) != 1)
		return false;
	if (getDWORD(header) != 40)
		return false;
	return true;
}

#define ERRORC {fclose(f);return false;}
#define ASSERTC(x) if(!(x)) ERRORC

bool ImageLoader::Load(const char *filename, Image &img)
{
	// Just Do It!
	FILE* f = fopen(filename, "rb");
	BYTE buf[bmp_size_infoheader];
	if (!f) return false;
	int bitmap_pos;
	ASSERTC(readFileHeader(f, bitmap_pos));
	ASSERTC(!fseek(f, bmp_size_fileheader, SEEK_SET));
	ASSERTC(fread(buf, bmp_size_infoheader, 1, f) == 1);
	int x_res = getDWORD(buf+4);
	int y_res = getDWORD(buf+8);
	int n_bits = getWORD(buf+14);
	ASSERTC(n_bits == bmp_color_bits);
	ASSERTC(bitmap_pos == bmp_size_fileheader + bmp_size_infoheader);
	ASSERTC(!fseek(f, bitmap_pos, SEEK_SET));
	int rgb_size = 3 * x_res;
	int rest_4 = rgb_size % 4;
	if (rest_4) rgb_size += 4 - rest_4;
	img.width = x_res;
	img.height = y_res;
	img.data = new BYTE[x_res * y_res * 3];
	ASSERTC(img.data != NULL);
	BYTE rgb[rgb_size];
	for (int y=0; y < y_res; ++y)
	{
		ASSERTC(fread(rgb, rgb_size, 1, f) == 1);
		BYTE* line = img.data + rgb_size * y;
		BYTE* line_r = rgb;
		for (int x=0; x < x_res; ++x)
		{
			line[2] = line_r[0];
			line[1] = line_r[1];
			line[0] = line_r[2];
			line += 3;
			line_r += 3;
		}
	}
	fclose(f);
	return true;
}

