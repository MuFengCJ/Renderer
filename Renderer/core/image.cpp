#include <assert.h>
#include <algorithm>
#include "image.h"
#include "utils.h"

Image::Image(int width, int height, int channels) : width_(width), height_(height), channels_(channels)
{
	int data_size = width * height * channels;
	data_ = (unsigned char *)malloc(data_size);
	memset(data_, 0, data_size);
}
Image::Image(const Image& image)
{
	width_ = image.width_;
	height_ = image.height_;
	channels_ = image.channels_;

	int data_size = width_ * height_ * channels_;
	data_ = (unsigned char *)malloc(data_size);
	memcpy(data_, image.data_, data_size);
}
Image::~Image()
{
	free(data_);
}

void Image::operator=(const Image& image)
{
	width_ = image.width_;
	height_ = image.height_;
	channels_ = image.channels_;

	free(data_); //release resource before change it

	int data_size = width_ * height_ * channels_;
	data_ = (unsigned char *)malloc(data_size);
	memcpy(data_, image.data_, data_size);
}

/********************************
*  image load/save 
*********************************/

/*
*  tga format
*/
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
}

static inline void write_bytes(FILE *file, void *buffer, int size)
{
	int count = (int)fwrite(buffer, 1, size, file);
	assert(count == size);
}

static inline int get_buffer_size(Image *image)
{
	return image->width() * image->height() * image->channels();
}

static void load_tga_rle(FILE *file, Image *image)
{
	unsigned char *buffer = image->data();
	int channels = image->channels();
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

void save_tga(Image *image, const char *filename)
{
	unsigned char header[TGA_HEADER_SIZE];
	FILE *file;

	file = fopen(filename, "wb");
	assert(file != NULL);

	memset(header, 0, TGA_HEADER_SIZE);
	header[2] = image->channels() == 1 ? 3 : 2;     /* image type */
	header[12] = image->width() & 0xFF;             /* width, lsb */
	header[13] = (image->width() >> 8) & 0xFF;      /* width, msb */
	header[14] = image->height() & 0xFF;            /* height, lsb */
	header[15] = (image->height() >> 8) & 0xFF;     /* height, msb */
	header[16] = (image->channels() * 8) & 0xFF;    /* image depth */
	write_bytes(file, header, TGA_HEADER_SIZE);

	write_bytes(file, image->data(), get_buffer_size(image));
	fclose(file);
}

void Image::loadFromTGA(const char *filePath)
{
	unsigned char header[TGA_HEADER_SIZE];
	int width, height, depth, channels;
	int idlength, imgtype, imgdesc;
	FILE *file;

	file = fopen(filePath, "rb");
	assert(file != NULL);
	read_bytes(file, header, TGA_HEADER_SIZE);

	width = header[12] | (header[13] << 8);
	height = header[14] | (header[15] << 8);
	assert(width > 0 && height > 0);
	depth = header[16];
	assert(depth == 8 || depth == 24 || depth == 32);
	channels = depth / 8;
	Image loadIMG(width, height, channels);

	idlength = header[0];
	assert(idlength == 0);
	imgtype = header[2];
	if (imgtype == 2 || imgtype == 3) {           /* uncompressed */
		read_bytes(file, loadIMG.data(), get_buffer_size(&loadIMG));
	}
	else if (imgtype == 10 || imgtype == 11) {  /* run-length encoded */
		load_tga_rle(file, &loadIMG);
	}
	else {
		assert(0);
	}
	fclose(file);

	imgdesc = header[17];
	if (imgdesc & 0x20) {
		loadIMG.flipVertical();
	}
	if (imgdesc & 0x10) {
		loadIMG.flipHorizontal();
	}
	
	(*this) = loadIMG;
}
void Image::loadFromFile(const char *filePath)
{
	const char *ext = get_extension(filePath);
	if (strcmp(ext, "tga") == 0) {
		loadFromTGA(filePath);
	}
	else {
		assert(0);
	}
}

void Image::saveAsFile(const char *filePath) const
{
	const char *ext = get_extension(filePath);
	if (strcmp(ext, "tga") == 0) {
		//save_tga(image, filePath);
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

void Image::flipHorizontal() 
{
	int half_width = width_ / 2;
	int row, col;
	for (row = 0; row < height_; row++) {
		for (col = 0; col < half_width; col++) {
			int flipped_col = width_ - col - 1;
			unsigned char *pixel1 = get_pixel(row, col);
			unsigned char *pixel2 = get_pixel(row, flipped_col);
			for (int k = 0; k < channels_; k++) {
				swap_byte(&pixel1[k], &pixel2[k]);
			}
		}
	}
}

void Image::flipVertical() 
{
	int half_height = height_ / 2;
	int row, col;
	for (row = 0; row < half_height; row++) {
		for (col = 0; col < width_; col++) {
			int flipped_row = height_ - row - 1;
			unsigned char *pixel1 = get_pixel(row, col);
			unsigned char *pixel2 = get_pixel(flipped_row, col);
			for (int k = 0; k < channels_; k++) {
				swap_byte(&pixel1[k], &pixel2[k]);
			}
		}
	}
}

void Image::resize(int width, int height) 
{
	assert(width > 0 && height > 0);
	Image target(width, height, channels_);
	float scale_row = (float)height_ / (float)height;
	float scale_col = (float)width_ / (float)width;

	for (int dst_row = 0; dst_row < height; dst_row++) {
		for (int dst_col = 0; dst_col < width; dst_col++) {
			float mapped_r = (float)dst_row * scale_row;
			float mapped_c = (float)dst_col * scale_col;
			int src_r0 = (int)mapped_r;
			int src_c0 = (int)mapped_c;
			int src_r1 = std::min(src_r0 + 1, height_ - 1);
			int src_c1 = std::min(src_c0 + 1, width_ - 1);
			float delta_r = mapped_r - (float)src_r0;
			float delta_c = mapped_c - (float)src_c0;

			unsigned char *pixel_00 = get_pixel(src_r0, src_c0);
			unsigned char *pixel_01 = get_pixel(src_r0, src_c1);
			unsigned char *pixel_10 = get_pixel(src_r1, src_c0);
			unsigned char *pixel_11 = get_pixel(src_r1, src_c1);
			unsigned char *pixel = target.get_pixel(dst_row, dst_col);
			for (int k = 0; k < channels_; k++) {
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
	
	(*this) = target;
}
