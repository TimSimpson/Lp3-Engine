#ifndef FILE_LP3_PLATFORM_HPP
#define FILE_LP3_PLATFORM_HPP

// Handles platform specific code. Only include this after you include
// windows.h or it will fail.
#include <functional>
#include <memory>
#include <vector>
#include <boost/optional.hpp>
#include <lp3/le.hpp>
#include <lp3/core/utils.hpp>

namespace lp3 { namespace core {

#ifdef LP3_COMPILE_TARGET_WINDOWS


// --------------------------------------------------------------------
// class Platform
// --------------------------------------------------------------------
//     Contains code to fetch native Windows arguments from type-erased
//     generic classes PlatformInitArgs and PlatformMessage. Use this 
//     from Windows-aware code.
// --------------------------------------------------------------------
struct Platform {
    // Called from Windows-aware code, this creates the initialization args 
    // needed by the main loop.
	static PlatformInitArgs create_init_args(HINSTANCE hinstance, HWND hwnd) {
		PlatformInitArgs wrapper;
		wrapper.hinstance = hinstance;
		wrapper.hwnd = hwnd;
		return wrapper;
	}

    // Called from Windows-aware code, this creates the generic PlatformMessage
    // object.
	static PlatformMessage create_message(HWND hwnd, UINT message,
									      WPARAM wparam, LPARAM lparam) {
		PlatformMessage wrapper;
		wrapper.hwnd = hwnd;
		wrapper.message = message;
		wrapper.w_param = wparam;
		wrapper.l_param = lparam;
		return wrapper;
	}

    // Native arguments for Windows aware code.
	struct NativeInitArgs {
		HINSTANCE hinstance;
		HWND hwnd;
	};

    // Called from Windows-aware code, this returns the native arguments 
    // wrapped by the generic PlatformInitArgs.
    static NativeInitArgs get_native_init_args(PlatformInitArgs args) {
        NativeInitArgs result;
        result.hinstance = boost::any_cast<HINSTANCE>(args.hinstance);
        result.hwnd = boost::any_cast<HWND>(args.hwnd);
        return result;
    }

    // Native message arguments for Windows aware code.
	struct NativeMessage {
		HWND hwnd;
		UINT message;
		WPARAM wparam;
		LPARAM lparam;
	};

    // Called from Windows-aware code, this takes the generic PlatformMessage
    // and returns the native Windows arguments.
	static NativeMessage get_native_message(PlatformMessage msg) {
		NativeMessage result;
		result.hwnd = boost::any_cast<HWND>(msg.hwnd);
		result.message = boost::any_cast<UINT>(msg.message);
		result.wparam = boost::any_cast<WPARAM>(msg.w_param);
		result.lparam = boost::any_cast<LPARAM>(msg.l_param);
		return result;
	}
};


// --------------------------------------------------------------------
// class PlatformLoop
// --------------------------------------------------------------------
//     Contains code to run the main loop on a given platform.
//     This class is not type-erased but doesn't need to be as a main
//     unit will have to be aware of platform specific arguments.
//     main.hpp has code to create the main loop and return this.
// --------------------------------------------------------------------
class PlatformLoop {
public:
    PlatformLoop(const char * const programName,
                 HINSTANCE hInstance, 
				 int nCmdShow=SW_SHOW);    

	PlatformLoop(const PlatformLoop & pl);

    // A vector of strings representing the command line arguments.
    std::vector<std::string> command_line_args() const;

    // The platform initialization arguments, which are needed by Lp3::Engine
    // code.
    PlatformInitArgs platform_args();

	bool do_events();

	// Run this to handle platform things. Does not call the iterate function.
	// Returns true if the platform said to continue going, and false if the
	// platform wants to quit.
	bool do_events(
		boost::optional<std::function<void(PlatformMessage)>> on_message);

    // Runs the main loop. Accepts two functors, the first of which is called
    // every frame and the second which is called each time the underlying 
    // platform emits an event.
    int run(std::function<void()> iterate,
            std::function<void(PlatformMessage)> on_message);

	int run(std::function<bool()> iterate,
		    std::function<void(PlatformMessage)> on_message);

	int run(std::function<void()> iterate,
		    boost::optional<std::function<void(PlatformMessage)>> on_message
		        = boost::none);

    // Runs the main loop. Accepts two functors, the first of which is called
    // every frame and the second which is called each time the underlying 
    // platform emits an event. The second argument can be omitted if you 
    // do not wish handle platform events. The loop stops when the first
	// argument stops returning true, but only on some platforms.
    int run(std::function<bool()> iterate,
            boost::optional<std::function<void(PlatformMessage)>> on_message
                =boost::none);	

private:
	struct WindowDestroyer {
		HWND hwnd;

		WindowDestroyer(HWND hwnd);

		~WindowDestroyer();
	};

    std::vector<std::string> arguments;
    HINSTANCE hinstance;
    HWND hwnd;
	std::shared_ptr<WindowDestroyer> wd;
};

#else


struct Platform {
    static PlatformInitArgs create_init_args() {
        return PlatformInitArgs{};
    }
};

class PlatformLoop {
public:
    PlatformLoop(int argc, char ** argv);
    PlatformLoop(const PlatformLoop & other) = delete;
    PlatformLoop & operator=(const PlatformLoop & other) = delete;

    std::vector<std::string> command_line_args() const;

    PlatformInitArgs platform_args();

    int run(std::function<void()> iterate,
        std::function<void(PlatformMessage)> on_message);
    int run(std::function<void()> iterate,
            boost::optional<std::function<void(PlatformMessage)>> on_message
                =boost::none);
private:
    std::vector<std::string> arguments;
};

#endif


}   }



#endif
