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

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>       // for unique_ptr
#include <stdexcept>
#include <string>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include <SDL_image.h>
#include "Initializable.hpp"
#include "Image.hpp"
#include "ImageColorkey.hpp"
#include "ImageAlpha.hpp"
#include "Text.hpp"
#include "Pixel.hpp"
#include "Circle.hpp"
#include "Ellipse.hpp"
#include "Rectangle.hpp"
#include "LineSegment.hpp"

// Its good idea to use own namespace
namespace DemoEngine {

    // Import only what we need
    using std::unique_ptr;
    using std::runtime_error;
    using std::string;

    class CRenderer : public CInitializable
    {
        public:
            // Public API
            void Init() throw(runtime_error);
            void CleanUp() throw(runtime_error);
            void OpenWindow( const int width, const int height, const int bpp, const Uint32 flags ) throw(runtime_error);

            void Begin() const;
            void End() const;

            // Render function (using CImage class)
            void Render( CImage *pImage, const int x, const int y, SDL_Rect* rect = nullptr ) const;
            void Render( CImageColorkey *pImage, const int x, const int y, SDL_Rect* rect = nullptr ) const;
            void Render( CImageAlpha *pImage, const int x, const int y, SDL_Rect* rect = nullptr ) const;
            // Smart pointer versions
            void Render( unique_ptr<CText>& pText, const int x, const int y ) const;
            void Render( unique_ptr<CImage>& pImage, const int x, const int y, SDL_Rect* rect = nullptr ) const;
            void Render( unique_ptr<CImageColorkey>& pImage, const int x, const int y, SDL_Rect* rect = nullptr ) const;
            void Render( unique_ptr<CImageAlpha>& pImage, const int x, const int y, SDL_Rect* rect = nullptr ) const;

            // Additional render functions
            void Render( CCircle *pCircle );
            void Render( CEllipse *pEllipse );
            void Render( CRectangle *pRect );
            void Render( CLineSegment *pLine );
            // Smart pointer versions
            void Render( unique_ptr<CCircle>& pCircle );
            void Render( unique_ptr<CEllipse>& pEllipse );
            void Render( unique_ptr<CRectangle>& pRect );
            void Render( unique_ptr<CLineSegment>& pLine );

            void ClearScreen() const;
            void SetClearColor(Uint8 r, Uint8 g, Uint8 b);
            SDL_Surface* GetScreen() const;

            CRenderer();
            virtual ~CRenderer();

            void Initialize() override;

            CRenderer(const CRenderer& other)=delete;             // Because we have pointer datamembers this class can't be automatically copied correctly.
            CRenderer& operator=(const CRenderer& other)=delete;  // Because we have pointer datamembers this class can't be automatically copied correctly.
        protected:
            // C++11 allows initializing here (for older compilers you can initialize at the ctor)
            SDL_Surface* m_pScreen = nullptr;
            Uint32 m_ClearColor = 0;
    };

}

#endif // RENDERER_HPP
