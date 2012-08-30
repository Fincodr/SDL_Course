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

#include "CollisionDetector.hpp"
#include "Vector2.hpp"

namespace DemoEngine {

    bool CCollisionDetector::Collides( const shared_ptr<CImage>& imgA, const CVector2i& posA, const shared_ptr<CImage>& imgB, const CVector2i& posB, shared_ptr<CRectangle>& hitRect)
    {
        auto srcA = imgA->GetSurface();
        auto srcB = imgB->GetSurface();

        int ax = posA.m_values[0];
        int ay = posA.m_values[1];
        int aw = srcA->w;
        int ah = srcA->h;

        int bx = posB.m_values[0];
        int by = posB.m_values[1];
        int bw = srcB->w;
        int bh = srcB->h;

        // 1. Check if we can collide at all
        // Bottom-Right vs Top-Left
        if ( ax+aw < bx ) return false;
        if ( ay+ah < by ) return false;
        // Top-Left vs Bottom-Right
        if ( ax > bx+bw ) return false;
        if ( ay > by+bh ) return false;

        // 2. Since we can collide, calculate the collision position and dimensions (thanks for Anssi =D)
        int cx, cy, cw, ch;
        if ( ax < bx )
        {
            cx = bx;
            cw = ax + aw - bx;
        }
        else
        {
            cx = ax;
            cw = (bx + bw) - ax;
        }
        if ( ay < by )
        {
            cy = by;
            ch = ay + ah - by;
        }
        else
        {
            cy = ay;
            ch = by + bh - ay;
        }

        hitRect->SetPosition( cx, cy );
        hitRect->SetDimensions( cw, ch );

        // 3. Now we can finally compare the pixel data
        // Lock surfaces only if required
        if ( SDL_MUSTLOCK( srcA ) )
            SDL_LockSurface( srcA );
        if ( SDL_MUSTLOCK( srcB ) )
            SDL_LockSurface( srcB );

        // To make this as fast as possible we can use pattern with offset

        for ( int offset = 0; offset < 3; ++offset )
        {
            for( int y = cy; y < cy+ch; ++y)
            {
                for( int x = cx+((y+offset) % 3); x < cx+cw; x+=3)
                {
                    // If either of the pixels is transparent, then there is no collision
                    if ( CPixel::CheckPixel( srcA, x-ax, y-ay ) ) continue;
                    if ( CPixel::CheckPixel( srcB, x-bx, y-by ) ) continue;

                    /*
                    CPixel::SetPixel( srcA, x-ax, y-ay, SDL_MapRGBA( srcA->format, 255, 0, 0, 128 ) );
                    CPixel::SetPixel( srcB, x-bx, y-by, SDL_MapRGBA( srcB->format, 0, 255, 0, 128 ) );
                    */

                    // Both are opaque so we have an collision
                    if ( SDL_MUSTLOCK( srcA ) )
                        SDL_UnlockSurface( srcA );
                    if ( SDL_MUSTLOCK( srcB ) )
                        SDL_UnlockSurface( srcB );
                    return true;
                }
            }

        }

        if ( SDL_MUSTLOCK( srcA ) )
            SDL_UnlockSurface( srcA );
        if ( SDL_MUSTLOCK( srcB ) )
            SDL_UnlockSurface( srcB );

        return false;
    }

    bool CCollisionDetector::Collides( const shared_ptr<CCircle>& c1, const shared_ptr<CCircle>& c2 )
    {
        CVector2<int> delta = c1->GetPosition() - c2->GetPosition();
        return ( delta.Length() < (c1->m_iRadius+c2->m_iRadius) );
    }

    bool CCollisionDetector::Collides( const CRectangle& r1, const CRectangle& r2 )
    {
        // Bottom-Right vs Top-Left
        if ( r1.m_vPosition[0/*X*/]+r1.m_iWidth < r2.m_vPosition[0/*X*/] ) return false;
        if ( r1.m_vPosition[1/*Y*/]+r1.m_iHeight < r2.m_vPosition[1/*Y*/] ) return false;
        // Top-Left vs Bottom-Right
        if ( r1.m_vPosition[0/*X*/] > r2.m_vPosition[0/*X*/]+r2.m_iWidth ) return false;
        if ( r1.m_vPosition[1/*Y*/] > r2.m_vPosition[1/*Y*/]+r2.m_iHeight ) return false;

        // must collide
        return true;
    }

    bool CCollisionDetector::Collides( const shared_ptr<CRectangle>& r1, const shared_ptr<CRectangle>& r2 )
    {
        // Bottom-Right vs Top-Left
        if ( r1->m_vPosition[0/*X*/]+r1->m_iWidth < r2->m_vPosition[0/*X*/] ) return false;
        if ( r1->m_vPosition[1/*Y*/]+r1->m_iHeight < r2->m_vPosition[1/*Y*/] ) return false;
        // Top-Left vs Bottom-Right
        if ( r1->m_vPosition[0/*X*/] > r2->m_vPosition[0/*X*/]+r2->m_iWidth ) return false;
        if ( r1->m_vPosition[1/*Y*/] > r2->m_vPosition[1/*Y*/]+r2->m_iHeight ) return false;

        // must collide
        return true;
    }

    bool CCollisionDetector::Collides( const shared_ptr<CCircle>& c, const CVector2i& pos )
    {
        // YOUR CODE HERE
        CVector2<int> delta = c->GetPosition() - pos;
        return ( delta.Length() < c->m_iRadius );
    }

    bool CCollisionDetector::Collides( const shared_ptr<CRectangle>& r, const CVector2i& pos )
    {
        if ( pos.m_values[0/*X*/] < r->m_vPosition[0/*X*/] ) return false;
        if ( pos.m_values[1/*Y*/] < r->m_vPosition[1/*Y*/] ) return false;
        if ( pos.m_values[0/*X*/] > r->m_vPosition[0/*X*/]+r->m_iWidth ) return false;
        if ( pos.m_values[1/*Y*/] > r->m_vPosition[1/*Y*/]+r->m_iHeight ) return false;

        // must collide
        return true;
    }
}
