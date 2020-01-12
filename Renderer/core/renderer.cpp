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

Color FrameBuffer::GetPixel(int x, int y) const
{
	assert(x < width_ && y < height_);
	return pixel_colors_[x][y];
}

void FrameBuffer::SetPixel(int x, int y, Color color) 
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

void Renderer::Render() const
{
	DrawLine(20, 30, 220, 220, Color::Cyan);
}

static void rasterize_line(int x0, int y0, int x1, int y1, Color color, FrameBuffer* framebuffer)
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
			framebuffer->SetPixel(y, x, color);
		else
			framebuffer->SetPixel(x, y, color);

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

void Renderer::DrawLine(int x0, int y0, int x1, int y1, Color color) const
{
	rasterize_line(x0, y0, x1, y1, color, framebuffer_);
}

void Renderer::DrawTriangle() const
{

}

void Renderer::KeyEventResponse(KeyCode key, bool pressed) const
{
	switch (key)
	{
	case KEY_A:
		if (pressed) {
			printf("Key \'A\' pressed\n");
		}
		else {
			printf("Key \'A\' unPressed\n");
		}
		break;
	case KEY_D:
		if (pressed) {
			printf("Key \'D\' pressed\n");
		}
		else {
			printf("Key \'D\' unPressed\n");
		}
		break;
	default:
		break;
	}
}

void Renderer::ButtonEventResponse(Button button, bool pressed) const
{
	switch (button)
	{
	case BUTTON_L:
		if (pressed) {
			printf("LeftButton pressed\n");
		}
		else {
			printf("LeftButton unPressed\n");
		}
		break;
	case BUTTON_R:
		if (pressed) {
			printf("RightButton pressed\n");
		}
		else {
			printf("RightButton unPressed\n");
		}
		break;
	default:
		break;
	}
}

void Renderer::ScrollEventResponse(float offset) const
{
	printf("scroll offset = %f\n", offset);
}