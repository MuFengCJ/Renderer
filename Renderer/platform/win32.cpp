/**********************************************
* https://github.com/zauonlok/renderer/blob/v1.2/renderer/platforms/win32.c
***********************************************/

#include <Windows.h>
#include <assert.h>
#include <direct.h>
#include "../core/window.h"
#include "../core/image.h"
#include "../core/utils.h"

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

/*************************************
*  Window related functions 
**************************************/
#ifdef UNICODE
static const wchar_t *WINDOW_CLASS_NAME = L"Class";
static const wchar_t *WINDOW_ENTRY_NAME = L"Entry";
#else
static const char *WINDOW_CLASS_NAME = "Class";
static const char *WINDOW_ENTRY_NAME = "Entry";
#endif

/*
 * for virtual-key codes, see
 * https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
 */
static void handle_key_message(Window *window, WPARAM virtual_key, bool pressed) 
{
	KeyCode key;
	switch (virtual_key) {
	case 'A':      key = KEY_A;     break;
	case 'D':      key = KEY_D;     break;
	case 'S':      key = KEY_S;     break;
	case 'W':      key = KEY_W;     break;
	case VK_SPACE: key = KEY_SPACE; break;
	default:       key = KEY_NUM;   break;
	}
	if (key < KEY_NUM) {
		window->keys[key] = pressed;
		if (window->callbacks.key_callback) {
			window->callbacks.key_callback(window, key, pressed);
		}
	}
}

static void handle_button_message(Window *window, Button button, bool pressed) 
{
	window->buttons[button] = pressed;
	if (window->callbacks.button_callback) {
		window->callbacks.button_callback(window, button, pressed);
	}
}

static void handle_scroll_message(Window *window, float offset) 
{
	if (window->callbacks.scroll_callback) {
		window->callbacks.scroll_callback(window, offset);
	}
}

