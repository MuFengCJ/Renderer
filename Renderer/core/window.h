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
// message response function pointer
typedef struct {
	void(*KeyCallback)(Window *window, KeyCode key, bool pressed);
	void(*ButtonCallback)(Window *window, Button button, bool pressed);
	void(*ScrollCallback)(Window *window, float offset);
} CallBacks;

class Window
{
public:
	Window(const char *title, int width, int height);
	~Window();

	// display function
	void Display(Image* image) const;
	void Display(FrameBuffer* framebuffer) const;

	// monitor input message
	void PollEvents() const;
	// get cursor pos within window
	void CursorPos(float &xpos, float &ypos) const;

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
	void InitBuffer(int width, int height);
	void ResetBuffer() const;
	void SwapBuffer() const;

	// common variable of different platform
	int width_;
	int height_;
	bool should_close_;
	bool keys_[KEY_NUM]; // whether specific key is pressed
	bool buttons_[BUTTON_NUM]; // whether specific button is pressed
	CallBacks callbacks_;
	void *userdata_;
};

/* misc functions */
float get_time();
void init_path();

#endif
