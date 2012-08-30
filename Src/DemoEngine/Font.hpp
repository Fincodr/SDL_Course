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

#ifndef FONT_HPP
#define FONT_HPP

#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>
#include <SDL_ttf.h>
#include "ImageAlpha.hpp"
#include "Vector2.hpp"
#include "TwoDimensional.hpp"
#include "Positional.hpp"

namespace DemoEngine {

    using std::cout;
    using std::endl;
    using std::runtime_error;
    using std::string;
    using std::make_shared;

    class CFont
    {
        public:
            CFont() throw( runtime_error ) {
                if ( !TTF_WasInit() )
                {
                    if ( TTF_Init() < 0 ) {
                        throw runtime_error( string("TTF_Init: ") + string( TTF_GetError() ) );
                    }
                }
            }
            virtual ~CFont() {
                CleanUp();
            }
            void CleanUp()
            {
                if ( m_pFont ) {
                    TTF_CloseFont(m_pFont);
                    m_pFont = nullptr;
                }
            }
            void Load( const char *szPath, int nFontSize ) throw( runtime_error ) {
                #ifdef DEBUG
                cout << "CFont::Load( \"" << szPath << "\" )" << endl;
                #endif
                m_pFont = TTF_OpenFont(szPath, nFontSize);
                if ( m_pFont == nullptr ) throw runtime_error("Load font error: " + string( TTF_GetError() ) );
            }
            TTF_Font* GetFontPointer() const { return ( m_pFont ); }
            CFont(const CFont& other)=delete;             // Because we have pointer datamembers this class can't be automatically copied correctly.
            CFont& operator=(const CFont& other)=delete;  // Because we have pointer datamembers this class can't be automatically copied correctly.
        protected:
        private:
            TTF_Font* m_pFont = nullptr;
    };

    typedef CSingleton<CResourceFactory<int, CFont>> FontFactory;

}


#endif // FONT_HPP
