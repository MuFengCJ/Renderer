#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "window.h"

class Color;
class Scene;

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
	Renderer(/*const char *name, */int width, int height);
	~Renderer();

	void render() const;

	//events response
	void key_events(KeyCode key, bool pressed) const;
	void button_events(Button button, bool pressed) const;
	void scroll_events(float offset) const;

	//
	void draw_line(int x0, int y0, int x1, int y1, Color color) const;
	void draw_triangle() const;

	FrameBuffer* framebuffer() const { return framebuffer_; }
	void set_render_target(Scene* target) { render_target_ = target; }

protected:


	FrameBuffer* framebuffer_;	 //data of one frame
	Scene* render_target_;			//scene to render
};

#endif