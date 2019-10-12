#ifndef UTILS_H
#define UTILS_H

#include "image.h"

/*
*  load/save image of certain format
*/
Image *load_tga(const char *filename);
void save_tga(Image *image, const char *filename);
//texture_t *load_hdr(const char *filename);
//void save_hdr(texture_t *texture, const char *filename);


/*
*  blit image data
*/
void blit_image_bgr(Image *src, Image *dst);
void blit_image_rgb(Image *src, Image *dst);
//void blit_buffer_bgr(framebuffer_t *source, image_t *target);
//void blit_buffer_rgb(framebuffer_t *source, image_t *target);


/*
*  misc functions 
*/
inline const char *get_extension(const char *filename) 
{
	const char *dot_pos = strrchr(filename, '.');
	return dot_pos == NULL ? "" : dot_pos + 1;
}

inline unsigned char *get_pixel(Image *image, int row, int col) 
{
	int index = row * image->width * image->channels + col * image->channels;
	return &(image->data[index]);
}


/*
*  math functions
*/
inline double lerp(double d0, double d1, double t) 
{
	return d0 + (d1 - d0) * t;
}
#endif