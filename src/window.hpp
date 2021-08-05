#pragma once

#include "strict_win.hpp"
#include "graphics.hpp"
#include <optional>
#include <memory>

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

	static std::optional<int> process_messages();
	void set_title(const char* title);
	Graphics& graphics();

private:
	HWND _window;
	std::unique_ptr<Graphics> _gfx;

	static LRESULT CALLBACK message_setup(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	static LRESULT CALLBACK message_thunk(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};