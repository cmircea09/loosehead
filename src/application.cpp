#include "application.hpp"

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
	_window.graphics().end_frame();
}