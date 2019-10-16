#include "renderer.h"
#include <assert.h>
#include <algorithm>
#include "color.h"

FrameBuffer::FrameBuffer(int width, int height)
{
	width_ = width;	   //correspond to x axis
	height_ = height;   //correspond to y axis
	pixel_colors_ = vector<vector<Color>>(width, vector<Color>(height, Color::Black));
}

Color FrameBuffer::get_pixel(int x, int y) const
{
	assert(x < width_ && y < height_);
	return pixel_colors_[x][y];
}

void FrameBuffer::put_pixel(int x, int y, Color color) 
{ 
	assert(x < width_ && y < height_); 
	pixel_colors_[x][y] = color; 
}

void rasterize_line(int x0, int y0, int x1, int y1, Color color, FrameBuffer* framebuffer)
{
	bool steep = false;
	if (std::abs(x1 - x0) < std::abs(y1 - y0)) { //表明斜率绝对值大于1，则相对于直线 y=x 做一个对称变换，最后画图时再恢复
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) { //确保 x0 < x1
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int deltaX = x1 - x0; //有了上述处理，保证该值大于0
	int deltaY = y1 - y0;
	int deltaError = std::abs(deltaY) * 2;
	int error = 0;
	int y = y0;

	for (int x = x0; x <= x1; x++) {
		if (steep)
			framebuffer->put_pixel(y, x, color);
		else
			framebuffer->put_pixel(x, y, color);

		error += deltaError;
		if (error > deltaX) {
			y += (deltaY > 0 ? 1 : -1);
			error -= 2 * deltaX;
		}
	}
}