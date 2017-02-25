#include <lp3/test_asserts.hpp>

#if defined(LP3_COMPILE_TARGET_DREAMCAST) || defined(LP3_COMPILE_TARGET_EMSCRIPTEN)


namespace lp3 { namespace asserts {

jmp_buf buf;
const char * lastExceptionType;

} }

#endif
