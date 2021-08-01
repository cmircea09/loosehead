#include "window.hpp"
#include <iostream>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	Window window(640, 480, "loosehead");

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	window.timer.reset();
	while(msg.message != WM_QUIT) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			window.timer.tick();
			window.calculate_frame_stats();

			window.graphics().clear_buffer(0.0f, 0.0f, 0.0f);
			window.graphics().end_frame();
		}
	}

	return msg.wParam;
}