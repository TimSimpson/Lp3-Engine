#define LP3_TEST_MODULE media_tests
#include <gsl/gsl>
#include <lp3/core.hpp>
#include <string>
#include <vector>
#define CATCH_CONFIG_MAIN
#include <catch.hpp>

namespace core = lp3::core;

TEST_CASE("Read file", "[read_a_file]") {
    core::MediaManager media{};
    std::unique_ptr<core::InputStream> story(
        media.load("Engine/Resources/story.txt"));

    char content[16];
    gsl::span<char> content_span{ content };
    std::size_t read_size = story->read(content_span);

    REQUIRE(std::size_t{16} == read_size);

    std::string contents(content, read_size);
    REQUIRE(contents == "Romulus and Remu");
}
