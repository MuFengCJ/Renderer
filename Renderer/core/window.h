#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include "image.h"

class Window;

typedef enum { KEY_A = 0, KEY_D, KEY_S, KEY_W, KEY_SPACE, KEY_NUM } KeyCode;
typedef enum { BUTTON_L = 0, BUTTON_R, BUTTON_NUM } Button;
//function pointer
typedef struct {
	void(*key_callback)(Window *window, KeyCode key, bool pressed);
	void(*button_callback)(Window *window, Button button, bool pressed);
	void(*scroll_callback)(Window *window, float offset);
} CallBacks;

class Window
{
public:
	HWND handle;
	HDC memory_dc; //memory device context
	Image *buffer;

	/* common data of different platform */
	bool should_close;
	bool keys[KEY_NUM]; //whether certain key is pressed
	bool buttons[BUTTON_NUM]; //whether certain button is pressed
	CallBacks callbacks;
	void *userdata;
};

/* Window related functions */
Window *window_create(const char *title, int width, int height);
void window_destroy(Window *window);
bool window_should_close(Window *window);
void window_set_userdata(Window *window, void *userdata);
void *window_userdata(Window *window);
void window_draw(Window *window, Image *image);
//void window_draw(Window *window, framebuffer_t *buffer);

/* input related functions */
void input_poll_events();
bool input_key_pressed(Window *window, KeyCode key);
bool input_button_pressed(Window *window, Button button);
void input_query_cursor(Window *window, float *xpos, float *ypos);
void input_set_callbacks(Window *window, CallBacks callbacks);

/* misc functions */
float get_time();
void init_path();

#endif
