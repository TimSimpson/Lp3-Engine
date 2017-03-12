// Only included this from one cpp file to avoid errors.

namespace lp3 { namespace core {

int PlatformLoop::run(std::function<void()> iterate,
                      std::function<void(PlatformMessage)> on_message)
{
    return run(iterate, boost::optional<decltype(on_message)>(on_message));
}

int PlatformLoop::run(std::function<bool()> iterate,
                      std::function<void(PlatformMessage)> on_message)
{
    return run(iterate, boost::optional<decltype(on_message)>(on_message));
}

int PlatformLoop::run(std::function<void()> iterate,
                      boost::optional<std::function<void(PlatformMessage)>>
                          on_message) {
    // Always keep running
    return run(
		std::function<bool()>([&iterate]() -> bool { 
			iterate(); return true; }), 
		on_message);
}

// Only :
//
// int PlatformLoop::run(std::function<bool()> iterate,
//                      boost::optional<std::function<void(PlatformMessage)>>
//                          on_message) {
//
// is handled differently on each platform.

}   }
