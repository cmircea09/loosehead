#include "window.hpp"

class Application
{
public:
	Application();
	int start();

private:
	void do_frame();

	Window _window;
	Timer _timer;
};

Application::Application() : _window(640, 480, "loosehead")
{
	_timer.reset();
}

int Application::start()
{
	while(true) {
		if(const auto e = Window::process_messages()) {
			return *e;
		}

		do_frame();
	}
}

void Application::do_frame()
{
	_timer.tick();

	static int frames = 0;
	static float time_elapsed = 0.0f;

	frames++;

	if((_timer.total_time() - time_elapsed) >= 1.0f) {
		float fps = (float)(frames);
		float mfps = 1000.0f / fps;

		char buffer[512];
		sprintf(buffer, "FPS: %.0f Frame Time: %f (ms) Delta Time: %f", fps, mfps, _timer.delta_time());
		_window.set_title(buffer);

		frames = 0;
		time_elapsed += 1.0f;
	}

	const float c = (float)sin(_timer.delta_time()) / 2.0f + 0.5f;
	_window.graphics().clear_buffer(c, c, 1.0f);
	_window.graphics().draw_triangle_test();
	_window.graphics().end_frame();
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	try {
		return Application{}.start();
	}
	catch(const Exception& e) {
		MessageBox(nullptr, e.what(), e.type(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch(const std::exception& e) {
		MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch(...) {
		MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return 0;
}