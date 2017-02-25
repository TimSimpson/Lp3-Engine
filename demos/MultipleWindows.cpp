#include <iostream>
#include <windows.h>
#include <lp3/core.hpp>
#include <lp3/platform.hpp>

namespace core = lp3::core;

int main() {
	std::cout << "hai!\n";
	HWND hwnd = GetConsoleWindow();
	HINSTANCE hinstance = (HINSTANCE) ::GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
	core::PlatformLoop loop1("Window 1", hinstance);
	core::PlatformLoop loop2("Window 2", hinstance);
	core::PlatformLoop loop3("Window 3", hinstance);
	while (loop1.do_events() && loop2.do_events() && loop3.do_events()) {
		std::cout << ";:";
	}

	return 0;
}