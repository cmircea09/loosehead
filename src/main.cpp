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
	Timer timer;

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	timer.reset();
	while(msg.message != WM_QUIT) {
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			timer.tick();

			static int frames = 0;
			static float time_elapsed = 0.0f;

			frames++;

			if((timer.total_time() - time_elapsed) >= 1.0f) {
				float fps = (float)(frames);
				float mfps = 1000.0f / fps;

				char buffer[512];
				sprintf(buffer, "FPS: %.0f Frame Time: %f (ms) Delta Time: %f", fps, mfps, timer.delta_time());
				window.set_title(buffer);

				frames = 0;
				time_elapsed += 1.0f;
			}
		}
	}

	return msg.wParam;
}