#include <assert.h>
#include <algorithm>
#include "utils.h"


/* 
*  tga format 
*/
#define TGA_HEADER_SIZE 18

static inline unsigned char read_byte(FILE *file) 
{
	int byte = fgetc(file);
	assert(byte != EOF);
	return (unsigned char)byte;
}

static inline void read_bytes(FILE *file, void *buffer, int size) 
{
	int count = (int)fread(buffer, 1, size, file);
	assert(count == size);
	//UNUSED_VAR(count);
}

static inline void write_bytes(FILE *file, void *buffer, int size) 
{
	int count = (int)fwrite(buffer, 1, size, file);
	assert(count == size);
	//UNUSED_VAR(count);
}

static inline int get_buffer_size(Image *image) 
{
	return image->width * image->height * image->channels;
}

static void load_tga_rle(FILE *file, Image *image) 
{
	unsigned char *buffer = image->data;
	int channels = image->channels;
	int buffer_size = get_buffer_size(image);
	int elem_count = 0;
	while (elem_count < buffer_size) {
		unsigned char header = read_byte(file);
		int rle_packet = header & 0x80;
		int pixel_count = (header & 0x7F) + 1;
		unsigned char pixel[4];
		int i, j;
		assert(elem_count + pixel_count * channels <= buffer_size);
		if (rle_packet) {  /* rle packet */
			for (j = 0; j < channels; j++) {
				pixel[j] = read_byte(file);
			}
			for (i = 0; i < pixel_count; i++) {
				for (j = 0; j < channels; j++) {
					buffer[elem_count++] = pixel[j];
				}
			}
		}
		else {           /* raw packet */
			for (i = 0; i < pixel_count; i++) {
				for (j = 0; j < channels; j++) {
					buffer[elem_count++] = read_byte(file);
				}
			}
		}
	}
	assert(elem_count == buffer_size);
}

Image *load_tga(const char *filename) 
{
	unsigned char header[TGA_HEADER_SIZE];
	int width, height, depth, channels;
	int idlength, imgtype, imgdesc;
	Image *image;
	FILE *file;

	file = fopen(filename, "rb");
	assert(file != NULL);
	read_bytes(file, header, TGA_HEADER_SIZE);

	width = header[12] | (header[13] << 8);
	height = header[14] | (header[15] << 8);
	assert(width > 0 && height > 0);
	depth = header[16];
	assert(depth == 8 || depth == 24 || depth == 32);
	channels = depth / 8;
	image = image_create(width, height, channels);

	idlength = header[0];
	assert(idlength == 0);
	//UNUSED_VAR(idlength);
	imgtype = header[2];
	if (imgtype == 2 || imgtype == 3) {           /* uncompressed */
		read_bytes(file, image->data, get_buffer_size(image));
	}
	else if (imgtype == 10 || imgtype == 11) {  /* run-length encoded */
		load_tga_rle(file, image);
	}
	else {
		assert(0);
	}
	fclose(file);

	imgdesc = header[17];
	if (imgdesc & 0x20) {
		image_flip_v(image);
	}
	if (imgdesc & 0x10) {
		image_flip_h(image);
	}
	return image;
}

void save_tga(Image *image, const char *filename) 
{
	unsigned char header[TGA_HEADER_SIZE];
	FILE *file;

	file = fopen(filename, "wb");
	assert(file != NULL);

	memset(header, 0, TGA_HEADER_SIZE);
	header[2] = image->channels == 1 ? 3 : 2;     /* image type */
	header[12] = image->width & 0xFF;             /* width, lsb */
	header[13] = (image->width >> 8) & 0xFF;      /* width, msb */
	header[14] = image->height & 0xFF;            /* height, lsb */
	header[15] = (image->height >> 8) & 0xFF;     /* height, msb */
	header[16] = (image->channels * 8) & 0xFF;    /* image depth */
	write_bytes(file, header, TGA_HEADER_SIZE);

	write_bytes(file, image->data, get_buffer_size(image));
	fclose(file);
}

/* hdr format */


/*
*  blit image data
*/
void blit_image_bgr(Image *src, Image *dst) 
{
	int width = std::min(src->width, dst->width);
	int height = std::min(src->height, dst->height);
	int row, col;

	assert(width > 0 && height > 0);
	assert(src->channels >= 1 && src->channels <= 4);
	assert(dst->channels == 3 || dst->channels == 4);

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			int flipped_row = src->height - 1 - row;
			unsigned char *src_pixel = get_pixel(src, flipped_row, col);
			unsigned char *dst_pixel = get_pixel(dst, row, col);
			if (src->channels == 3 || src->channels == 4) {
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
	int width = std::min(src->width, dst->width);
	int height = std::min(src->height, dst->height);
	int row, col;

	assert(width > 0 && height > 0);
	assert(src->channels >= 1 && src->channels <= 4);
	assert(dst->channels == 3 || dst->channels == 4);

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			int flipped_row = src->height - 1 - row;
			unsigned char *src_pixel = get_pixel(src, flipped_row, col);
			unsigned char *dst_pixel = get_pixel(dst, row, col);
			if (src->channels == 3 || src->channels == 4) {
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
