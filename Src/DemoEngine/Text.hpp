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

#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>
#include "Font.hpp"
#include "Colored.hpp"
#include "ImageAlpha.hpp"

namespace DemoEngine {

    using std::cout;
    using std::endl;
    using std::runtime_error;
    using std::string;
    using std::make_shared;

    class CText : public CColored
    {
        public:
            CText()
            {
                SetColor( 255,255,255,255 );
            }

            virtual ~CText() {};

            void Create( unique_ptr<CFont>& font, std::string text, const int r = 255, const int g = 255, const int b = 255, const int a = 255 ) throw( runtime_error )
            {
                const char *szText = text.c_str();
                #ifdef DEBUG
                cout << "CText::Create( \"" << text << "\" )" << endl;
                #endif
                if ( font == nullptr ) throw std::runtime_error("Font not loaded!");
                SetColor( r, g, b, a );
                m_pImage = unique_ptr<CImageAlpha>(new CImageAlpha());
                if ( text.length() > 0 )
                {
                    auto surface = TTF_RenderText_Blended( font->GetFontPointer(), szText, GetColor());
                    m_pImage->SetSurface( surface );
                    m_pImage->SetAlpha(10);
                }
                else
                {
                    string emptyString = " ";
                    szText = emptyString.c_str();
                    auto surface = TTF_RenderText_Blended( font->GetFontPointer(), szText, GetColor());
                    m_pImage->SetSurface( surface );
                    m_pImage->SetAlpha(10);
                    GetSurface()->w = 0;
                    GetSurface()->h = 0;
                }
            }

            SDL_Surface* GetSurface() const
            {
                return ( m_pImage->GetSurface() );
            }

        protected:
        private:
            unique_ptr<CImageAlpha> m_pImage = nullptr;
    };

    typedef CSingleton<CResourceFactory<int, CText>> TextFactory;
}

#endif // TEXT_HPP
