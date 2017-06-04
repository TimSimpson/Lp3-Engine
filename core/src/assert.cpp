#include <lp3/assert.hpp>

namespace lp3 { namespace core {

namespace {
    bool throw_on_asserts = false;
}

LP3_CORE_API
AssertFailed::AssertFailed()
:   Exception("Assert failed!")
{
}

LP3_CORE_API
AssertListener::AssertListener() {
    throw_on_asserts = true;
}

LP3_CORE_API
AssertListener::~AssertListener() {
    throw_on_asserts = false;
}

LP3_CORE_API
void AssertListener::throw_assert_failed() {
    if (throw_on_asserts) {
        throw AssertFailed{};
    }
}

}   }
