#include <lp3/core.hpp>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

namespace core = lp3::core;

TEST_CASE("Capture fail assert", "[capture-fail-assert]") {
	auto something_that_fails = []() {
		LP3_ASSERT(false);
	};
	core::AssertListener listener;
    REQUIRE_THROWS_AS(something_that_fails(), lp3::core::AssertFailed);
}
