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

#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <iostream>
#include <SDL_image.h>
#include "UniqueID.hpp"

namespace DemoEngine {

    using std::cout;
    using std::endl;

    // Simple SDL_Surface wrapper to enable automatic SDL_FreeSurface
    class CSurface
    {
        public:
            CSurface() : m_ObjectID( CSingleton<CUniqueID>::Instance()->getID() ) {
                //this->m_ObjectID = CSingleton<CUniqueID>::Instance()->getID();
                #ifdef DEBUGCTORS
                cout << "Surface ctor called (id: " << this->m_ObjectID << ")" << endl;
                #endif
            }
            virtual ~CSurface() {
                #ifdef DEBUGCTORS
                cout << "Surface dtor called (id:" << this->m_ObjectID << ")" << endl;
                #endif
                Free();
            }
            SDL_Surface* GetSurfacePointer() { return m_pSurface; }
            void SetSurfacePointer( SDL_Surface* pSurface ) {
                Free();
                m_pSurface = pSurface;
            }
            void Free() {
                if ( m_pSurface != nullptr )
                {
                    SDL_FreeSurface( m_pSurface );
                }
                m_pSurface = nullptr;
            }
            CSurface(const CSurface& other)=delete;             // Because we have pointer datamembers this class can't be automatically copied correctly.
            CSurface& operator=(const CSurface& other)=delete;  // Because we have pointer datamembers this class can't be automatically copied correctly.
        protected:
        private:
            SDL_Surface* m_pSurface = nullptr;
            int m_ObjectID;
    };

}

#endif // SURFACE_HPP
