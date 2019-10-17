/**********************************************
* https://github.com/zauonlok/renderer/blob/v1.2/renderer/platforms/win32.c
***********************************************/
#include "../core/window.h"
#include <direct.h>
#include "../core/utils.h"


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
static void key_message_response(Window *window, WPARAM virtual_key, bool pressed) 
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
		window->set_key_pressed(key, pressed);
		if (window->callbacks().key_callback) {
			window->callbacks().key_callback(window, key, pressed);
		}
	}
}

static void button_message_response(Window *window, Button button, bool pressed) 
{
	window->set_button_pressed(button, pressed);
	if (window->callbacks().button_callback) {
		window->callbacks().button_callback(window, button, pressed);
	}
}

static void scroll_message_response(Window *window, float offset) 
{
	if (window->callbacks().scroll_callback) {
		window->callbacks().scroll_callback(window, offset);
	}
}

static LRESULT CALLBACK process_message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
	Window *window = (Window*)GetProp(hWnd, WINDOW_ENTRY_NAME);
	if (window == NULL) {
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	else if (uMsg == WM_CLOSE) {
		window->set_should_close(true);
		return 0;
	}
	else if (uMsg == WM_KEYDOWN) {
		key_message_response(window, wParam, true);
		return 0;
	}
	else if (uMsg == WM_KEYUP) {
		key_message_response(window, wParam, false);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN) {
		button_message_response(window, BUTTON_L, true);
		return 0;
	}
	else if (uMsg == WM_RBUTTONDOWN) {
		button_message_response(window, BUTTON_R, true);
		return 0;
	}
	else if (uMsg == WM_LBUTTONUP) {
		button_message_response(window, BUTTON_L, false);
		return 0;
	}
	else if (uMsg == WM_RBUTTONUP) {
		button_message_response(window, BUTTON_R, false);
		return 0;
	}
	else if (uMsg == WM_MOUSEWHEEL) {
		float offset = GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		scroll_message_response(window, offset);
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

	HWND handle = CreateWindow(WINDOW_CLASS_NAME, title, style,
		CW_USEDEFAULT, CW_USEDEFAULT, width, height,
		NULL, NULL, GetModuleHandle(NULL), NULL);
	assert(handle != NULL);
	return handle;
}

/*
 * for memory device context, see
 * https://docs.microsoft.com/en-us/windows/desktop/gdi/memory-device-contexts
 */
void Window::init_buffer(int width, int height)
{
	Byte* data;

	HDC window_dc = GetDC(handle_);
	memory_dc_ = CreateCompatibleDC(window_dc);
	ReleaseDC(handle_, window_dc);

	BITMAPINFOHEADER bi_header;
	memset(&bi_header, 0, sizeof(BITMAPINFOHEADER));
	bi_header.biSize = sizeof(BITMAPINFOHEADER);
	bi_header.biWidth = width;
	bi_header.biHeight = -height;  /* top-down */
	bi_header.biPlanes = 1;
	bi_header.biBitCount = 32; //RGBA, 4 channels, each channel has 8 bits;
	bi_header.biCompression = BI_RGB;
	HBITMAP dib_bitmap = CreateDIBSection(memory_dc_, (BITMAPINFO*)&bi_header,
		DIB_RGB_COLORS, (void**)&data, NULL, 0);
	assert(dib_bitmap != NULL);
	HBITMAP old_bitmap = (HBITMAP)SelectObject(memory_dc_, dib_bitmap);
	DeleteObject(old_bitmap);

	back_buffer_ = data; //initialize back_buffer_ with system-allocated space
}

Window::Window(const char *title, int width, int height)
{
	assert(width > 0 && height > 0);
	width_ = width;
	height_ = height;

	register_class();

	handle_ = create_window(title, width, height);
	init_buffer(width, height);
	should_close_ = false;

	SetProp(handle_, WINDOW_ENTRY_NAME, this);
	ShowWindow(handle_, SW_SHOW);
}

Window::~Window()
{
	ShowWindow(handle_, SW_HIDE);
	RemoveProp(handle_, WINDOW_ENTRY_NAME);

	DeleteDC(memory_dc_);
	DestroyWindow(handle_);
}

void Window::swap_buffer() const
{
	HDC window_dc = GetDC(handle_);
	BitBlt(window_dc, 0, 0, width_, height_, memory_dc_, 0, 0, SRCCOPY);
	ReleaseDC(handle_, window_dc);
}

void Window::reset_buffer() const
{
	memset(back_buffer_, 0, width_ * height_ * 4);
}

void Window::display(Image *image) const
{
	reset_buffer();
	blit_image_bgr(image, width_, height_, back_buffer_);
	swap_buffer();
}

void Window::display(FrameBuffer* framebuffer) const
{
	reset_buffer();
	blit_frame_bgr(framebuffer, width_, height_, back_buffer_);
	swap_buffer();
}


/*************************************
*  input related functions 
**************************************/
void Window::poll_events() const
{
	MSG message;
	while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

void Window::cursor_pos(float &xpos, float &ypos) const
{
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(handle_, &point);
	xpos = (float)point.x;
	ypos = (float)point.y;
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
