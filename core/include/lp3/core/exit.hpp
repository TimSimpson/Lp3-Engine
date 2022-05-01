#ifndef LP3_CORE_EXIT_HPP
#define LP3_CORE_EXIT_HPP
#pragma once

#include <utility>

namespace lp3 { namespace core {

typedef void(*ExitCall)();

// This is called automatically by main.hpp.
class OnExitCleanUp {
public:
    OnExitCleanUp();
    ~OnExitCleanUp();
};


struct GlobalResource {
	virtual ~GlobalResource() {}
};

void on_exit_clean_up(GlobalResource *);

// --------------------------------------------------------------------
// class GlobalVar
// --------------------------------------------------------------------
// In C++11 and above you can declare certain global variables and be
// certain it will be initialized correctly without suffering from
// race conditions or other historical problems.
//
// Unfortunately if you want to use Visual C++'s otherwise excellent
// leak detector it's not that easy, as it will wrongly flag data that
// is created before the ``main`` is called.
//
// GlobalVar works in conjunction with the LP3_MAIN macro to get
// around this problem by storing a pointer to global data which is
// then deleted right before ``main`` exits, allowing you to use the
// leak detector without triggering false negatives.
// --------------------------------------------------------------------
template<typename T>
class GlobalVar {
public:
    template<typename... Params>
    GlobalVar(Params&&... args)
    :   ptr(new T(std::forward<Params>(args)...))
    {
		on_exit_clean_up(new GR{ ptr });
	}

    ~GlobalVar() {
		// Handled elsewhere.
    }

    T & get() {
        return *ptr;
    }

private:
	T * ptr;

	struct GR : public GlobalResource {
		T * ptr;

		GR(T * _ptr)
		: ptr(_ptr)
		{}

		GR(GR && other)
		{
			std::swap(this->ptr, other.ptr);
		}

		~GR() override {
			if (ptr) {
				delete ptr;
			}
		}
	};
};
// ~end-doc

}   }

#endif
