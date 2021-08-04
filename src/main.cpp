#include "strict_win.hpp"
#include "exception.hpp"
#include "application.hpp"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR lpCmdLine, INT nCmdShow)
{
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