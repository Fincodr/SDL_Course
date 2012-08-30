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

#ifndef MUSIC_HPP
#define MUSIC_HPP

#include <stdexcept>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

namespace DemoEngine {

    using std::runtime_error;
    using std::string;

    class CMusic
    {
        public:
            CMusic() {}

            virtual ~CMusic() {
                Mix_FreeMusic( m_pMusic );
                m_pMusic = nullptr;
            }

            void Load( const char* szSoundFile ) throw(runtime_error)
            {
                #ifdef DEBUG
                cout << "CMusic::Load( \"" << szSoundFile << "\" )" << endl;
                #endif
                Mix_Music* pMusic = Mix_LoadMUS( szSoundFile );
                if ( !pMusic )
                    throw runtime_error( string("Cannot load music: ") + string(szSoundFile) );

                if ( m_pMusic ) Mix_FreeMusic( m_pMusic );
                m_pMusic = pMusic;
            }

            Mix_Music* GetMusic() { return m_pMusic; }

            CMusic(const CMusic& other)=delete;             // Because we have pointer datamembers this class can't be automatically copied correctly.
            CMusic& operator=(const CMusic& other)=delete;  // Because we have pointer datamembers this class can't be automatically copied correctly.
        protected:
            Mix_Music* m_pMusic = nullptr;
        private:
    };

    typedef CSingleton<CResourceFactory<int, CMusic>> MusicFactory;

}

#endif // MUSIC_HPP
