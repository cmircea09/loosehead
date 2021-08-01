#pragma once

#include "strict_win.hpp"
#include "timer.hpp"

class Window
{
public:
	class WC
	{
	public:
		static const char* name();
		static HINSTANCE instance();

	private:
		WC();
		~WC();

		static constexpr const char* _name = "loosehead";
		static WC _wc;
		HINSTANCE _instance;
	};

public:
	Window(int width, int height, const char* title);
	~Window();

	void set_title(const char* title);
private:
	HWND _window;

	static LRESULT CALLBACK message_setup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK message_thunk(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};