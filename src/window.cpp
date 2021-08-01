#include "window.hpp"

Window::WC Window::WC::_wc;

Window::WC::WC() : _instance(GetModuleHandle(nullptr))
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = message_setup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instance();
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = name();
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);
}

Window::WC::~WC()
{
	UnregisterClass(name(), instance());
}

const char* Window::WC::name()
{
	return _name;
}

HINSTANCE Window::WC::instance()
{
	return _wc._instance;
}

Window::Window(int width, int height, const char* name)
{
	RECT rt = {0, 0, width, height};
        AdjustWindowRect(&rt, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 0);

        int w = rt.right - rt.left;
        int h = rt.bottom - rt.top;

	_window = CreateWindow(
		WC::name(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		w,
		h,
		nullptr,
		nullptr,
		WC::instance(),
		this
	);

	ShowWindow(_window, SW_SHOWDEFAULT);

	_gfx = std::make_unique<Graphics>(_window);
}

Window::~Window()
{
	DestroyWindow(_window);
}

LRESULT CALLBACK Window::message_setup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(msg == WM_NCCREATE) {
		const CREATESTRUCTW* const create = reinterpret_cast<CREATESTRUCTW*>(lparam);
		Window* const t = static_cast<Window*>(create->lpCreateParams);
		
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(t));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::message_thunk));
		
		return t->wnd_proc(hwnd, msg, wparam, lparam);
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK Window::message_thunk(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	Window* const t = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return t->wnd_proc(hwnd, msg, wparam, lparam);
}

LRESULT CALLBACK Window::wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::set_title(const char* title)
{
	SetWindowText(_window, title);
}

Graphics& Window::graphics()
{
	return *_gfx;
}