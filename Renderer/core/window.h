#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <assert.h>

class Image;
class FrameBuffer;
class Window;

typedef unsigned char Byte;
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
	void draw(Image* image) const;
	void draw(FrameBuffer* framebuffer) const;

	//input message reponse
	void poll_events() const;
	void cursor_pos(float &xpos, float &ypos) const;

	// member variable access function
	int width() const { return width_; }
	int height() const { return height_; }
	bool should_close() const { return should_close_; }
	bool key_pressed(KeyCode key) const { assert(key >= 0 && key < KEY_NUM); return keys_[key]; }
	bool button_pressed(Button button) const { assert(button >= 0 && button < BUTTON_NUM); return buttons_[button]; }
	CallBacks callbacks() const { return callbacks_; }
	void* userdata() const { return userdata_; }

	void set_should_close(bool should_close) { should_close_ = should_close; }
	void set_key_pressed(KeyCode key, bool pressed) { assert(key >= 0 && key < KEY_NUM); keys_[key] = pressed; }
	void set_button_pressed(Button button, bool pressed) { assert(button >= 0 && button < BUTTON_NUM); buttons_[button] = pressed; }
	void set_callbacks(CallBacks callbacks) { callbacks_ = callbacks; }
	void set_userdata(void* userdata) { userdata_ = userdata; }
	
private:
	void init_buffer(int width, int height);
	void reset_buffer() const;
	void swap_buffer() const;

	HWND handle_;
	HDC memory_dc_;	//memory device context
	Byte* back_buffer_;	//pre-rendered buffer; RGBA 4 channels, associated with memory_dc_, auto deleted by system

	/* common data of different platform */
	int width_;
	int height_;
	bool should_close_;
	bool keys_[KEY_NUM]; //whether specific key is pressed
	bool buttons_[BUTTON_NUM]; //whether specific button is pressed
	CallBacks callbacks_;
	void *userdata_;
};

/* misc functions */
float get_time();
void init_path();

#endif
