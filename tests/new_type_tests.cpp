#include <gsl/gsl>
#include <lp3/core.hpp>
#include <string>
#include <vector>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

namespace core = lp3::core;

LP3_NEW_TYPE(Dollars, double);

bool called = false;

void use_dollar(double d) {
	called = false;
}

void use_dollar(Dollars d) {
	called = true;
}

TEST_CASE("New Types", "[new_types]") {
    Dollars d(55.00);
	REQUIRE(!called);
	use_dollar(d);
	REQUIRE(called);
}

struct Hi {
	char hello() { return '_';  }
};

template<typename T, typename U>
class HelloNewType : lp3::core::NewType<T, U> {
public:	
	LP3_NEW_TYPE_SUBCLASS_CTOR(HelloNewType)

	char hello() { return t.hello(); }
};

LP3_NEW_TYPE_PLUS(Underscore, Hi, HelloNewType);

char called2 = '0';

void hi_there(Hi hi) {
	hi.hello();
	called2 = '0';
}

void hi_there(Underscore u) {
	called2 = u.hello();
}

TEST_CASE("New Types 2", "[new_types_2]") {
	Underscore u();
	REQUIRE(called2 == '0');
	hi_there(u);
	REQUIRE(called2 == '_');
}
