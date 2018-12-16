#include <lp3/core/utils.hpp>
#include <boost/lexical_cast.hpp>
#ifdef LP3_COMPILE_TARGET_WINDOWS
    #include <windows.h>
#endif

namespace lp3::core {

namespace {
    std::optional<int> loop_count() {
        auto value = get_env_var("LP3_LOOP_COUNT");
        if (!value)
            return std::nullopt;
        else
            return boost::lexical_cast<int>(*value);
    }
}

PlatformLoop::PlatformLoop()
:   arguments()
{
    #if defined(LP3_COMPILE_TARGET_WINDOWS)
        int argLength;
        LPWSTR * windowsString = ::CommandLineToArgvW(GetCommandLineW(),
                                                      &argLength);
        if (nullptr == windowsString)
        {
            LP3_LOG_ERROR("Error converting command line arguments.");
			LP3_THROW2(lp3::core::Exception,
				       "Error converting command line arguments.");
        }
        for(int i = 0; i < argLength; i ++)
        {
            WCharToCharConverter original(windowsString[i]);
            this->arguments.push_back(original.GetCharPtr());
        }
        LocalFree(windowsString);
    #endif
}

PlatformLoop::PlatformLoop(int argc, char ** argv)
:   arguments()
{
    for (int i = 0; i < argc; i++) {
        this->arguments.push_back(argv[i]);
    }
}

std::vector<std::string> PlatformLoop::command_line_args() const {
    return arguments;
}

int PlatformLoop::run(std::function<bool()> iterate) {
    if (iterate) {
        const auto count = loop_count();
        if (count) {
            int c = *count;
            while(iterate() && (--c) < 0) {}
        } else {
            while(iterate()) {}
        }
    }
    return 0;
}

}
