
#include "Text.hpp"

namespace DemoEngine {

    CText::CText()
    {
        SetColor( 255,255,255,255 );
    }

    CText::~CText()
    {
    }

    SDL_Surface* CText::GetSurface() const
    {
        return ( m_pImage->GetSurface() );
    }

    void CText::Create( unique_ptr<CFont>& font, const char *szText, const int r, const int g, const int b, const int a ) throw( runtime_error )
    {
        #ifdef DEBUG
        cout << "CText::Create( \"" << szText << "\" )" << endl;
        #endif
        if ( font == nullptr ) throw std::runtime_error("Font not loaded!");
        SetColor( r, g, b, a );
        m_pImage = unique_ptr<CImageAlpha>(new CImageAlpha());
        auto surface = TTF_RenderText_Blended( font->GetFontPointer(), szText, GetColor());
        m_pImage->SetSurface( surface );
        m_pImage->SetAlpha(10);
    }


}

