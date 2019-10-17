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

void App::init() const
{
	//set window callbacks
	CallBacks callbacks;
	callbacks.key_callback = key_callback;
	callbacks.button_callback = button_callback;
	callbacks.scroll_callback = scroll_callback;
	window_->set_callbacks(callbacks);

	if (renderer_) {
		
	}
}

void App::start() const
{
	//main render loop
	while (!window_->should_close()) {
		update();
	}
}

void App::update()
{
	//run user renderer
	if (renderer_) {
		renderer_->render();
	}

	//display rendering result on screen
	window_->display(renderer_->framebuffer());

	//poll events
	window_->poll_events();
}

void App::set_renderer(Renderer* renderer)
{
	renderer_ = renderer;
}

void App::key_callback(Window *window, KeyCode key, bool pressed)
{
	//renderer_->key_events(key, pressed);
}

void App::button_callback(Window *window, Button button, bool pressed)
{
	//renderer_->button_events(button, pressed);
}

void App::scroll_callback(Window *window, float offset)
{
	//renderer_->scroll_events(offset);
}