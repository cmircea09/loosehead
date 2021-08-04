#pragma once

#include "window.hpp"
#include "timer.hpp"

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