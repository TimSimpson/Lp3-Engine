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

// So you know how in C++11 and over, you can simply declare a global variable
// and know it won't have a race condition in it's initialization and will
// simply work? Ha, I remember those days too. Unfortunately if you want the
// VC++ leak detector to be worth anything to you, you'll have to use this
// from now on. Yay!?
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

}   }

#endif
