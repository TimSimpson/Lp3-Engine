#include <lp3/core/exit.hpp>
#include <list>
#include <mutex>
#include <thread>
#include <gsl/gsl>
#include <lp3/assert.hpp>


namespace lp3::core {

namespace {

    typedef void(*ExitCall)();

    // Visual C++'s memory leak detector complains if a typical global variable
    // is used for a complex type, but not if we use a pointer, which coerces
    // some of the anti-patterns seen here.
    static std::list<gsl::owner<GlobalResource *>> * & get_globals() {
        static std::list<gsl::owner<GlobalResource *>> * calls
			= new std::list<gsl::owner<GlobalResource *>>();
        return calls;
    }

    bool initialized = false;
    bool has_shut_down = false;

    void clean_up() {
        if (get_globals() == nullptr) {
            return;
        }
		for (auto p : *get_globals()) {
			delete p;
		}
        delete get_globals();
        get_globals() = nullptr;
    }

    // I'm adding a custom object in this unit that will
    // call this in it's destructor, so on non-crazy compilers this
    // will just work. But in my use case I will always use the
    // OnExitCleanUp class from the LP3_MAIN macro, which means this
    // may get called a second time which can be ignored.
    struct AuxCleaner {
        ~AuxCleaner() {
            clean_up();
        }
    } aux_cleaner;
}

OnExitCleanUp::OnExitCleanUp() {
    LP3_ASSERT(!initialized);
    initialized = true;
}

OnExitCleanUp::~OnExitCleanUp() {
    // Unlike the above use case, no client code should be try to call
    // this twice.
	LP3_ASSERT(!has_shut_down);
    has_shut_down = true;
    clean_up();
}

void on_exit_clean_up(GlobalResource * gr) {
	static std::mutex _mutex;
	std::lock_guard<std::mutex> guard(_mutex);
	get_globals()->emplace_back(gr);
}

}
