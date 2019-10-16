#ifndef RENDERER_H
#define RENDERER_H

#include <vector>

class Window;
class Color;

using std::vector;

class FrameBuffer
{
public:
	FrameBuffer(int width, int height);

	Color get_pixel(int x, int y) const;
	void put_pixel(int x, int y, Color color);

	int width() const { return width_; }
	int height() const { return height_; }

private:
	int width_;
	int height_;
	vector< vector<Color>> pixel_colors_;
};

class Renderer
{
public:
	Renderer();
	~Renderer();

	void render() const;

protected:

	FrameBuffer* framebuffer_;

	Window* render_window_;
};

void rasterize_line(int x0, int y0, int x1, int y1, Color color, FrameBuffer* framebuffer);

#endif