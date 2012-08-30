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

#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>
#include "Singleton.hpp"
#include "Surface.hpp"
#include "ResourceFactory.hpp"

namespace DemoEngine {

    using std::runtime_error;

    class CImage
    {
        public:
            CImage() {};
            CImage( const char *szFileName ) throw( runtime_error );
            virtual ~CImage() {};
            virtual void Load( const char *szFileName ) throw( runtime_error );
            void SetSurface( SDL_Surface *pSurface );
            SDL_Surface* GetSurface() const;
            /*
            void SetBlittedArea( int x, int y, int width, int height );
            void SetBlittedArea( const SDL_Rect & rect );
            SDL_Rect * GetBlittedArea();
            virtual void ScaleUniform( float factor, bool smooth  );
            virtual void ScaleXY( float factorX, float factorY, bool smooth );
            virtual void Rotate( float degrees, bool smooth);
            virtual void SetOriginalSurface( SDL_Surface *pSurface );
            virtual void CreateEmptySurface( int, int, int ) throw( runtime_error );
            */

        protected:
            unique_ptr<CSurface> m_pSurface = nullptr;

        private:
            int m_iWidth = 0;
            int m_iHeight = 0;
    };

    typedef CSingleton<CResourceFactory<int, CImage>> ImageFactory;
}

#endif // IMAGE_HPP
