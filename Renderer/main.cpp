#include <stdio.h>
#include "core/window.h"
#include "core/image.h"

//按键按下去时一直处于 pressed = true 响应状态，松开时有一次 pressed = false 响应
inline void keyCallBack(Window *window, KeyCode key, bool pressed)
{
	Image* bufferPtr = window->buffer;
	switch (key)
	{
	case KEY_A:
		if (pressed) {
			//bufferPtr->resize(bufferPtr->width()*1.5, bufferPtr->height()*1.5);
			printf("Key \'A\' pressed\n");
		}
		else {
			//bufferPtr->resize(bufferPtr->width() / 1.5, bufferPtr->height() / 1.5);
			printf("Key \'A\' unPressed\n");
		}
		break;
	case KEY_D:
		if (pressed) {
			//bufferPtr->resize(bufferPtr->width()*0.8, bufferPtr->height()*0.8);
			printf("Key \'D\' pressed\n");
		}
		else {
			//bufferPtr->resize(bufferPtr->width() / 0.8, bufferPtr->height() / 0.8);
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
	Image* bufferPtr = window->buffer;
	switch (button)
	{
	case BUTTON_L:
		if (pressed) {
			//bufferPtr->flipHorizontal();
			printf("LeftButton pressed\n");
		}
		else {
			printf("LeftButton unPressed\n");
		}
		break;
	case BUTTON_R:
		if (pressed) {
			//bufferPtr->flipVertical();
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

int main(void) {
	CallBacks testCallBacks;
	testCallBacks.key_callback = keyCallBack;
	testCallBacks.button_callback = buttonCallBack;
	testCallBacks.scroll_callback = scrollCallBack;

	Window *window = window_create("Hello", 800, 600);
	input_set_callbacks(window, testCallBacks);
	Image image;
	image.loadFromFile("test.tga"/*"demo.png"*/);

	while (!window_should_close(window)) {
		//if (input_key_pressed(window, KEY_A)) {
		//	image.resize(800, 600);
		//	printf("Key \'A\' pressed\n");
		//}
		//if (input_button_pressed(window, BUTTON_L)) {
		//	image.flipHorizontal();
		//	printf("Left MouseButton pressed\n");
		//}

		window_draw(window, &image);
		input_poll_events();
	}

	window_destroy(window);

	return 0;
}