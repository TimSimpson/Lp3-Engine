// ---------------------------------------------------------------------------
// The following test shows how to read from MediaManger.
// ---------------------------------------------------------------------------
#include <gsl/gsl>
#include <lp3/core.hpp>
#include <string>
#include <vector>
#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Read file", "[read_a_file]") {
    lp3::core::MediaManager media{};
	auto story = media.load("Engine/Resources/story.txt");

    char content[16];
    std::size_t read_size = story.read(content, 1, 16);

    REQUIRE(std::size_t{16} == read_size);

    std::string contents(content, read_size);
    REQUIRE(contents == "Romulus and Remu");
}
// ~end-doc
