#include "app.h"
#include "../core/renderer.h"


Window* App::window_;
Renderer* App::renderer_;


App::App(const char* title, int width, int height)
{
	window_ = new Window(title, width, height);
	renderer_ = NULL;
}

App::~App()
{
	delete window_;
	delete renderer_;
}

void App::Init() const
{
	//set window callbacks
	CallBacks callbacks;
	callbacks.KeyCallback = KeyCallback;
	callbacks.ButtonCallback = ButtonCallback;
	callbacks.ScrollCallback = ScrollCallback;
	window_->set_callbacks(callbacks);

	if (renderer_) {
		
	}
}

void App::Start() const
{
	//main render loop
	while (!window_->should_close()) {
		Update();
	}
}

void App::Update()
{
	//run user renderer
	if (renderer_) {
		renderer_->Render();
	}

	//display rendering result on screen
	window_->Display(renderer_->framebuffer());

	//poll events
	window_->PollEvents();
}

void App::set_renderer(Renderer* renderer)
{
	renderer_ = renderer;
}

void App::KeyCallback(Window *window, KeyCode key, bool pressed)
{
	renderer_->KeyEventResponse(key, pressed);
}

void App::ButtonCallback(Window *window, Button button, bool pressed)
{
	float pos_x, pos_y;
	window->CursorPos(pos_x, pos_y);
	printf("cursor x: %f, y: %f\n", pos_x, pos_y);

	renderer_->ButtonEventResponse(button, pressed);
}

void App::ScrollCallback(Window *window, float offset)
{
	renderer_->ScrollEventResponse(offset);
}