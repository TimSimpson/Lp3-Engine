#include <iostream>
#include <map>
#include <string>

#include <lp3/core.hpp>
#include <lp3/main.hpp>

namespace core = lp3::core;

// Everyone knows that global variables are bad, but they also allow us to
// do neato reflection type things.
// Unfortunately Microsoft Visual C++'s leak detector is triggered if any
// global variables are added to the program (supposedly it's not that severe
// and has to do with config options but I haven't been able to make it work).
//
// So I've written a few custom things to create globals using pointers and
// make sure they are deleted, as shown here.

struct Data {
    const char * name;
    const char * desc;
};

namespace {
    core::GlobalVar<std::map<std::string, Data>> data;
}

struct RegisterData {
    RegisterData(const char * name, const char * desc) {
        data.get()[std::string(name)] = Data{name, desc};
    }
};

RegisterData data_1("Tim", "Author");
RegisterData data_2("C++", "Language");

int _main(core::PlatformLoop & loop) {	
    lp3::core::LogSystem log;

    LP3_LOG_DEBUG("Let's play with crazy global data.");

	for (auto & e : data.get()) {
		LP3_ASSERT(e.first == e.second.name);
		LP3_LOG_DEBUG("%s - %s", e.first, e.second.desc);
	}

    LP3_LOG_DEBUG("Good bye.");
	return 0;
}

LP3_MAIN(_main)
