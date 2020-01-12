#include "image.h"
#include <assert.h>
#include <algorithm>
#include "utils.h"

Image::Image(int width, int height, int channels)
{
	assert(width > 0 && height > 0 && channels >= 1 && channels <= 4);
	width_ = width;
	height_ = height;
	channels_ = channels;

	int data_size = width * height * channels;
	data_ = new Byte[data_size];
	memset(data_, 0, data_size);
}

Image::Image(int width, int height, int channels, Byte* data)
{
	assert(width > 0 && height > 0 && channels >= 1 && channels <= 4 && data != NULL);
	width_ = width;
	height_ = height;
	channels_ = channels;
	data_ = data;
}

Image::Image(const Image& image)
{
	width_ = image.width_;
	height_ = image.height_;
	channels_ = image.channels_;

	int data_size = width_ * height_ * channels_;
	data_ = new Byte[data_size];
	memcpy(data_, image.data_, data_size);
}

Image::~Image()
{
	delete[] data_;
}

Image& Image::operator=(const Image& image)
{
	width_ = image.width_;
	height_ = image.height_;
	channels_ = image.channels_;

	delete[] data_; //release resourcePtr before change it

	int data_size = width_ * height_ * channels_;
	data_ = new Byte[data_size];
	memcpy(data_, image.data_, data_size);

	return *this;
}

Byte* Image::GetPixel(int x, int y) const
{
	int index = y * width_ * channels_ + x * channels_;
	return &(data_[index]);
}

/********************************
*  image load/save 
*********************************/

/*
*  tga format
*/
void Image::LoadFromTGA(const char *filePath)
{
	Byte header[TGA_HEADER_SIZE];
	int width, height, depth, channels;
	int idlength, imgtype, imgdesc;
	FILE *file;

	file = fopen(filePath, "rb");
	assert(file != NULL);
	ReadBytes(file, header, TGA_HEADER_SIZE);

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
		ReadBytes(file, loadIMG.data(), loadIMG.data_size());
	}
	else if (imgtype == 10 || imgtype == 11) {  /* run-length encoded */
		LoadTGA(file, &loadIMG);
	}
	else {
		assert(0);
	}
	fclose(file);

	imgdesc = header[17];
	if (imgdesc & 0x20) {
		loadIMG.FlipVertical();
	}
	if (imgdesc & 0x10) {
		loadIMG.FlipHorizontal();
	}
	
	(*this) = loadIMG;
}

void Image::LoadFromFile(const char *filePath)
{
	const char *ext = GetExtension(filePath);
	if (strcmp(ext, "tga") == 0) {
		LoadFromTGA(filePath);
	}
	else {
		assert(0);
	}
}

void Image::SaveAsFile(const char *filePath) const
{
	const char *ext = GetExtension(filePath);
	if (strcmp(ext, "tga") == 0) {
		SaveTGA(this, filePath);
	}
	else {
		assert(0);
	}
}


/* 
*  image processing 
*/
static inline void SwapByte(Byte *x, Byte *y)
{
	Byte t = *x;
	*x = *y;
	*y = t;
}

void Image::FlipHorizontal() const
{
	int half_width = width_ / 2;
	int row, col;
	for (row = 0; row < height_; row++) {
		for (col = 0; col < half_width; col++) {
			int flipped_col = width_ - col - 1;
			Byte *pixel1 = GetPixel(col, row);
			Byte *pixel2 = GetPixel(flipped_col, row);
			for (int k = 0; k < channels_; k++) {
				SwapByte(&pixel1[k], &pixel2[k]);
			}
		}
	}
}

void Image::FlipVertical() const 
{
	int half_height = height_ / 2;
	int row, col;
	for (row = 0; row < half_height; row++) {
		for (col = 0; col < width_; col++) {
			int flipped_row = height_ - row - 1;
			Byte *pixel1 = GetPixel(col, row);
			Byte *pixel2 = GetPixel(col, flipped_row);
			for (int k = 0; k < channels_; k++) {
				SwapByte(&pixel1[k], &pixel2[k]);
			}
		}
	}
}

void Image::Resize(int width, int height) 
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

			Byte *pixel_00 = GetPixel(src_c0, src_r0);
			Byte *pixel_01 = GetPixel(src_c1, src_r0);
			Byte *pixel_10 = GetPixel(src_c0, src_r1);
			Byte *pixel_11 = GetPixel(src_c1, src_r1);
			Byte *pixel = target.GetPixel(dst_col, dst_row);
			for (int k = 0; k < channels_; k++) {
				float v00 = pixel_00[k];  /* row 0, col 0 */
				float v01 = pixel_01[k];  /* row 0, col 1 */
				float v10 = pixel_10[k];  /* row 1, col 0 */
				float v11 = pixel_11[k];  /* row 1, col 1 */
				float v0 = Lerp(v00, v01, delta_c);  /* row 0 */
				float v1 = Lerp(v10, v11, delta_c);  /* row 1 */
				float value = Lerp(v0, v1, delta_r);
				pixel[k] = (Byte)(value + 0.5f);
			}
		}
	}
	
	(*this) = target;
}

void Image::Reset() const 
{ 
	memset(data_, 0, data_size()); 
};
