#include "renderer.h"
#include <assert.h>
#include <algorithm>
#include "window.h"
#include "color.h"

FrameBuffer::FrameBuffer(int width, int height)
{
	width_ = width;	   //corresponding to x axis
	height_ = height;   //corresponding to y axis
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

Renderer::Renderer(/*const char *name, */int width, int height)
{
	framebuffer_ = new FrameBuffer(width, height);
	render_target_ = NULL;
}

Renderer::~Renderer()
{
	delete framebuffer_;
}

void Renderer::render() const
{
	draw_line(20, 30, 220, 220, Color::Cyan);
}

static void rasterize_line(int x0, int y0, int x1, int y1, Color color, FrameBuffer* framebuffer)
{
	bool steep = false;
	if (std::abs(x1 - x0) < std::abs(y1 - y0)) { //����б�ʾ���ֵ����1���������ֱ�� y=x ��һ���ԳƱ任�����ͼʱ�ٻָ�
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}
	if (x0 > x1) { //ȷ�� x0 < x1
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int deltaX = x1 - x0; //����������������֤��ֵ����0
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

static void rasterize_triangle()
{

}

void Renderer::draw_line(int x0, int y0, int x1, int y1, Color color) const
{
	rasterize_line(x0, y0, x1, y1, color, framebuffer_);
}

void Renderer::draw_triangle() const
{

}