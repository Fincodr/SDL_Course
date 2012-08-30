/*
 *     _/_/_/_/  _/                                      _/
 *    _/            _/_/_/      _/_/_/    _/_/      _/_/_/  _/  _/_/
 *   _/_/_/    _/  _/    _/  _/        _/    _/  _/    _/  _/_/
 *  _/        _/  _/    _/  _/        _/    _/  _/    _/  _/
 * _/        _/  _/    _/    _/_/_/    _/_/      _/_/_/  _/
 *
 * Copyright (c) 2012 Mika Luoma-aho <fincodr@mxl.fi>
 *
 * This source code and software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the use of this source code or software.
 *
 * Permission is granted to anyone to use this software (and the source code when its released from the author)
 * as a learning point to create games, including commercial applications.
 *
 * You are however subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software.
 *    If you use this software's source code in a product,
 *    an acknowledgment in the product documentation would be appreciated but is not required.
 * 2. Altered versions must be plainly marked as such, and must not be misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any distribution.
 *
 */

#ifndef SOUNDSERVER_HPP
#define SOUNDSERVER_HPP

#include <SDL.h>
#include <SDL_mixer.h>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <memory>
#include "Singleton.hpp"
#include "ResourceFactory.hpp"
#include "Sound.hpp"
#include "Music.hpp"

namespace DemoEngine {

    using std::runtime_error;

    class CSoundServer
    {
        public:
            CSoundServer() {};

            ~CSoundServer()
            {
                Destroy();
            }

            void Init( int freq = MIX_DEFAULT_FREQUENCY, Uint16 format = MIX_DEFAULT_FORMAT, int channels = 2, int chunksize = 1024) throw(runtime_error)
            {
                if ( !SDL_WasInit( SDL_INIT_AUDIO ) )
                    if ( SDL_InitSubSystem( SDL_INIT_AUDIO ) == -1 )
                        throw runtime_error( "Init error :" + string(SDL_GetError()) );
                if ( Mix_OpenAudio(freq, format, channels, chunksize ) == -1 )
                    throw runtime_error( "OpenAudio error :" + string(Mix_GetError()) );
            }

            void Destroy()
            {
                // Close resources
                Mix_CloseAudio();
                Mix_Quit();
                SDL_QuitSubSystem( SDL_INIT_AUDIO );
            }

            int Play( const int resourceID, int channel = -1 )
            {
                return PlayLooped( resourceID, 0, channel );
            }

            int PlayLooped( const int resourceID, int loops, int channel = -1 )
            {
                return PlayLoopedTimed( resourceID, loops, 0, channel );
            }

            int PlayLoopedTimed( const int resourceID, int loops, int ms, int channel = -1 )
            {
                auto& sound = SoundFactory::Instance()->Get( resourceID );
                if ( sound != nullptr )
                    return Mix_PlayChannelTimed(channel, sound->GetChunk(), loops, ms);
                return -1;
            }

            void Halt( int channel = -1 )
            {
                Mix_HaltChannel( channel );
            }

            void Pause( int channel = -1 )
            {
                Mix_Pause( channel );
            }

            void Resume( int channel = -1 )
            {
                Mix_Resume(channel);
            }

            void PlayMusic( const int resourceID )
            {
                auto& music = MusicFactory::Instance()->Get( resourceID );
                if ( music != nullptr )
                    Mix_PlayMusic( music->GetMusic(), 0 );
            }

            void HaltMusic()
            {
                Mix_HaltMusic();
            }

            void PauseMusic()
            {
                Mix_PauseMusic();
            }

            void ResumeMusic()
            {
                Mix_ResumeMusic();
            }

            void FadeOutMusic( int ms )
            {
                Mix_FadeOutMusic( ms );
            }

            void FadeInMusic( const int resourceID, int ms )
            {
                auto& music = MusicFactory::Instance()->Get( resourceID );
                if ( music != nullptr )
                    Mix_FadeInMusic( music->GetMusic(), -1, ms );
            }

            bool IsPaused( int channel )
            {
                return Mix_Paused( channel );
            }

            bool IsPlaying( int channel )
            {
                return Mix_Playing(channel);
            }

            bool IsFading( int channel )
            {
                return ( Mix_FadingChannel( channel ) != MIX_NO_FADING );
            }

            int FadeInLooped( const int resourceID, int loops, int ms, int channel )
            {
                auto& sound = SoundFactory::Instance()->Get( resourceID );
                if ( sound != nullptr )
                    return Mix_FadeInChannel( channel, sound->GetChunk(), loops, ms );
                return -1;
            }

            void FadeOutLooped( int channel, int ms )
            {
                Mix_FadeOutChannel( channel, ms );
            }

        private:
    };

}

#endif // SOUNDSERVER_HPP
