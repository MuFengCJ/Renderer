#ifndef APP_H
#define APP_H

#include "../core/window.h"

class Renderer;

class App
{
public:
	App(const char* title, int width, int height);
	~App();

	void init() const;
	void start() const;
	void set_renderer(Renderer* renderer);
	
private:
	static void update();

	//reponse to user input of window, called by window
	static void key_callback(Window *window, KeyCode key, bool pressed);
	static void button_callback(Window *window, Button button, bool pressed);
	static void scroll_callback(Window *window, float offset);
	static void cursor_callback(Window* window, float xpos, float ypos);
	static void resize_callback(Window* window, int width, int height);

	static Window* window_;		//window to display
	static Renderer* renderer_;		//renderer

};


#endif

