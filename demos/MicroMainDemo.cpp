#include <lp3/MicroMain.hpp>


class Main
{
public:
    Main(const std::vector<std::string> & arguments,
         lp3::core::PlatformInitArgs platform_args)
    {
        LP3_LOG_DEBUG("Greetings from the MicroMain Demo.");
        for(const std::string & element : arguments)
        {
            LP3_LOG_DEBUG(element.c_str());
        }
    }

    ~Main()
    {
    }

    void iterate()
    {
    }

    void windows_message(lp3::core::PlatformMessage msg)
    {
    }

};

LP3_MICROMAIN(Main, "MicroMainDemo");
