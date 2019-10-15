#ifndef IMAGE_H
#define IMAGE_H

typedef unsigned char UInt8;

static const int TGA_HEADER_SIZE = 18;

class Image
{
 public:
	 Image(int width = 1, int height = 1, int channels = 4);
	 Image(int width, int height, int channels, UInt8* data);
	 Image(const Image& image);
	 ~Image();

	 void operator=(const Image& image);

	 void loadFromFile(const char *filePath);
	 void saveAsFile(const char *filePath) const ;

	 void flipHorizontal() const; //flip left and right
	 void flipVertical() const;	//flip up and down
	 void resize(int width, int height);
	 void reset() const { memset(data_, 0, data_size()); };

	 UInt8 *get_pixel(int row, int col) const
	 {
		 int index = row * width_ * channels_ + col * channels_;
		 return &(data_[index]);
	 }

	int width() const { return width_; }
	int height() const { return height_; }
	int channels() const { return channels_; }
	int data_size() const { return width_ * height_*channels_; }
	UInt8 * data() const { return data_; } //const ptr, none-const data

	//void set_width(int width) { width_ = width; }
	//void set_height(int height) { height_ = height; }
	//void set_channels(int channels) { channels_ = channels; }
	void set_data(UInt8 * data) { delete[] data_; data_ = data; }


private:
	void loadFromTGA(const char *filePath);

	int width_;
	int height_;
	int channels_;
	UInt8 *data_;
};

#endif
