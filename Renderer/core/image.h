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

void image_flip_h(Image *image);//flip left and right
void image_flip_v(Image *image);//flip up and down
Image* image_resize(Image *image, int width, int height);

#endif
