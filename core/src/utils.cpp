#define LP3_CORE_API_CREATE
#include <lp3/core/utils.hpp>
#include <lp3/assert.hpp>
#include <lp3/log.hpp>

#ifdef LP3_COMPILE_TARGET_PC
    #include <cstdlib>  // getenv
#endif
#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <kos.h>
#endif

namespace lp3 { namespace core {

namespace {
    #ifdef LP3_COMPILE_TARGET_DREAMCAST
        bool start_is_pressed() {
            maple_device_t * cont = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);
            if (!cont) {
                LP3_LOG_ERROR("Error getting controller.");
                return false;
            }

            /* Check for start on the controller */
            cont_state_t *state = (cont_state_t *) maple_dev_status(cont);
            if (!state) {
                LP3_LOG_ERROR("Error getting controller status");
                return false;
            }

            return (state->buttons & CONT_START);
        }
    #endif
}  // end anon namespace

LP3_CORE_API void prompt_to_continue_on_dreamcast() {
    #ifdef LP3_COMPILE_TARGET_DREAMCAST
        LP3_LOG_DEBUG("Press START on the controller to continue!");
        while(!start_is_pressed()) {
            // zzz
        }
    #endif
}


std::optional<std::string> get_env_var(const gsl::cstring_span<> & name) {
#ifdef LP3_COMPILE_TARGET_WINDOWS
    char * env_var_value;
    std::size_t length;
    auto result = _dupenv_s(&env_var_value, &length, name.data());
    LP3_ASSERT(0 == result); // Error calling _dupenv_s
    std::unique_ptr<char> delete_env_var(env_var_value);

#elif defined(LP3_COMPILE_TARGET_EMSCRIPTEN)
    const char * env_var_value = nullptr;
#else
    const char * const env_var_value = getenv(name.data());
#endif
    if (env_var_value) {
        return std::string{env_var_value};
    } else {
        return std::nullopt;
    }
}

} }


