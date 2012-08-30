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

#ifndef ENTITYPROJECTILE_HPP
#define ENTITYPROJECTILE_HPP

#include "DemoEngine/Game.hpp"
#include "DemoEngine/ImageAlpha.hpp"
#include "DemoEngine/GameObject.hpp"

using namespace DemoEngine;

class EntityProjectile : public CGameObjectFloat
{
    public:
        EntityProjectile()
        {
            auto& renderer = CSingleton<CRenderer>::Instance();
            auto screen = renderer->GetScreen();
            m_iScreenW = screen->w;
            m_iScreenH = screen->h;

            // Get projectile image
            #ifdef DEBUGCTORS
            cout << "EntityProjectile ctor called! (id = " << GetID() << ")" << endl;
            #endif
        }

        ~EntityProjectile()
        {
            #ifdef DEBUGCTORS
            cout << "EntityProjectile dtor called! (id = " << GetID() << ")" << endl;
            #endif
        }

        void SetProjectile( int nProjectileID ) {
            m_nProjectileID = nProjectileID;
            auto& m_projectileImg = ImageAlphaFactory::Instance()->Get( m_nProjectileID );
            m_iW = m_projectileImg->GetSurface()->w;
            m_iH = m_projectileImg->GetSurface()->h;
            auto& bounds = GetBounds();
            bounds.SetDimensions( m_iW, m_iH );
            bounds.SetPosition( 0, 0 );
        }

        void Render( unique_ptr<CRenderer>& renderer ) override
        {
            if ( !IsDead() ) {
                int m_iX = GetX() - m_iW/2;
                int m_iY = GetY() - m_iH/2;
                renderer->Render( ImageAlphaFactory::Instance()->Get( m_nProjectileID ), m_iX, m_iY );
            }
        }

        void RenderDebug( unique_ptr<CRenderer>& renderer ) override
        {
            if ( !IsDead() ) {
                // For debug, render the hitboxes
                auto& bb = GetBoundingBox();
                int bx = bb.GetX();
                int by = bb.GetY();
                int bw = bb.GetWidth();
                int bh = bb.GetHeight();
                CRectangle r( bx, by, bw, bh );
                r.SetColor( 64, 64, 255, 128 );
                renderer->Render( &r );
                Render( renderer );

                #ifdef DEBUGMORE
                CLineSegment line;
                line.SetPosition( GetX(), GetY() );
                int m_iX2 = GetSpeed()[0] * 10;
                int m_iY2 = GetSpeed()[1] * 10;
                line.SetDirection( CVector2i( m_iX2, m_iY2 ) );
                line.SetColor( 255, 64, 64, 128 );
                renderer->Render( &line );
                #endif

            }
        }

        void Update( float fSeconds, float fRealSeconds ) override
        {
            DISCARD_UNUNSED_PARAMETER( fRealSeconds );

            // Update position and calculate new velocity
            m_vPosition[0] += GetSpeed()[0] * fSeconds;
            m_vPosition[1] += GetSpeed()[1] * fSeconds;

            if ( this->GetY() < -m_iH ) {
                // Mark this entity to be deleted (or reused [I know, I'm optimization junkie]) on next update
                SetDead( true );
            }

            if ( this->GetY() > m_iScreenH + m_iH ) {
                // Mark this entity to be deleted (or reused [I know, I'm optimization junkie]) on next update
                SetDead( true );
            }

            if ( this->GetX() < -m_iW ) {
                // Mark this entity to be deleted (or reused [I know, I'm optimization junkie]) on next update
                SetDead( true );
            }

            if ( this->GetX() > m_iScreenW + m_iW ) {
                // Mark this entity to be deleted (or reused [I know, I'm optimization junkie]) on next update
                SetDead( true );
            }

            UpdateBoundingBox();
        }

        void UpdateBoundingBox() override
        {
            SetBoundingBoxPos( GetX() - (m_iW/2), GetY() - (m_iH/2));
        }

    protected:
    private:
        // Projectile
        int m_nProjectileID = -1;
        // Projectile w/h
        int m_iW = 0;
        int m_iH = 0;
        // Screen w/h
        int m_iScreenW = 0;
        int m_iScreenH = 0;
        // Owner
        int m_nOwnerID = -1;
};

#endif // ENTITYENEMYPROJECTILE_HPP
