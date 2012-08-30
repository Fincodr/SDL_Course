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

#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <SDL.h>
#include <iostream>
#include "Singleton.hpp"
#include "ResourceFactory.hpp"

namespace DemoEngine {

    using std::cerr;
    using std::endl;

    class CMutex
    {
        public:
            CMutex() {
                #ifdef DEBUGCTORS
                cerr << "CMutex ctor called" << endl;
                #endif
                Initialize();
            }
            virtual ~CMutex() {
                #ifdef DEBUGCTORS
                cerr << "CMutex dtor called" << endl;
                #endif
                Free();
            }
            void Lock() {
                SDL_LockMutex( m_pMutex );
            }
            void Release() {
                SDL_UnlockMutex( m_pMutex );
            }
            CMutex(const CMutex& other)=delete;             // Because we have pointer datamembers this class can't be automatically copied correctly.
            CMutex& operator=(const CMutex& other)=delete;  // Because we have pointer datamembers this class can't be automatically copied correctly.
        protected:
            void Initialize() {
                if ( !m_pMutex )
                {
                    m_pMutex = SDL_CreateMutex();
                }
            }
            void Free() {
                if ( m_pMutex ) {
                    SDL_DestroyMutex( m_pMutex );
                    m_pMutex = nullptr;
                }
            }
        private:
            SDL_mutex* m_pMutex = nullptr;
    };

    typedef CSingleton<CMultithreadedResourceFactory<int, CMutex>> MutexFactory;
}

#endif // MUTEX_HPP
