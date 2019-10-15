#ifndef RENDERER_H
#define RENDERER_H

#include "window.h"

class Renderer
{
public:
	Renderer();
	~Renderer();

	void render() const;

protected:
	Window* windowToRender;
};

#endif