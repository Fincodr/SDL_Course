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

#include "Renderer.hpp"

namespace DemoEngine {

    CRenderer::CRenderer() {
        #ifdef DEBUGCTORS
        cout << "CRenderer ctor called." << endl;
        #endif
    }

    /** \brief dtor calls CleanUp
     */
    CRenderer::~CRenderer() {
        #ifdef DEBUGCTORS
        cout << "CRenderer dtor called." << endl;
        #endif
        CleanUp();
    }

    void CRenderer::Initialize() {
        if ( !IsInitialized() ) {
            Init();
        }
    }

    /** \brief Open window using SDL
     *
     * \return void
     *
     * \throw runtime_error with SDL error message if failed
     *
     */
    void CRenderer::OpenWindow( const int width,const int height, const int bpp, const Uint32 flags ) throw(runtime_error) {
        if ( !IsInitialized() ) {
            throw runtime_error( "You need to initialize SDL first" );
        }
        m_pScreen = SDL_SetVideoMode( width, height, bpp, flags );
        if ( !m_pScreen ) {
            throw std::runtime_error( std::string( SDL_GetError() ) );
        }
    }

    /** \brief Initializes SDL
     *
     * \return void
     *
     * \throw runtime_error with SDL error message if failed
     *
     */
    void CRenderer::Init() throw(runtime_error) {
        // Initialize SDL
        if ( IsInitialized() ) return;
        if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
            throw runtime_error( std::string( SDL_GetError() ) );
        }
        // Initialize SDL_image
        int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF;
        int initted = IMG_Init( flags );
        if ( (initted & flags) != flags ) {
            throw runtime_error( string( "Failed to init SDL_image jpg, png or tiff support! " ) + string(IMG_GetError()) );
        }
        #ifdef DEBUG
        cout << "CRenderer::Init() = Initialized." << endl;
        #endif
        SetInitialized( true );
    }

    /** \brief Cleans up the SDL (uninitializes SDL)
     *
     * \return void
     *
     * \throw runtime_error with SDL error message if failed
     *
     */
    void CRenderer::CleanUp() throw(runtime_error) {
        if ( !IsInitialized() ) return;
        IMG_Quit();
        SDL_Quit();
        SetInitialized( false );
    }

    /** \brief Render CText objects bitmap to screen
     *
     * \param pText CText*
     * \param x int
     * \param y int
     * \return void
     *
     */
    void CRenderer::Render( unique_ptr<CText>& pText, const int x, const int y ) const {
        Sint16 dst_x = (Sint16)x;
        Sint16 dst_y = (Sint16)y;
        SDL_Rect dst = { dst_x, dst_y, 0, 0 };
        SDL_BlitSurface( pText->GetSurface(), NULL, m_pScreen, &dst );
    }

    /** \brief Render CImage objects bitmap to screen
     *
     * \param pImage CImage*
     * \param x int
     * \param y int
     * \return void
     *
     */
    void CRenderer::Render( CImage *pImage, const int x, const int y, SDL_Rect* rect ) const {
        Sint16 dst_x = (Sint16)x;
        Sint16 dst_y = (Sint16)y;
        SDL_Rect dst = { dst_x, dst_y, 0, 0 };
        SDL_BlitSurface( pImage->GetSurface(), rect, m_pScreen, &dst );
    }
    void CRenderer::Render( unique_ptr<CImage>& pImage, const int x, const int y, SDL_Rect* rect ) const {
        Sint16 dst_x = (Sint16)x;
        Sint16 dst_y = (Sint16)y;
        SDL_Rect dst = { dst_x, dst_y, 0, 0 };
        SDL_BlitSurface( pImage->GetSurface(), rect, m_pScreen, &dst );
    }

    /** \brief Render CImageColorkey objects bitmap to screen
     *
     * \param pImage CImageColorkey*
     * \param x const int
     * \param y const int
     * \return void
     *
     */
    void CRenderer::Render( CImageColorkey *pImage, const int x, const int y, SDL_Rect* rect ) const {
        Sint16 dst_x = (Sint16)x;
        Sint16 dst_y = (Sint16)y;
        SDL_Rect dst = { dst_x, dst_y, 0, 0 };
        SDL_SetColorKey( pImage->GetSurface(), SDL_SRCCOLORKEY, pImage->GetColorkey() );
        SDL_BlitSurface( pImage->GetSurface(), rect, m_pScreen, &dst );
    }
    void CRenderer::Render( unique_ptr<CImageColorkey>& pImage, const int x, const int y, SDL_Rect* rect ) const {
        Sint16 dst_x = (Sint16)x;
        Sint16 dst_y = (Sint16)y;
        SDL_Rect dst = { dst_x, dst_y, 0, 0 };
        SDL_SetColorKey( pImage->GetSurface(), SDL_SRCCOLORKEY, pImage->GetColorkey() );
        SDL_BlitSurface( pImage->GetSurface(), rect, m_pScreen, &dst );
    }

    /** \brief Render CImageAlpha objects bitmap to screen
     *
     * \param pImage CImageAlpha*
     * \param x const int
     * \param y const int
     * \return void
     *
     */
    void CRenderer::Render( CImageAlpha *pImage, const int x, const int y, SDL_Rect* rect ) const {
        Sint16 dst_x = (Sint16)x;
        Sint16 dst_y = (Sint16)y;
        SDL_Rect dst = { dst_x, dst_y, 0, 0 };
        SDL_SetAlpha( pImage->GetSurface(), ( pImage->bIsTransparent() ? SDL_SRCALPHA : 0 ), pImage->GetAlpha() );
        SDL_BlitSurface( pImage->GetSurface(), rect, m_pScreen, &dst );
    }
    void CRenderer::Render( unique_ptr<CImageAlpha>& pImage, const int x, const int y, SDL_Rect* rect ) const {
        Sint16 dst_x = (Sint16)x;
        Sint16 dst_y = (Sint16)y;
        SDL_Rect dst = { dst_x, dst_y, 0, 0 };
        SDL_SetAlpha( pImage->GetSurface(), ( pImage->bIsTransparent() ? SDL_SRCALPHA : 0 ), pImage->GetAlpha() );
        SDL_BlitSurface( pImage->GetSurface(), rect, m_pScreen, &dst );
    }


    /** \brief Begin is called before starting to draw to screen
     *
     * \return void
     *
     */
    void CRenderer::Begin() const {
        /*
        if ( m_pScreen != nullptr ) {
            SDL_FillRect( m_pScreen, NULL, m_ClearColor );
        }
        */
    }

    /** \brief End is called when drawing has been ended and will display the buffer to screen
     *
     * \return void
     *
     */
    void CRenderer::End() const {
        if ( m_pScreen != nullptr ) {
            SDL_Flip( m_pScreen );
        }
    }

    void CRenderer::SetClearColor(Uint8 r, Uint8 g, Uint8 b)
    {
        if ( m_pScreen != nullptr )
            m_ClearColor = SDL_MapRGB( m_pScreen->format, r, g, b );
    }

    void CRenderer::ClearScreen() const
    {
        if ( m_pScreen != nullptr )
            SDL_FillRect( m_pScreen, NULL, m_ClearColor );
    }

    SDL_Surface* CRenderer::GetScreen() const
    {
        return ( m_pScreen );
    }

    void CRenderer::Render( CCircle *pCircle )
    {
        int x = static_cast<int>( pCircle->GetX() );
        int y = static_cast<int>( pCircle->GetY() );
        int radius = static_cast<int>( pCircle->GetRadius() );
        SDL_Color & c = pCircle->GetColor();
        if ( pCircle->IsFilled() )
            filledCircleRGBA( GetScreen(), x, y, radius, c.r, c.g, c.b, c.unused );
        else
            circleRGBA( GetScreen(), x, y, radius, c.r, c.g, c.b, c.unused );
    }
    void CRenderer::Render( unique_ptr<CCircle>& pCircle )
    {
        int x = static_cast<int>( pCircle->GetX() );
        int y = static_cast<int>( pCircle->GetY() );
        int radius = static_cast<int>( pCircle->GetRadius() );
        SDL_Color & c = pCircle->GetColor();
        if ( pCircle->IsFilled() )
            filledCircleRGBA( GetScreen(), x, y, radius, c.r, c.g, c.b, c.unused );
        else
            circleRGBA( GetScreen(), x, y, radius, c.r, c.g, c.b, c.unused );
    }

    void CRenderer::Render( CEllipse *pEllipse)
    {
        int x = static_cast<int>( pEllipse->GetX() );
        int y = static_cast<int>( pEllipse->GetY() );
        int w = static_cast<int>( pEllipse->GetWidth() );
        int h = static_cast<int>( pEllipse->GetHeight() );
        SDL_Color & c = pEllipse->GetColor();
        if ( pEllipse->IsFilled() )
            filledEllipseRGBA( GetScreen(), x, y, w , h, c.r, c.g, c.b, c.unused );
        else
            ellipseRGBA( GetScreen(), x, y, w ,h, c.r, c.g, c.b, c.unused  );
    }
    void CRenderer::Render( unique_ptr<CEllipse>& pEllipse)
    {
        int x = static_cast<int>( pEllipse->GetX() );
        int y = static_cast<int>( pEllipse->GetY() );
        int w = static_cast<int>( pEllipse->GetWidth() );
        int h = static_cast<int>( pEllipse->GetHeight() );
        SDL_Color & c = pEllipse->GetColor();
        if ( pEllipse->IsFilled() )
            filledEllipseRGBA( GetScreen(), x, y, w , h, c.r, c.g, c.b, c.unused );
        else
            ellipseRGBA( GetScreen(), x, y, w ,h, c.r, c.g, c.b, c.unused  );
    }

    void CRenderer::Render( CRectangle *pRect )
    {
        int x = static_cast<int>( pRect->GetX() );
        int y = static_cast<int>( pRect->GetY() );
        int x2 = x + static_cast<int>( pRect->GetWidth() );
        int y2 = y + static_cast<int>( pRect->GetHeight() );
        SDL_Color & c = pRect->GetColor();
        if ( pRect->IsFilled() )
            boxRGBA( GetScreen(), x, y, x2, y2, c.r, c.g, c.b, c.unused );
        else
            rectangleRGBA( GetScreen(), x, y, x2, y2, c.r, c.g, c.b, c.unused );
    }
    void CRenderer::Render( unique_ptr<CRectangle>& pRect )
    {
        int x = static_cast<int>( pRect->GetX() );
        int y = static_cast<int>( pRect->GetY() );
        int x2 = x + static_cast<int>( pRect->GetWidth() );
        int y2 = y + static_cast<int>( pRect->GetHeight() );
        SDL_Color & c = pRect->GetColor();
        if ( pRect->IsFilled() )
            boxRGBA( GetScreen(), x, y, x2, y2, c.r, c.g, c.b, c.unused );
        else
            rectangleRGBA( GetScreen(), x, y, x2, y2, c.r, c.g, c.b, c.unused );
    }

    void CRenderer::Render( CLineSegment *pLine )
    {
        int x  = static_cast<int>( pLine->GetStart()[0] );
        int y  = static_cast<int>( pLine->GetStart()[1] );
        int x2 = static_cast<int>( pLine->GetEnd()[0] );
        int y2 = static_cast<int>( pLine->GetEnd()[1] );
        SDL_Color & c = pLine->GetColor();
        lineRGBA( GetScreen(), x, y, x2, y2, c.r, c.g, c.b, c.unused );
    }
    void CRenderer::Render( unique_ptr<CLineSegment>& pLine )
    {
        int x  = static_cast<int>( pLine->GetStart()[0] );
        int y  = static_cast<int>( pLine->GetStart()[1] );
        int x2 = static_cast<int>( pLine->GetEnd()[0] );
        int y2 = static_cast<int>( pLine->GetEnd()[1] );
        SDL_Color & c = pLine->GetColor();
        lineRGBA( GetScreen(), x, y, x2, y2, c.r, c.g, c.b, c.unused );
    }
}

