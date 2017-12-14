#define LP3_MIX_API_CREATE
#include <lp3/mix.hpp>

namespace lp3 { namespace mix {

namespace {
	static int init_count = 0;
	static int mixer_ctor_count = 0;
}

LP3_MIX_API
Init::Init(const int flags) {
	LP3_ASSERT(0 == init_count);
	const int result = Mix_Init(flags);
	if (flags != (result&flags)) {
		LP3_LOG_ERROR("One or more flags invalid!");
		LP3_LOG_ERROR("SDL error: %s", SDL_GetError());
		LP3_THROW2(lp3::core::Exception, "Error initializing SDL Mixer.");
	}	
	++init_count;
}

LP3_MIX_API
Init::~Init() {
	Mix_Quit();
	--init_count;
}

LP3_MIX_API
Mixer::Mixer(const int frequency, const std::uint16_t format, 
	         const int channels, const int chunk_size) {
	LP3_ASSERT(0 == mixer_ctor_count);
	++ mixer_ctor_count;
	const int result = Mix_OpenAudio(frequency, format, channels, chunk_size);
	if (0 != result) {
		LP3_LOG_ERROR("Mix_OpenAudio error: %s", Mix_GetError());
		LP3_THROW2(lp3::core::Exception, "Mix_OpenAudio error.");
	}
}

LP3_MIX_API
Mixer::~Mixer() {
	Mix_CloseAudio();
	--mixer_ctor_count;
}

LP3_MIX_API
Chunk::Chunk()
:   _chunk(nullptr)
{}

LP3_MIX_API
Chunk::Chunk(gsl::owner<Mix_Chunk *> chunk)
:   _chunk(chunk)
{
	if (nullptr == _chunk) {
		LP3_LOG_ERROR("Chunk creation failed: %s", Mix_GetError());
		LP3_THROW(lp3::core::Exception);
	}
    SDL_assert(nullptr != _chunk);
}

LP3_MIX_API
Chunk::~Chunk() {
    if (nullptr != _chunk) {
        Mix_FreeChunk(_chunk);
    }
    _chunk = nullptr;
}

LP3_MIX_API
Chunk::Chunk(Chunk && rhs)
: _chunk(rhs._chunk)
{
    rhs._chunk = nullptr;
}

LP3_MIX_API
Chunk & Chunk::operator=(Chunk && rvalue) {
    SDL_assert(nullptr == _chunk);
    if (this->_chunk != rvalue._chunk) {
        this->_chunk = rvalue._chunk;
        rvalue._chunk = nullptr;
    }
    return *this;
}


LP3_MIX_API
Music::Music()
:   _music(nullptr)
{}

LP3_MIX_API
Music::Music(gsl::owner<Mix_Music *> music)
:   _music(music)
{
	if (nullptr == _music) {
		LP3_LOG_ERROR("Music creation failed: %s", Mix_GetError());
		LP3_THROW(lp3::core::Exception);
	}
}

LP3_MIX_API
Music::~Music() {
    if (nullptr != _music) {
        Mix_FreeMusic(_music);
    }
    _music = nullptr;
}

LP3_MIX_API
Music::Music(Music && rhs)
: _music(rhs._music)
{
    rhs._music = nullptr;
}

LP3_MIX_API
Music & Music::operator=(Music && rvalue) {
    SDL_assert(nullptr == _music);
    if (this->_music != rvalue._music) {
        this->_music = rvalue._music;
        rvalue._music = nullptr;
    }
    return *this;
}

}   }
