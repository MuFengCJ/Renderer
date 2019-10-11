#ifndef IMAGE_H
#define IMAGE_H

class Image
{
 public:
	int width;
	int height;
	int channels;
	unsigned char *data;
};

Image *image_create(int width, int height, int channels);
void image_release(Image *image);
Image *image_load(const char *filename);
void image_save(Image *image, const char *filename);

unsigned char *image_pixel_ptr(Image *image, int row, int col);
void image_blit_bgr(Image *src, Image *dst);
void image_blit_rgb(Image *src, Image *dst);
void image_flip_h(Image *image);
void image_flip_v(Image *image);
void image_resize(Image *image, int width, int height);

#endif
