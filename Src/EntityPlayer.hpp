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

#ifndef ENTITYPLAYER_HPP
#define ENTITYPLAYER_HPP

#include "DemoEngine/Game.hpp"
#include "DemoEngine/ImageAlpha.hpp"
#include "DemoEngine/GameObject.hpp"
#include "ResourceIDs.hpp"

using namespace DemoEngine;

class EntityPlayer : public CGameObjectFloat
{
    public:
        const string kAnimationName = "Plane";
        const float kMinSpeed = 10.0f;
        const float kMaxSpeed = 500.0f;

        EntityPlayer()
        {
            auto& renderer = CSingleton<CRenderer>::Instance();
            auto screen = renderer->GetScreen();
            m_iScreenW = screen->w;
            m_iScreenH = screen->h;

            auto& anim = AnimationFactory::Instance()->Get( RESOURCE::PLAYER_PLANE );
            m_iFrames = anim->GetAnimationFrames( kAnimationName );
            auto& rect = anim->GetAnimationRect( kAnimationName, m_iFrame );
            m_iW = rect.w;
            m_iH = rect.h;

            m_iFrame = 2;

            // Set bounding box for collision detection
            auto& bounds = GetBounds();
            bounds.SetDimensions( 40-4, 64-8 );
            bounds.SetPosition( 1+2, 0+4 );

            #ifdef DEBUGCTORS
            cout << "EntityPlayer ctor called! (id = " << GetID() << ")" << endl;
            #endif
        }

        ~EntityPlayer()
        {
            #ifdef DEBUGCTORS
            cout << "EntityPlayer dtor called! (id = " << GetID() << ")" << endl;
            #endif
        }

        void Fire();

        void Hit()
        {
            m_fHitTimer = 0.1f;
            m_bHit = true;
        }

        void Render( unique_ptr<CRenderer>& renderer ) override
        {
            if ( !IsDead() ) {
                int m_iX = GetX();
                int m_iY = GetY();

                auto& m_playerShipImg = ImageAlphaFactory::Instance()->Get( RESOURCE::PLAYER_PLANE );
                auto& m_playerShipImgHit = ImageAlphaFactory::Instance()->Get( RESOURCE::PLAYER_PLANE_HIT );
                auto& m_playerShipShadow = ImageAlphaFactory::Instance()->Get( RESOURCE::PLAYER_PLANE_SHADOW );
                auto& anim = AnimationFactory::Instance()->Get( RESOURCE::PLAYER_PLANE );
                auto& animShadow = AnimationFactory::Instance()->Get( RESOURCE::PLAYER_PLANE_SHADOW );
                auto& rect = anim->GetAnimationRect( kAnimationName, m_iFrame );
                auto& rectShadow = animShadow->GetAnimationRect( kAnimationName, m_iFrame );
                m_playerShipShadow->SetAlpha(50);
                renderer->Render( m_playerShipShadow, m_iX - (m_iW/2) + 45, m_iY - (m_iH/2) + 55, &rectShadow );
                if ( !m_bHit )
                    renderer->Render( m_playerShipImg, m_iX - (m_iW/2), m_iY - (m_iH/2), &rect );
                else
                    renderer->Render( m_playerShipImgHit, m_iX - (m_iW/2), m_iY - (m_iH/2), &rect );

                // if we are at the edge we can mirror the ship to other side to simulate wrapping
                // calculate where would be the mirror ships
                // M1 = top-right
                int M1x = (m_iX + m_iScreenW);
                int M1y = m_iY;
                if ( M1x > m_iScreenW*2-m_iW/2 ) {
                    M1x = M1x - m_iScreenW*2;
                }
                // M2 = bottom-right
                int M2x = (m_iX + m_iScreenW);
                int M2y = (m_iY + m_iScreenH);
                if ( M2x > m_iScreenW*2-m_iW/2 ) {
                    M2x = M2x - m_iScreenW*2;
                }
                if ( M2y > m_iScreenH*2-m_iH/2 ) {
                    M2y = M2y - m_iScreenH*2;
                }
                // M3 = bottom-left
                int M3x = m_iX;
                int M3y = (m_iY + m_iScreenH);
                if ( M3y > m_iScreenH*2-m_iH/2 ) {
                    M3y = M3y - m_iScreenH*2;
                }
                renderer->Render( m_playerShipImg, M1x - (m_iW/2), M1y - (m_iH/2), &rect );
                renderer->Render( m_playerShipShadow, M1x - (m_iW/2) + 45, M1y - (m_iH/2) + 55, &rectShadow );
                renderer->Render( m_playerShipImg, M2x - (m_iW/2), M2y - (m_iH/2), &rect );
                renderer->Render( m_playerShipShadow, M2x - (m_iW/2) + 45, M2y - (m_iH/2) + 55, &rectShadow );
                renderer->Render( m_playerShipImg, M3x - (m_iW/2), M3y - (m_iH/2), &rect );
                renderer->Render( m_playerShipShadow, M2x - (m_iW/2) + 45, M3y - (m_iH/2) + 55, &rectShadow );
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
                r.SetColor( 64, 255, 64, 128 );
                renderer->Render( &r );
                Render( renderer );
            }
        }

