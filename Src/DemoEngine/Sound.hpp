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

#ifndef SOUND_HPP
#define SOUND_HPP

#include <stdexcept>
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

namespace DemoEngine {

    using std::runtime_error;
    using std::string;

    class CSound
    {
        public:
            CSound() {}
            virtual ~CSound() {
                Mix_FreeChunk( m_pSound );
                m_pSound = nullptr;
            }

            void Load( const char* szSoundFile ) throw(runtime_error)
            {
                #ifdef DEBUG
                cout << "CSound::Load( \"" << szSoundFile << "\" )" << endl;
                #endif
                Mix_Chunk *pSound = Mix_LoadWAV( szSoundFile );
                if ( !pSound )
                    throw runtime_error( string("Cannot load sample: ") + string(szSoundFile) );

                if ( m_pSound ) Mix_FreeChunk( m_pSound );
                m_pSound = pSound;
            }

            Mix_Chunk* GetChunk() { return m_pSound; }

            CSound(const CSound& other)=delete;             // Because we have pointer datamembers this class can't be automatically copied correctly.
            CSound& operator=(const CSound& other)=delete;  // Because we have pointer datamembers this class can't be automatically copied correctly.
        protected:
            Mix_Chunk* m_pSound = nullptr;
        private:
    };

    typedef CSingleton<CResourceFactory<int, CSound>> SoundFactory;

}

#endif // SOUND_HPP
