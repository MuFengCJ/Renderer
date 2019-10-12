#include <assert.h>
#include <algorithm>
#include "image.h"
#include "utils.h"

/* 
*  image create/destroy 
*/
Image *image_create(int width, int height, int channels) 
{
	int data_size = width * height * channels;
	Image *image;

	assert(width > 0 && height > 0 && channels >= 1 && channels <= 4);

	image = (Image*)malloc(sizeof(Image));
	image->width = width;
	image->height = height;
	image->channels = channels;
	image->data = (unsigned char *)malloc(data_size);
	memset(image->data, 0, data_size);
	return image;
}

void image_release(Image *image) 
{
	free(image->data);
	free(image);
}

/* 
*  image load/save 
*/
Image *image_load(const char *filename) 
{
	const char *ext = get_extension(filename);
	if (strcmp(ext, "tga") == 0) {
		return load_tga(filename);
	}
	else {
		assert(0);
		return NULL;
	}
}

void image_save(Image *image, const char *filename) 
{
	const char *ext = get_extension(filename);
	if (strcmp(ext, "tga") == 0) {
		save_tga(image, filename);
	}
	else {
		assert(0);
	}
}


/* 
*  image processing 
*/
static inline void swap_byte(unsigned char *x, unsigned char *y) 
{
	unsigned char t = *x;
	*x = *y;
	*y = t;
}

void image_flip_h(Image *image) 
{
	int half_width = image->width / 2;
	int row, col;
	for (row = 0; row < image->height; row++) {
		for (col = 0; col < half_width; col++) {
			int flipped_col = image->width - col - 1;
			unsigned char *pixel1 = get_pixel(image, row, col);
			unsigned char *pixel2 = get_pixel(image, row, flipped_col);
			for (int k = 0; k < image->channels; k++) {
				swap_byte(&pixel1[k], &pixel2[k]);
			}
		}
	}
}

void image_flip_v(Image *image) 
{
	int half_height = image->height / 2;
	int row, col;
	for (row = 0; row < half_height; row++) {
		for (col = 0; col < image->width; col++) {
			int flipped_row = image->height - row - 1;
			unsigned char *pixel1 = get_pixel(image, row, col);
			unsigned char *pixel2 = get_pixel(image, flipped_row, col);
			for (int k = 0; k < image->channels; k++) {
				swap_byte(&pixel1[k], &pixel2[k]);
			}
		}
	}
}

Image *image_resize(Image *source, int width, int height) 
{
	int channels = source->channels;
	Image *target;
	float scale_row, scale_col;
	int dst_row, dst_col;

	assert(width > 0 && height > 0);
	target = image_create(width, height, channels);
	scale_row = (float)source->height / (float)height;
	scale_col = (float)source->width / (float)width;
	for (dst_row = 0; dst_row < height; dst_row++) {
		for (dst_col = 0; dst_col < width; dst_col++) {
			float mapped_r = (float)dst_row * scale_row;
			float mapped_c = (float)dst_col * scale_col;
			int src_r0 = (int)mapped_r;
			int src_c0 = (int)mapped_c;
			int src_r1 = std::min(src_r0 + 1, source->height - 1);
			int src_c1 = std::min(src_c0 + 1, source->width - 1);
			float delta_r = mapped_r - (float)src_r0;
			float delta_c = mapped_c - (float)src_c0;

			unsigned char *pixel_00 = get_pixel(source, src_r0, src_c0);
			unsigned char *pixel_01 = get_pixel(source, src_r0, src_c1);
			unsigned char *pixel_10 = get_pixel(source, src_r1, src_c0);
			unsigned char *pixel_11 = get_pixel(source, src_r1, src_c1);
			unsigned char *pixel = get_pixel(target, dst_row, dst_col);
			for (int k = 0; k < channels; k++) {
				float v00 = pixel_00[k];  /* row 0, col 0 */
				float v01 = pixel_01[k];  /* row 0, col 1 */
				float v10 = pixel_10[k];  /* row 1, col 0 */
				float v11 = pixel_11[k];  /* row 1, col 1 */
				float v0 = lerp(v00, v01, delta_c);  /* row 0 */
				float v1 = lerp(v10, v11, delta_c);  /* row 1 */
				float value = lerp(v0, v1, delta_r);
				pixel[k] = (unsigned char)(value + 0.5f);
			}
		}
	}
	return target;
}
