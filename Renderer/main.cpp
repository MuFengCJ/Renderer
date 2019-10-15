#include <stdio.h>
#include "core/window.h"
#include "core/image.h"


static Image* image;

//按键按下去时一直处于 pressed = true 响应状态，松开时有一次 pressed = false 响应
inline void keyCallBack(Window *window, KeyCode key, bool pressed)
{
	switch (key)
	{
	case KEY_A:
		if (pressed) {
			image->resize(image->width()*1.5, image->height()*1.5);
			printf("Key \'A\' pressed\n");
		}
		else {
			printf("Key \'A\' unPressed\n");
		}
		break;
	case KEY_D:
		if (pressed) {
			image->resize(image->width() / 1.5, image->height() / 1.5);
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
inline void buttonCallBack(Window *window, Button button, bool pressed)
{
	switch (button)
	{
	case BUTTON_L:
		if (pressed) {
			image->flipHorizontal();
			printf("LeftButton pressed\n");
		}
		else {
			printf("LeftButton unPressed\n");
		}
		break;
	case BUTTON_R:
		if (pressed) {
			image->flipVertical();
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
inline void scrollCallBack(Window *window, float offset)
{
	printf("scroll offset = %f\n", offset);
}

int main(void) 
{
	CallBacks testCallBacks;
	testCallBacks.key_callback = keyCallBack;
	testCallBacks.button_callback = buttonCallBack;
	testCallBacks.scroll_callback = scrollCallBack;

	Window *window = new Window("Render", 800, 600);
	window->set_callbacks(testCallBacks);

	image = new Image();
	image->loadFromFile("test.tga"/*"demo.png"*/);

	while (!window->should_close()) {
		window->draw(image);
		window->poll_events();
	}

	delete image;
	delete window;

	return 0;
}