        void Update( float fSeconds, float fRealSeconds ) override
        {
            // if we are not accelerating we can slow down gradually
            if ( !IsMovingX() )
            {
                // calculate deceleration speed
                float fDecelerationX = -(GetSpeed()[0]);
                GetAcceleration()[0] = fDecelerationX;
            }
            if ( !IsMovingY() )
            {
                // calculate deceleration speed
                float fDecelerationY = -(GetSpeed()[1]);
                GetAcceleration()[1] = fDecelerationY;
            }

            // compute change in speed (acceleration)
            GetSpeed()[0] += GetAcceleration()[0] * fRealSeconds;
            GetSpeed()[1] += GetAcceleration()[1] * fRealSeconds;


            // If below min speed set speed to zero
            if ( !IsMovingX() ) {
                if ( GetSpeed()[0] > -kMinSpeed && GetSpeed()[0] < kMinSpeed )
                    GetSpeed()[0] = 0.0f;
            }
            if ( !IsMovingY() ) {
                if ( GetSpeed()[1] > -kMinSpeed && GetSpeed()[1] < kMinSpeed )
                    GetSpeed()[1] = 0.0f;
            }

            // Cap max speed
            if ( GetSpeed()[0] < -kMaxSpeed )
                GetSpeed()[0] = -kMaxSpeed;
            if ( GetSpeed()[0] > kMaxSpeed )
                GetSpeed()[0] = kMaxSpeed;
            if ( GetSpeed()[1] < -kMaxSpeed )
                GetSpeed()[1] = -kMaxSpeed;
            if ( GetSpeed()[1] > kMaxSpeed )
                GetSpeed()[1] = kMaxSpeed;

            // Update position and calculate new velocity
            m_vPosition[0] += GetSpeed()[0] * fRealSeconds;
            m_vPosition[1] += GetSpeed()[1] * fRealSeconds;

            // Update bounding box for current position
            UpdateBoundingBox();

            if ( m_fHitTimer > 0 )
            {
                m_fHitTimer -= fSeconds;
                if ( m_fHitTimer <= 0 )
                {
                    m_fHitTimer = 0;
                    m_bHit = false;
                }
            }
        }

        void UpdateBoundingBox() override
        {
            SetBoundingBoxPos( GetX() - (m_iW/2), GetY() - (m_iH/2) );
        }

        void SetFrame( int mFrame ) { m_iFrame = mFrame; }

        int GetFrame() { return m_iFrame; }

    protected:
    private:
        // Ship w/h
        int m_iW = 0;
        int m_iH = 0;
        // Screen w/h
        int m_iScreenW = 0;
        int m_iScreenH = 0;
        // Animation
        int m_iFrame = 0;           // Current animation frame
        int m_iFrames = 0;          // Animation frame count
        float m_fTime = 0.0f;       // Animation cumulative time
        int m_iFPS = 30;            // Default to 30fps
        // Hit timer
        float m_fHitTimer = 0.0f;
        bool m_bHit = false;
};

#endif // ENTITYPLAYER_HPP
