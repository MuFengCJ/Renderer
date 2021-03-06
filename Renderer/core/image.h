#ifndef IMAGE_H
#define IMAGE_H

typedef unsigned char Byte;

static const int TGA_HEADER_SIZE = 18;

class Image
{
 public:
	 Image(int width = 1, int height = 1, int channels = 4);
	 Image(int width, int height, int channels, Byte* data);
	 Image(const Image& image);
	 ~Image();

	 Image& operator=(const Image& image);

	 void LoadFromFile(const char *filePath);
	 void SaveAsFile(const char *filePath) const ;

	 void FlipHorizontal() const; //flip left and right
	 void FlipVertical() const;	//flip up and down
	 void Resize(int width, int height);
	 void Reset() const;

	 Byte *GetPixel(int x, int y) const;

	int width() const { return width_; }
	int height() const { return height_; }
	int channels() const { return channels_; }
	int data_size() const { return width_ * height_*channels_; }
	Byte * data() const { return data_; } //const ptr, none-const data

	//void set_width(int width) { width_ = width; }
	//void set_height(int height) { height_ = height; }
	//void set_channels(int channels) { channels_ = channels; }
	void set_data(Byte * data) { delete[] data_; data_ = data; }


private:
	void LoadFromTGA(const char *filePath);

	int width_;
	int height_;
	int channels_;
	Byte *data_;
};

#endif
