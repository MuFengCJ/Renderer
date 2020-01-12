#include "utils.h"
#include <assert.h>
#include <algorithm>
#include "image.h"
#include "renderer.h"
#include "color.h"

using std::min;
using std::max;

Byte ReadByte(FILE *file)
{
	int byte = fgetc(file);
	assert(byte != EOF);
	return (unsigned char)byte;
}

void ReadBytes(FILE *file, void *buffer, int size)
{
	int count = (int)fread(buffer, 1, size, file);
	assert(count == size);
}

void WriteBytes(FILE *file, void *buffer, int size)
{
	int count = (int)fwrite(buffer, 1, size, file);
	assert(count == size);
}

void LoadTGA(FILE *file, Image *image)
{
	Byte *buffer = image->data();
	int channels = image->channels();
	int buffer_size = image->data_size();
	int elem_count = 0;
	while (elem_count < buffer_size) {
		Byte header = ReadByte(file);
		int rle_packet = header & 0x80;
		int pixel_count = (header & 0x7F) + 1;
		Byte pixel[4];
		int i, j;
		assert(elem_count + pixel_count * channels <= buffer_size);
		if (rle_packet) {  /* rle packet */
			for (j = 0; j < channels; j++) {
				pixel[j] = ReadByte(file);
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
					buffer[elem_count++] = ReadByte(file);
				}
			}
		}
	}
	assert(elem_count == buffer_size);
}

void SaveTGA(const Image *image, const char *filePath)
{
	Byte header[TGA_HEADER_SIZE];
	FILE *file;

	file = fopen(filePath, "wb");
	assert(file != NULL);

	memset(header, 0, TGA_HEADER_SIZE);
	header[2] = image->channels() == 1 ? 3 : 2;     /* image type */
	header[12] = image->width() & 0xFF;             /* width, lsb */
	header[13] = (image->width() >> 8) & 0xFF;      /* width, msb */
	header[14] = image->height() & 0xFF;            /* height, lsb */
	header[15] = (image->height() >> 8) & 0xFF;     /* height, msb */
	header[16] = (image->channels() * 8) & 0xFF;    /* image depth */
	WriteBytes(file, header, TGA_HEADER_SIZE);

	WriteBytes(file, image->data(), image->data_size());
	fclose(file);
}


/* hdr format */


/*
*  blit image data
*/
void blit_image_bgr(Image *src, int buffer_width, int buffer_height, Byte* buffer)
{
	int width = min(src->width(), buffer_width);
	int height = min(src->height(), buffer_height);
	int row, col;

	assert(width > 0 && height > 0);
	assert(src->channels() >= 1 && src->channels() <= 4);

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			int flipped_row = src->height() - 1 - row;
			Byte *src_pixel = src->GetPixel(col, flipped_row);
			int dst_pixel_index = row * buffer_width * 4 + col * 4;
			if (src->channels() == 3 || src->channels() == 4) {
				buffer[dst_pixel_index + 0] = src_pixel[0];  /* blue */
				buffer[dst_pixel_index + 1] = src_pixel[1];  /* green */
				buffer[dst_pixel_index + 2] = src_pixel[2];  /* red */
			}
			else {
				Byte gray = src_pixel[0];
				buffer[dst_pixel_index + 0] = buffer[dst_pixel_index + 1] = buffer[dst_pixel_index + 2] = gray;
			}
		}
	}
}

void blit_image_rgb(Image *src, int buffer_width, int buffer_height, Byte* buffer)
{
	int width = min(src->width(), buffer_width);
	int height = min(src->height(), buffer_height);
	int row, col;

	assert(width > 0 && height > 0);
	assert(src->channels() >= 1 && src->channels() <= 4);

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			int flipped_row = src->height() - 1 - row;
			Byte *src_pixel = src->GetPixel(col, flipped_row);
			int dst_pixel_index = row * buffer_width * 4 + col * 4;
			if (src->channels() == 3 || src->channels() == 4) {
				buffer[dst_pixel_index + 0] = src_pixel[2];  /* red */
				buffer[dst_pixel_index + 1] = src_pixel[1];  /* green */
				buffer[dst_pixel_index + 2] = src_pixel[0];  /* blue */
			}
			else {
				Byte gray = src_pixel[0];
				buffer[dst_pixel_index + 0] = buffer[dst_pixel_index + 1] = buffer[dst_pixel_index + 2] = gray;
			}
		}
	}
}

void blit_frame_bgr(FrameBuffer* src, int buffer_width, int buffer_height, Byte* buffer)
{
	int width = min(src->width(), buffer_width);
	int height = min(src->height(), buffer_height);
	int row, col;

	assert(width > 0 && height > 0);

	for (row = 0; row < height; row++) {
		for (col = 0; col < width; col++) {
			//window origin is topLeft while frame and image are default as bottomLeft
			int flipped_row = src->height() - 1 - row;
			Color src_pixel = src->GetPixel(col, flipped_row);
			int dst_pixel_index = row * buffer_width * 4 + col * 4;

			buffer[dst_pixel_index + 0] = src_pixel.b * 255;  /* blue */
			buffer[dst_pixel_index + 1] = src_pixel.g * 255;  /* green */
			buffer[dst_pixel_index + 2] = src_pixel.r * 255;  /* red */
		}
	}
}

const char *GetExtension(const char *filename)
{
	const char *dot_pos = strrchr(filename, '.');
	return dot_pos == NULL ? "" : dot_pos + 1;
}

float Lerp(float d0, float d1, float t)
{
	return d0 + (d1 - d0) * t;
}
