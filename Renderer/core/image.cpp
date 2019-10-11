#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "error.h"

/* 
*  helper functions 
*/
static const char *extract_extension(const char *filename) {
	const char *dot_pos = strrchr(filename, '.');
	return (dot_pos == NULL) ? "" : dot_pos + 1;
}

static unsigned char read_byte(FILE *file) {
	int byte = fgetc(file);
	FORCE(byte != EOF, "fgetc");
	return (unsigned char)byte;
}

#if 0
static void write_byte(FILE *file, unsigned char byte) {
	int status = fputc(byte, file);
	FORCE(status != EOF, "fputc");
}
#endif

static void read_bytes(FILE *file, void *buffer, int size) {
	int count = fread(buffer, 1, size, file);
	FORCE(count == size, "fread");
}

static void write_bytes(FILE *file, void *buffer, int size) {
	int count = fwrite(buffer, 1, size, file);
	FORCE(count == size, "fwrite");
}

static void swap_byte(unsigned char *x, unsigned char *y) {
	unsigned char t = *x;
	*x = *y;
	*y = t;
}

static int calc_data_size(Image *image) {
	return image->width * image->height * image->channels;
}

/* 
*  image creation/destruction 
*/
Image *image_create(int width, int height, int channels) {
	int data_size = width * height * channels;
	Image *image;

	FORCE(width > 0 && height > 0 && channels > 0, "image_create: size");

	image = (Image*)malloc(sizeof(Image));
	image->width = width;
	image->height = height;
	image->channels = channels;
	image->data = (unsigned char *)malloc(data_size);
	memset(image->data, 0, data_size);
	return image;
}

void image_release(Image *image) {
	free(image->data);
	free(image);
}

/* 
*  image input/output 
*/
static Image *load_tga(const char *filename);
static void save_tga(Image *image, const char *filename);

Image *image_load(const char *filename) {
	const char *ext = extract_extension(filename);
	if (strcmp(ext, "tga") == 0) {
		return load_tga(filename);
	}
	else {
		FATAL("image_load: format");
		return NULL;
	}
}

void image_save(Image *image, const char *filename) {
	const char *ext = extract_extension(filename);
	if (strcmp(ext, "tga") == 0) {
		save_tga(image, filename);
	}
	else {
		FATAL("image_save: format");
	}
}

static void load_tga_rle(FILE *file, Image *image) {
	int channels = image->channels;
	unsigned char *pixel = (unsigned char*)malloc(channels);
	unsigned char *buffer = image->data;
	int buffer_size = calc_data_size(image);
	int buffer_count = 0;
	while (buffer_count < buffer_size) {
		unsigned char header = read_byte(file);
		int is_rle_packet = header & 0x80;
		int pixel_count = (header & 0x7F) + 1;
		int expected_size = buffer_count + pixel_count * channels;
		FORCE(expected_size <= buffer_size, "load_tga_rle: packet size");
		if (is_rle_packet) {  /* run-length packet */
			int i, j;
			for (j = 0; j < channels; j++) {
				pixel[j] = read_byte(file);
			}
			for (i = 0; i < pixel_count; i++) {
				for (j = 0; j < channels; j++) {
					buffer[buffer_count++] = pixel[j];
				}
			}
		}
		else {              /* raw packet */
			int i, j;
			for (i = 0; i < pixel_count; i++) {
				for (j = 0; j < channels; j++) {
					buffer[buffer_count++] = read_byte(file);
				}
			}
		}
	}
	free(pixel);
}

#define TGA_HEADER_SIZE 18

static Image *load_tga(const char *filename) {
	FILE *file;
	unsigned char header[TGA_HEADER_SIZE];
	int width, height, depth, channels;
	int idlength, imgtype, imgdesc;
	Image *image;

	file = fopen(filename, "rb");
	FORCE(file != NULL, "fopen");
	read_bytes(file, header, TGA_HEADER_SIZE);

	width = header[12] + (header[13] << 8);
	height = header[14] + (header[15] << 8);
	FORCE(width > 0 && height > 0, "load_tga: width/height");
	depth = header[16];
	FORCE(depth == 8 || depth == 24 || depth == 32, "load_tga: depth");
	channels = depth / 8;
	image = image_create(width, height, channels);

	idlength = header[0];
	FORCE(idlength == 0, "load_tga: idlength");
	imgtype = header[2];
	if (imgtype == 2 || imgtype == 3) {           /* uncompressed */
		read_bytes(file, image->data, calc_data_size(image));
	}
	else if (imgtype == 10 || imgtype == 11) {  /* run-length encoded */
		load_tga_rle(file, image);
	}
	else {
		FATAL("load_tga: image type");
	}
	fclose(file);

	imgdesc = header[17];
	if (!(imgdesc & 0x20)) {
		image_flip_v(image);
	}
	if (imgdesc & 0x10) {
		image_flip_h(image);
	}
	return image;
}

static void save_tga(Image *image, const char *filename) {
	FILE *file;
	unsigned char header[TGA_HEADER_SIZE];

	file = fopen(filename, "wb");
	FORCE(file != NULL, "fopen");

	memset(header, 0, TGA_HEADER_SIZE);
	header[2] = (image->channels == 1) ? 3 : 2;  /* image type */
	header[12] = image->width & 0xFF;             /* width, lsb */
	header[13] = (image->width >> 8) & 0xFF;      /* width, msb */
	header[14] = image->height & 0xFF;            /* height, lsb */
	header[15] = (image->height >> 8) & 0xFF;     /* height, msb */
	header[16] = (image->channels * 8) & 0xFF;    /* image depth */
	header[17] = 0x20;                            /* top-left origin */
	write_bytes(file, header, TGA_HEADER_SIZE);

	write_bytes(file, image->data, calc_data_size(image));
	fclose(file);
}

