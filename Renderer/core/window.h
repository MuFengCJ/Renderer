#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <assert.h>
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
	Window(const char *title, int width, int height);
	~Window();

	//display function
	void display() const;
	void swapBuffer() const;
	void draw(Image* img) const;

	//input message reponse
	void poll_events() const;
	void handle_key_message(WPARAM virtual_key, bool pressed);
	void handle_button_message(Button button, bool pressed);
	void handle_scroll_message(float offset);

	// load/save function
	HWND handle() const { return handle_; }
	HDC memory_dc() const { return memory_dc_; }
	int width() const { return width_; }
	int height() const { return height_; }
	Image* buffer() const { return back_buffer_; }
	bool should_close() const { return should_close_; }
	bool key_pressed(KeyCode key) const { assert(key >= 0 && key < KEY_NUM); return keys_[key]; }
	bool button_pressed(Button button) const { assert(button >= 0 && button < BUTTON_NUM); return buttons_[button]; }
	CallBacks callbacks() const { return callbacks_; }
	void* userdata() const { return userdata_; }

	void set_should_close(bool should_close) { should_close_ = should_close; }
	void set_callbacks(CallBacks callbacks) { callbacks_ = callbacks; }
	void set_userdata(void* userdata) { userdata_ = userdata; }
	
private:
	void init_buffer(int width, int height);
	void reset_buffer() const { memset(back_buffer_->data(), 0, back_buffer_->data_size()); }

	HWND handle_;
	HDC memory_dc_;     //memory device context
	UInt8* front_buffer_; //actual pixel space to display on screen, associated with memory_dc_, auto freed by system
	Image *back_buffer_; //pre-rendered buffer

	/* common data of different platform */
	int width_;
	int height_;
	bool should_close_;
	bool keys_[KEY_NUM]; //whether certain key is pressed
	bool buttons_[BUTTON_NUM]; //whether certain button is pressed
	CallBacks callbacks_;
	void *userdata_;
};

/* input related functions */
void input_query_cursor(Window *window, float *xpos, float *ypos);

/* misc functions */
float get_time();
void init_path();

#endif
