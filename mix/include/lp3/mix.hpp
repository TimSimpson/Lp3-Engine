#ifndef LP3_MIX_HPP
#define LP3_MIX_HPP
#pragma once

#include "mix/config.hpp"
#include <lp3/core.hpp>
#ifndef LP3_COMPILE_TARGET_EMSCRIPTEN
#include <SDL_mixer.h>
#else
#include <SDL/SDL_mixer.h>
#endif

namespace lp3 { namespace mix {

// Calls Mix_Init on construction, Mix_Quit on destruction.
// Only one instance of this should exist at a time.
LP3_MIX_API
class Init {
public:
	Init(const int flags);
	~Init();
};

// Calls Mix_OpenAudio on construction, Mix_CloseAudio on destruction.
// Only one of these should exist at a time.
LP3_MIX_API
class Mixer {
public:
	Mixer(const int frequency, const std::uint16_t format, const int channels, 
		  const int chunk_size);
	~Mixer();
};

LP3_MIX_API
class Chunk {
public:
    explicit Chunk();

    Chunk(gsl::owner<Mix_Chunk *> chunk);

    ~Chunk();

    Chunk(Chunk && rhs);

    Chunk & operator=(Chunk && rvalue);

    // No copy ctor
    Chunk & operator=(const Chunk & other) = delete;

    // Implicitly convert to Mix_Chunk
    inline operator Mix_Chunk * () {
        SDL_assert(nullptr != _chunk);
        return _chunk;
    }

	inline int volume() {
		return Mix_VolumeChunk(_chunk, -1);
	}

	inline int volume(const int volume) {
		LP3_ASSERT(volume >= 0);
		LP3_ASSERT(volume <= MIX_MAX_VOLUME);
		return Mix_VolumeChunk(_chunk, volume);
	}

private:
    gsl::owner<Mix_Chunk *> _chunk;
};

LP3_MIX_API
class Music {
public:
    explicit Music();

    Music(gsl::owner<Mix_Music *> music);
	
    ~Music();

    Music(Music && rhs);

    Music & operator=(Music && rvalue);

    // No copy ctor
    Music & operator=(const Music & other) = delete;

    // Implicitly convert to Mix_Music
    inline operator Mix_Music * () {
        SDL_assert(nullptr != _music);
        return _music;
    }

	inline void fade_in(const int milliseconds, const int loops = -1) {
		const int result = Mix_FadeInMusic(_music, loops, milliseconds);
		LP3_ASSERT(0 == result);
	}

	inline static bool is_playing() {
		return 0 != Mix_PlayingMusic();
	}
	inline void play(const int loops = -1) {
		const int result = Mix_PlayMusic(_music, loops);
		LP3_ASSERT(0 == result);
	}

	inline void stop() {
		const int result = Mix_PlayMusic(_music, 0);
		LP3_ASSERT(0 == result);
	}
private:
    gsl::owner<Mix_Music *> _music;
};

}   }

#endif
