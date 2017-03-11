#define LP3_TEST_MODULE media_tests
#include <lp3/test.hpp>
#include <lp3/assert.hpp>

#include <gsl/gsl>
#include <lp3/core.hpp>
#include <string>
#include <vector>


namespace core = lp3::core;


LP3_TEST(testOne)
{
    core::MediaManager media{};
    gsl::owner<core::InputStream *> story =
        media.load("Engine/Resources/story.txt");

    char content[16];
    gsl::span<char> content_span{ content };
    std::size_t read_size = story->read(content_span);

    LP3_ASSERT_EQUAL(std::size_t{16}, read_size);

    std::string contents(content, read_size);
    LP3_ASSERT_EQUAL(contents, "Romulus and Remu");
}
