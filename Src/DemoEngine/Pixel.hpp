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

#ifndef PIXEL_HPP
#define PIXEL_HPP

#include <SDL.h>

class CPixel
{
    public:
        CPixel() {}
        virtual ~CPixel() {}

        // http://www.ohjelmointiputka.net/keskustelu/aihe.php?id=20263&sivu=1
        // Optimized by me :)
        static inline Uint32 GetPixel( SDL_Surface* surface, int x, int y )
        {
            int bpp = surface->format->BytesPerPixel;
            /* Here p is the address to the pixel we want to retrieve */
            Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

            switch (bpp) {
            case 1:
                return *p;

            case 2:
                return *(Uint16 *)p;

            case 3:
                /* get 24-bit pixel */
                #if SDL_BYTEORDER - SDL_LIL_ENDIAN
                    /* big endian */
                    return p[0] << 16 | p[1] << 8 | p[2];
                #else
                    /* little endian */
                    return p[0] | p[1] << 8 | p[2] << 16;
                #endif

            case 4:
                return *(Uint32 *)p;

            default:
                return 0;       /* shouldn't happen, but avoids warnings */
            } // switch
        }

        // http://www.ohjelmointiputka.net/keskustelu/aihe.php?id=19691&sivu=1
        // Optimized by me :)
        static inline void SetPixel( SDL_Surface* surface, int x, int y, Uint32 pixel )
        {
            int bpp = surface->format->BytesPerPixel;
            /* Here p is the address to the pixel we want to set */
            Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

            switch(bpp) {
            case 1:
                *p = pixel;
                break;

            case 2:
                *(Uint16 *)p = pixel;
                break;

            case 3:
                /* get 24-bit pixel */
                #if SDL_BYTEORDER - SDL_LIL_ENDIAN
                    /* big endian */
                    p[0] = (pixel >> 16) & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = pixel & 0xff;
                #else
                    /* little endian */
                    p[0] = pixel & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = (pixel >> 16) & 0xff;
                #endif
                break;

            case 4:
                *(Uint32 *)p = pixel;
                break;

            default:
                break;       /* shouldn't happen, but avoids warnings */
            }
        }

        static inline bool CheckPixel( SDL_Surface* surface, int x, int y )
        {
            Uint32 p = GetPixel( surface, x, y );
            if ( ( surface->flags & SDL_SRCCOLORKEY ) && ( p == surface->format->colorkey ) )
                return true;
            if ( ( surface->flags & SDL_SRCALPHA ) && ( p & surface->format->Amask ) == 0 )
                return true;
            return false;
        }

    protected:
    private:
};

#endif // PIXEL_HPP
