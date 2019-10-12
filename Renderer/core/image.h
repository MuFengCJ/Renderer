#ifndef IMAGE_H
#define IMAGE_H

#define TGA_HEADER_SIZE 18

class Image
{
 public:
	 Image(int width = 1, int height = 1, int channels = 3);
	 Image(const Image& image);
	 ~Image();

	 void operator=(const Image& image);

	 void loadFromFile(const char *filePath);
	 void saveAsFile(const char *filePath) const ;

	 void flipHorizontal(); //flip left and right
	 void flipVertical();	//flip up and down
	 void resize(int width, int height);


	 unsigned char *get_pixel(int row, int col) const
	 {
		 int index = row * width_ * channels_ + col * channels_;
		 return &(data_[index]);
	 }

	 void set_pixel();

	int width() const { return width_; }
	int height() const { return height_; }
	int channels() const { return channels_; }
	unsigned char * data() const { return data_; }

	void set_width(int width) { width_ = width; }
	void set_height(int height) { height_ = height; }
	void set_channels(int channels) { channels_ = channels; }
	void set_data(unsigned char * data) { data_ = data; }


private:
	void loadFromTGA(const char *filePath);

	int width_;
	int height_;
	int channels_;
	unsigned char *data_;
};

#endif
