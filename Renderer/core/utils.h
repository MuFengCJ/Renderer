#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "image.h"

/*
*  read/write file
*/
UInt8 read_byte(FILE *file);
void read_bytes(FILE *file, void *buffer, int size);
void write_bytes(FILE *file, void *buffer, int size);

/*
*  load/save file of certain format
*/
void load_tga(FILE *file, Image *image);
void save_tga(const Image *image, const char *filePath);


/*
*  blit image data
*/
void blit_image_bgr(Image *src, Image *dst);
void blit_image_rgb(Image *src, Image *dst);


/*
*  misc functions 
*/
inline const char *get_extension(const char *filename) 
{
	const char *dot_pos = strrchr(filename, '.');
	return dot_pos == NULL ? "" : dot_pos + 1;
}


/*
*  math functions
*/
inline double lerp(double d0, double d1, double t) 
{
	return d0 + (d1 - d0) * t;
}
#endif