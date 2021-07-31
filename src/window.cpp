#include "window.hpp"

static HWND window = nullptr;

LRESULT WINAPI wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::init()
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = wnd_proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hIcon = nullptr;
	wc.hCursor = nullptr;
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = "LPSZ_CLASS_NAME";
	wc.hIconSm = nullptr;

	RegisterClassEx(&wc);

	RECT rt = {0, 0, 640, 480};
	AdjustWindowRect(&rt, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, 0);

	int w = rt.right - rt.left;
	int h = rt.bottom - rt.top;

	window = CreateWindow(
		"LPSZ_CLASS_NAME",
		"loosehead",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		w,
		h,
		NULL,
		NULL,
		wc.hInstance,
		NULL
	);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void Window::loop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while(msg.message != WM_QUIT) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			/* update & draw code here*/
		}
	}
}

void Window::destroy()
{
	DestroyWindow(window);
	UnregisterClass("LPSZ_CLASS_NAME", GetModuleHandle(NULL));
}

HWND Window::handle() 
{
	return window;
}