/* 
*  image processing 
*/
unsigned char *image_pixel_ptr(Image *image, int row, int col) {
	int index = row * image->width * image->channels + col * image->channels;
	FORCE(row >= 0 && row < image->height, "image_pixel_ptr: row");
	FORCE(col >= 0 && col < image->width, "image_pixel_ptr: col");
	return &(image->data[index]);
}

static void blit_truecolor(Image *src, Image *dst, int swap_rb) {
	int r, c;

	if (src->channels != 1 && src->channels != 3 && src->channels != 4) {
		FATAL("blit_truecolor: src channels");
	}
	else if (dst->channels != 3 && dst->channels != 4) {
		FATAL("blit_truecolor: dst channels");
	}

	memset(dst->data, 0, calc_data_size(dst));
	for (r = 0; r < src->height && r < dst->height; r++) {
		for (c = 0; c < src->width && c < dst->width; c++) {
			unsigned char *src_pixel = image_pixel_ptr(src, r, c);
			unsigned char *dst_pixel = image_pixel_ptr(dst, r, c);
			if (src->channels == 1) {  /* gray */
				dst_pixel[0] = src_pixel[0];
				dst_pixel[1] = src_pixel[0];
				dst_pixel[2] = src_pixel[0];
			}
			else {
				if (swap_rb) {         /* rgb */
					dst_pixel[0] = src_pixel[2];
					dst_pixel[1] = src_pixel[1];
					dst_pixel[2] = src_pixel[0];
				}
				else {               /* bgr */
					dst_pixel[0] = src_pixel[0];
					dst_pixel[1] = src_pixel[1];
					dst_pixel[2] = src_pixel[2];
				}
			}
		}
	}
}

void image_blit_bgr(Image *src, Image *dst) {
	int swap_rb = 0;
	blit_truecolor(src, dst, swap_rb);
}

void image_blit_rgb(Image *src, Image *dst) {
	int swap_rb = 1;
	blit_truecolor(src, dst, swap_rb);
}

void image_flip_h(Image *image) {
	int half_width = image->width / 2;
	int r, c, k;
	for (r = 0; r < image->height; r++) {
		for (c = 0; c < half_width; c++) {
			int c2 = image->width - c - 1;
			unsigned char *pixel1 = image_pixel_ptr(image, r, c);
			unsigned char *pixel2 = image_pixel_ptr(image, r, c2);
			for (k = 0; k < image->channels; k++) {
				swap_byte(&pixel1[k], &pixel2[k]);
			}
		}
	}
}

void image_flip_v(Image *image) {
	int half_height = image->height / 2;
	int r, c, k;
	for (r = 0; r < half_height; r++) {
		for (c = 0; c < image->width; c++) {
			int r2 = image->height - r - 1;
			unsigned char *pixel1 = image_pixel_ptr(image, r, c);
			unsigned char *pixel2 = image_pixel_ptr(image, r2, c);
			for (k = 0; k < image->channels; k++) {
				swap_byte(&pixel1[k], &pixel2[k]);
			}
		}
	}
}

static double linear_interp(double v0, double v1, double d) {
	return v0 + (v1 - v0) * d;
}

static double bilinear_interp(double v00, double v01, double v10, double v11,
	double drow, double dcol) {
	double v0 = linear_interp(v00, v01, dcol);  /* row 0 */
	double v1 = linear_interp(v10, v11, dcol);  /* row 1 */
	return linear_interp(v0, v1, drow);
}

static int bound_index(int index, int length) {
	return (index > length - 1) ? length - 1 : index;
}

void image_resize(Image *image, int width, int height) {
	int channels = image->channels;
	Image src = *image;
	Image *dst = image;
	double scale_r, scale_c;
	int r, c, k;

	FORCE(width > 0 && height > 0, "image_resize: width/height");

	dst->width = width;
	dst->height = height;
	dst->data = (unsigned char*)malloc(width * height * channels);

	scale_r = (double)src.height / dst->height;
	scale_c = (double)src.width / dst->width;

	for (r = 0; r < dst->height; r++) {
		for (c = 0; c < dst->width; c++) {
			double mapped_r = r * scale_r;
			double mapped_c = c * scale_c;
			int src_r = (int)mapped_r;
			int src_c = (int)mapped_c;
			double drow = mapped_r - src_r;
			double dcol = mapped_c - src_c;
			int src_r_p1 = bound_index(src_r + 1, src.height);
			int src_c_p1 = bound_index(src_c + 1, src.width);

			unsigned char *pixel_00 = image_pixel_ptr(&src, src_r, src_c);
			unsigned char *pixel_01 = image_pixel_ptr(&src, src_r, src_c_p1);
			unsigned char *pixel_10 = image_pixel_ptr(&src, src_r_p1, src_c);
			unsigned char *pixel_11 = image_pixel_ptr(&src, src_r_p1, src_c_p1);

			unsigned char *pixel = image_pixel_ptr(dst, r, c);
			for (k = 0; k < channels; k++) {
				double v00 = pixel_00[k];
				double v01 = pixel_01[k];
				double v10 = pixel_10[k];
				double v11 = pixel_11[k];
				double value = bilinear_interp(v00, v01, v10, v11, drow, dcol);
				pixel[k] = (unsigned char)(value + 0.5);
			}
		}
	}
	free(src.data);
}
