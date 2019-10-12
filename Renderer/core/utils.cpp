#include <assert.h>
#include <algorithm>
#include "utils.h"




/* hdr format */


/*
*  blit image data
*/
void blit_image_bgr(Image *src, Image *dst) 
{
	int width = std::min(src->width(), dst->width());
	int height = std::min(src->height(), dst->height());
	int row, col;

	assert(width > 0 && height > 0);
	assert(src->channels() >= 1 && src->channels() <= 4);
	assert(dst->channels() == 3 || dst->channels() == 4);

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			int flipped_row = src->height() - 1 - row;
			unsigned char *src_pixel = src->get_pixel(flipped_row, col);
			unsigned char *dst_pixel = dst->get_pixel(row, col);
			if (src->channels() == 3 || src->channels() == 4) {
				dst_pixel[0] = src_pixel[0];  /* blue */
				dst_pixel[1] = src_pixel[1];  /* green */
				dst_pixel[2] = src_pixel[2];  /* red */
			}
			else {
				unsigned char gray = src_pixel[0];
				dst_pixel[0] = dst_pixel[1] = dst_pixel[2] = gray;
			}
		}
	}
}

void blit_image_rgb(Image *src, Image *dst) 
{
	int width = std::min(src->width(), dst->width());
	int height = std::min(src->height(), dst->height());
	int row, col;

	assert(width > 0 && height > 0);
	assert(src->channels() >= 1 && src->channels() <= 4);
	assert(dst->channels() == 3 || dst->channels() == 4);

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			int flipped_row = src->height() - 1 - row;
			unsigned char *src_pixel = src->get_pixel(flipped_row, col);
			unsigned char *dst_pixel = dst->get_pixel(row, col);
			if (src->channels() == 3 || src->channels() == 4) {
				dst_pixel[0] = src_pixel[2];  /* red */
				dst_pixel[1] = src_pixel[1];  /* green */
				dst_pixel[2] = src_pixel[0];  /* blue */
			}
			else {
				unsigned char gray = src_pixel[0];
				dst_pixel[0] = dst_pixel[1] = dst_pixel[2] = gray;
			}
		}
	}
}
