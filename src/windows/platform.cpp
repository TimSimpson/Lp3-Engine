#include <mutex>
#include <windows.h>
#include <lp3/platform.hpp>

#include <boost/scope_exit.hpp>
#include <lp3/assert.hpp>

namespace lp3 { namespace core {

namespace {
	using CallBackFunction = std::function<void(PlatformMessage)> *;

	std::mutex call_backs_mutex;
	std::vector<std::pair<HWND, CallBackFunction>> call_backs;    
	
	auto find_callback_itr(const HWND hwnd) {
		return std::find_if(
			call_backs.begin(),
			call_backs.end(),
			[&hwnd](const auto e) {
			return e.first == hwnd;
		});
	}

	CallBackFunction find_callback(const HWND hwnd) {
		std::lock_guard<std::mutex> lock(call_backs_mutex);
		auto location = find_callback_itr(hwnd);
		if (location != call_backs.end()) {
			return (*location).second;
		} else {
			return nullptr;
		}
	}

	void register_callback(const HWND hwnd, CallBackFunction cb) {		
		std::lock_guard<std::mutex> lock(call_backs_mutex);
		LP3_ASSERT_TRUE(cb != nullptr);
		call_backs.push_back(std::pair<HWND, CallBackFunction>(hwnd, cb));
	}

	void remove_callback(const HWND hwnd) {
		std::lock_guard<std::mutex> lock(call_backs_mutex);
		auto location = find_callback_itr(hwnd);
		if (location != call_backs.end()) {
			LP3_LOG_DEBUG("Erasing callback.");
			call_backs.erase(location);
		}
	}

    struct CallBack {

        static LRESULT CALLBACK window_proc(HWND hwnd, UINT message, 
                                            WPARAM wParam, LPARAM lParam) {
            switch (message) {
            case WM_DESTROY: {
                PostQuitMessage(0);
                return 0;
            } break;
            }		
			CallBackFunction cb = find_callback(hwnd);
			if (cb) {
				// I don't *think* this is a race condition. :/
				lp3::core::PlatformMessage msg =
					lp3::core::Platform::create_message(hwnd, message,
						wParam, lParam);
				(*cb)(msg);
			}
			return DefWindowProc(hwnd, message, wParam, lParam);
        }
    };
}

PlatformLoop::WindowDestroyer::WindowDestroyer(HWND hwnd)
	:hwnd(hwnd)
{}

PlatformLoop::WindowDestroyer::~WindowDestroyer() {
	DestroyWindow(hwnd);
}

PlatformLoop::PlatformLoop(const char * const program_name, 
                           HINSTANCE hinstance, int n_cmd_show)
:   arguments(),
    hinstance(hinstance),
	wd(new WindowDestroyer(nullptr))
{    
    using lp3::core::WCharToCharConverter;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = CallBack::window_proc;
    wc.hInstance = hinstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "WindowClass";

    RegisterClassEx(&wc);

    this->hwnd = CreateWindowEx(NULL,
                                "WindowClass",
                                program_name,
                                WS_OVERLAPPEDWINDOW,
                                0, 0,
                                640, 480,
                                NULL,
                                NULL,
                                hinstance,
                                NULL);

	wd->hwnd = this->hwnd;

    int arg_length;
    LPWSTR * windows_string = CommandLineToArgvW(GetCommandLineW(),
                                                 &arg_length);
    if (NULL == windows_string) {
        LP3_FAIL("Error converting command line arguments.");
    }
    for (int i = 0; i < arg_length; i++) {
        WCharToCharConverter original(windows_string[i]);
        this->arguments.push_back(original.GetCharPtr());
    }
    LocalFree(windows_string);

    ShowWindow(this->hwnd, n_cmd_show);
}

PlatformLoop::PlatformLoop(const PlatformLoop & pl) 
:	arguments(pl.arguments),
	hinstance(pl.hinstance),
	hwnd(pl.hwnd),
	wd(pl.wd)
{
}

std::vector<std::string> PlatformLoop::command_line_args() const {
    return arguments;
}

PlatformInitArgs PlatformLoop::platform_args() {
    return Platform::create_init_args(this->hinstance, this->hwnd);
}

bool PlatformLoop::do_events() {
	return do_events(boost::none);
}

bool PlatformLoop::do_events(
	boost::optional<std::function<void(PlatformMessage)>> on_message) {
	if (on_message) {
		register_callback(this->hwnd, &(on_message.get()));
	}
	const HWND hwnd = this->hwnd;
	BOOST_SCOPE_EXIT(&hwnd) {
		remove_callback(hwnd);
	} BOOST_SCOPE_EXIT_END

	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.message != WM_QUIT;
}

int PlatformLoop::run(std::function<void()> iterate, 
                      std::function<void(PlatformMessage)> on_message)
{
    return run(iterate, boost::optional<decltype(on_message)>(on_message));
}

int PlatformLoop::run(std::function<void()> iterate, 
                      boost::optional<std::function<void(PlatformMessage)>> 
                          on_message) {
    if (on_message) {
		register_callback(this->hwnd, &(on_message.get()));        
    }
	const HWND hwnd = this->hwnd;
    BOOST_SCOPE_EXIT(&hwnd) {
		remove_callback(hwnd);
    } BOOST_SCOPE_EXIT_END

    MSG msg;

    while (true) {        
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (msg.message == WM_QUIT) {
            break;
        }
        else {
            iterate();
        }
    }
    return (int)msg.wParam;
}

}   }