static LRESULT CALLBACK process_message(HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam) 
{
	Window *window = (Window*)GetProp(hWnd, WINDOW_ENTRY_NAME);
	if (window == NULL) {
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_CLOSE) {
		window->should_close = true;
		return 0;
	}
	else if (uMsg == WM_KEYDOWN) {
		handle_key_message(window, wParam, true);
		return 0;
	}
	else if (uMsg == WM_KEYUP) {
		handle_key_message(window, wParam, false);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		handle_button_message(window, BUTTON_L, true);
		return 0;
	}
	else if (uMsg == WM_RBUTTONDOWN) {
		handle_button_message(window, BUTTON_R, true);
		return 0;
	}
	else if (uMsg == WM_LBUTTONUP) {
		handle_button_message(window, BUTTON_L, false);
		return 0;
	}
	else if (uMsg == WM_RBUTTONUP) {
		handle_button_message(window, BUTTON_R, false);
		return 0;
	}
	else if (uMsg == WM_MOUSEWHEEL) {
		float offset = GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		handle_scroll_message(window, offset);
		return 0;
	}
	else {
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

static void register_class(void) 
{
	static int initialized = 0;
	if (initialized == 0) {
		ATOM class_atom;
		WNDCLASS window_class;
		window_class.style = CS_HREDRAW | CS_VREDRAW;
		window_class.lpfnWndProc = process_message;
		window_class.cbClsExtra = 0;
		window_class.cbWndExtra = 0;
		window_class.hInstance = GetModuleHandle(NULL);
		window_class.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
		window_class.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		window_class.lpszMenuName = NULL;
		window_class.lpszClassName = WINDOW_CLASS_NAME;
		class_atom = RegisterClass(&window_class);
		assert(class_atom != 0);
		//UNUSED_VAR(class_atom);
		initialized = 1;
	}
}
 
// origin of window is topLeft
static HWND create_window(const char *title_, int width, int height) 
{
	DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	RECT rect;
	HWND handle;

#ifdef UNICODE
	wchar_t title[LINE_SIZE];
	mbstowcs(title, title_, LINE_SIZE);
#else
	const char *title = title_;
#endif

	rect.left = 0;
	rect.top = 0;
	rect.right = width;
	rect.bottom = height;
	AdjustWindowRect(&rect, style, 0);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	handle = CreateWindow(WINDOW_CLASS_NAME, title, style,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	assert(handle != NULL);
	return handle;
}

/*
 * for memory device context, see
 * https://docs.microsoft.com/en-us/windows/desktop/gdi/memory-device-contexts
 */
static void window_init_buffer(HWND handle, int width, int height,
	Image* &out_buffer, HDC &out_memory_dc) 
{
	BITMAPINFOHEADER bi_header;
	HDC window_dc;
	HDC memory_dc;
	HBITMAP dib_bitmap;
	HBITMAP old_bitmap;
	unsigned char *data;
	Image *buffer;

	window_dc = GetDC(handle);
	memory_dc = CreateCompatibleDC(window_dc);
	ReleaseDC(handle, window_dc);

	memset(&bi_header, 0, sizeof(BITMAPINFOHEADER));
	bi_header.biSize = sizeof(BITMAPINFOHEADER);
	bi_header.biWidth = width;
	bi_header.biHeight = -height;  /* top-down */
	bi_header.biPlanes = 1;
	bi_header.biBitCount = 32;
	bi_header.biCompression = BI_RGB;
	dib_bitmap = CreateDIBSection(memory_dc, (BITMAPINFO*)&bi_header,
		DIB_RGB_COLORS, (void**)&data, NULL, 0);
	assert(dib_bitmap != NULL);
	old_bitmap = (HBITMAP)SelectObject(memory_dc, dib_bitmap);
	DeleteObject(old_bitmap);

	buffer = (Image*)malloc(sizeof(Image));
	buffer->width = width;
	buffer->height = height;
	buffer->channels = 4;
	buffer->data = data;

	out_buffer = buffer;
	out_memory_dc = memory_dc;
}

Window *window_create(const char *title, int width, int height) 
{
	Window *window;
	HWND handle;
	Image *buffer;
	HDC memory_dc;

	assert(width > 0 && height > 0);

	register_class();
	handle = create_window(title, width, height);
	window_init_buffer(handle, width, height, buffer, memory_dc);

	window = (Window*)malloc(sizeof(Window));
	memset(window, 0, sizeof(Window));
	window->handle = handle;
	window->memory_dc = memory_dc;
	window->buffer = buffer;
	window->should_close = false;

	SetProp(handle, WINDOW_ENTRY_NAME, window);
	ShowWindow(handle, SW_SHOW);
	return window;
}

void window_destroy(Window *window) 
{
	ShowWindow(window->handle, SW_HIDE);
	RemoveProp(window->handle, WINDOW_ENTRY_NAME);

	DeleteDC(window->memory_dc);
	DestroyWindow(window->handle);

	free(window->buffer);
	free(window);
}

bool window_should_close(Window *window) 
{
	return window->should_close;
}

void window_set_userdata(Window *window, void *userdata) 
{
	window->userdata = userdata;
}

void *window_get_userdata(Window *window) 
{
	return window->userdata;
}

static void flush_buffer(Window *window) 
{
	HDC window_dc = GetDC(window->handle);
	HDC memory_dc = window->memory_dc;
	Image *buffer = window->buffer;
	int width = buffer->width;
	int height = buffer->height;
	BitBlt(window_dc, 0, 0, width, height, memory_dc, 0, 0, SRCCOPY);
	ReleaseDC(window->handle, window_dc);
}

void window_draw_image(Window *window, Image *image) 
{
	blit_image_bgr(image, window->buffer);
	flush_buffer(window);
}

//void window_draw_buffer(Window *window, framebuffer_t *buffer) {
//	private_blit_buffer_bgr(buffer, window->buffer);
//	flush_buffer(window);
//}


/*************************************
*  input related functions 
**************************************/
void input_poll_events() 
{
	MSG message;
	while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

bool input_key_pressed(Window *window, KeyCode key) 
{
	assert(key >= 0 && key < KEY_NUM);
	return window->keys[key];
}

bool input_button_pressed(Window *window, Button button) 
{
	assert(button >= 0 && button < BUTTON_NUM);
	return window->buttons[button];
}

void input_query_cursor(Window *window, float *xpos, float *ypos) 
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(window->handle, &point);
	*xpos = (float)point.x;
	*ypos = (float)point.y;
}

void input_set_callbacks(Window *window, CallBacks callbacks) 
{
	window->callbacks = callbacks;
}


/*************************************
*  misc functions 
*************************************/
static double get_native_time() 
{
	static double period = -1;
	LARGE_INTEGER counter;
	if (period < 0) {
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		period = 1 / (double)frequency.QuadPart;
	}
	QueryPerformanceCounter(&counter);
	return counter.QuadPart * period;
}

float get_time() 
{
	static double initial = -1;
	if (initial < 0) {
		initial = get_native_time();
	}
	return (float)(get_native_time() - initial);
}

void init_path() 
{
#ifdef UNICODE
	wchar_t path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	*wcsrchr(path, L'\\') = L'\0';
	_wchdir(path);
	_wchdir(L"assets");
#else
	char path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	*strrchr(path, '\\') = '\0';
	_chdir(path);
	_chdir("assets");
#endif
}
