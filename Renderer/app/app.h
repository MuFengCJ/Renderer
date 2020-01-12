#ifndef APP_H
#define APP_H

#include "../core/window.h"

class Renderer;

class App
{
public:
	App(const char* title, int width, int height);
	~App();

	void Init() const;
	void Start() const;

	void set_renderer(Renderer* renderer);
	
private:
	static void Update();

	//reponse to user input message, called by window
	static void KeyCallback(Window *window, KeyCode key, bool pressed);
	static void ButtonCallback(Window *window, Button button, bool pressed);
	static void ScrollCallback(Window *window, float offset);
	static void CursorCallback(Window* window, float xpos, float ypos);
	static void ResizeCallback(Window* window, int width, int height);

	static Window* window_;		//window for display
	static Renderer* renderer_;		//renderer
};


#endif

