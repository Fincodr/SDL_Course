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

#include "Image.hpp"
#include "UniqueID.hpp"

#include <iostream>

namespace DemoEngine {

    using std::cout;
    using std::endl;

    void CImage::SetSurface( SDL_Surface *pSurface )
    {
        if ( !m_pSurface ) {
            m_pSurface = unique_ptr<CSurface>(new CSurface);
        }
        m_pSurface->SetSurfacePointer( pSurface );
        /*
        if ( m_pSurface )
            SetBlittedArea( 0,0, m_pSurface->w, m_pSurface->h);
        */
    }

    /*
    void CImage::ScaleUniform( float factor, bool smooth )
    {
        SDL_Surface *pTmp = NULL;
        pTmp = zoomSurface( m_pSurfaceOrig, factor, factor, smooth);
        if ( pTmp )
        {
            Release();
            SetSurface(pTmp);
        }
    }

    void CImage::ScaleXY( float factorX, float factorY, bool smooth )
    {
        SDL_Surface *pTmp = NULL;
        pTmp = zoomSurface( m_pSurfaceOrig, factorX, factorY, smooth);
        if ( pTmp )
        {
            Release();
            SetSurface(pTmp);
        }
    }

    void CImage::Rotate( float degrees, bool smooth )
    {
        SDL_Surface *pTmp = NULL;
        pTmp = rotozoomSurface( m_pSurfaceOrig, degrees, 1.0, smooth );
        if ( pTmp )
        {
            Release();
            SetSurface(pTmp);
        }
    }

    void CImage::SetBlittedArea( int x, int y, int width, int height )
    {
        m_Area.x = x;
        m_Area.y = y;
        m_Area.w = width;
        m_Area.h = height;
    }

    void CImage::SetBlittedArea( const SDL_Rect & rect )
    {
        m_Area = rect;
    }

    SDL_Rect * CImage::GetBlittedArea()
    {
        return &m_Area;
    }

    void CImage::SetOriginalSurface( SDL_Surface *pSurface )
    {
        m_pSurfaceOrig = pSurface;
    }
    */

    SDL_Surface* CImage::GetSurface() const
    {
        return ( m_pSurface->GetSurfacePointer() );
    }

    CImage::CImage( const char *szFileName ) throw( runtime_error )
    {
        Load( szFileName );
    }

    void CImage::Load( const char *szFileName ) throw( runtime_error )
    {
        if ( !m_pSurface ) {
            m_pSurface = unique_ptr<CSurface>(new CSurface);
        }
        #ifdef DEBUG
        cout << "CImage::Load( \"" << szFileName << "\" )" << endl;
        #endif
        auto pSurface = IMG_Load( szFileName );
        if ( pSurface == NULL ) {
            throw ( runtime_error( "CImage::Cannot load image: " + std::string(szFileName) ) );
        }
        SDL_SetAlpha(pSurface, 0, 0);
        m_pSurface->SetSurfacePointer( pSurface );
    }

    /*
    void CImage::CreateEmptySurface( int width, int height, int bits ) throw( runtime_error )
    {
        Release();
        //ReleaseOrig();

        // Create a 32-bit surface with the bytes of each pixel in R,G,B,A order,
        // as expected by OpenGL for textures
        Uint32 rmask, gmask, bmask, amask;

        // SDL interprets each pixel as a 32-bit number, so our masks must depend
        // on the endianness (byte order) of the machine
        #if SDL_BYTEORDER == SDL_BIG_ENDIAN
            rmask = 0xff000000;
            gmask = 0x00ff0000;
            bmask = 0x0000ff00;
            amask = 0x000000ff;
        #else
            rmask = 0x000000ff;
            gmask = 0x0000ff00;
            bmask = 0x00ff0000;
            amask = 0xff000000;
        #endif

        m_pSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, bits, rmask, gmask, bmask, amask);

        if ( m_pSurface == NULL ) {
            throw ( runtime_error( "CImage::Cannot create empty surface: " + std::string(SDL_GetError()) ) );
        }

        //m_pSurfaceOrig = m_pSurface;
    }
    */

}

