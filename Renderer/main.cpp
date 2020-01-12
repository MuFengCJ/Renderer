#include <stdio.h>
#include "app/app.h"
#include "core/window.h"
#include "core/image.h"
#include "core/renderer.h"
#include "core/color.h"


static Image* image;

//按键按下去时一直处于 pressed = true 响应状态，松开时有一次 pressed = false 响应
inline void KeyCallBack(Window *window, KeyCode key, bool pressed)
{
	switch (key)
	{
	case KEY_A:
		if (pressed) {
			image->Resize(image->width()*1.5, image->height()*1.5);
			printf("Key \'A\' pressed\n");
		}
		else {
			printf("Key \'A\' unPressed\n");
		}
		break;
	case KEY_D:
		if (pressed) {
			image->Resize(image->width() / 1.5, image->height() / 1.5);
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

//鼠标点击时有一次 pressed = true 响应，松开时有一次 pressed = false 响应
inline void ButtonCallBack(Window *window, Button button, bool pressed)
{
	switch (button)
	{
	case BUTTON_L:
		if (pressed) {
			image->FlipHorizontal();
			printf("LeftButton pressed\n");
		}
		else {
			float xpos, ypos;
			window->CursorPos(xpos, ypos);
			printf("cursor x: %f, y: %f\n", xpos, ypos);
			printf("LeftButton unPressed\n");
		}
		break;
	case BUTTON_R:
		if (pressed) {
			image->FlipVertical();
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

//鼠标滚轮滚动一次就对应一次响应
inline void ScrollCallBack(Window *window, float offset)
{
	printf("scroll offset = %f\n", offset);
}

#if 0
int main(void) 
{
	CallBacks test_callbacks;
	test_callbacks.KeyCallback = KeyCallBack;
	test_callbacks.ButtonCallback = ButtonCallBack;
	test_callbacks.ScrollCallback = ScrollCallBack;

	Window *window = new Window("Render", 800, 600);
	window->set_callbacks(test_callbacks);

	image = new Image();
	image->LoadFromFile("test.tga"/*"demo.png"*/);

	while (!window->should_close()) {
		window->Display(image);
		window->PollEvents();
	}

	delete image;
	delete window;

	return 0;
}
#else
int main()
{
	App app = App("SoftRenderer", 800, 600);

	Renderer* renderer = new Renderer(800, 600);
	app.set_renderer(renderer);

	app.Init();

	app.Start();

	return 0;
}
#endif