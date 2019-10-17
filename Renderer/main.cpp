#include <stdio.h>
#include "app/app.h"
#include "core/window.h"
#include "core/image.h"
#include "core/renderer.h"
#include "core/color.h"


static Image* image;

//��������ȥʱһֱ���� pressed = true ��Ӧ״̬���ɿ�ʱ��һ�� pressed = false ��Ӧ
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

//�����ʱ��һ�� pressed = true ��Ӧ���ɿ�ʱ��һ�� pressed = false ��Ӧ
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
			float xpos, ypos;
			window->cursor_pos(xpos, ypos);
			printf("cursor x: %f, y: %f\n", xpos, ypos);
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

//�����ֹ���һ�ξͶ�Ӧһ����Ӧ
inline void scrollCallBack(Window *window, float offset)
{
	printf("scroll offset = %f\n", offset);
}

#if 0
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
		window->display(image);
		window->poll_events();
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

	app.init();

	app.start();

	return 0;
}
#endif