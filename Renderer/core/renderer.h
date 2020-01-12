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

	Color GetPixel(int x, int y) const;
	void SetPixel(int x, int y, Color color);

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

	void Render() const;

	//events response
	void KeyEventResponse(KeyCode key, bool pressed) const;
	void ButtonEventResponse(Button button, bool pressed) const;
	void ScrollEventResponse(float offset) const;

	//
	void DrawLine(int x0, int y0, int x1, int y1, Color color) const;
	void DrawTriangle() const;

	FrameBuffer* framebuffer() const { return framebuffer_; }
	void set_render_target(Scene* target) { render_target_ = target; }

protected:


	FrameBuffer* framebuffer_;	 //data of one frame
	Scene* render_target_;			//scene to render
};

#endif