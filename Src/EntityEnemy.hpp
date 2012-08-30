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

#ifndef ENTITYENEMY_HPP
#define ENTITYENEMY_HPP

#include "DemoEngine/Game.hpp"
#include "DemoEngine/ImageAlpha.hpp"
#include "DemoEngine/GameObject.hpp"
#include "DemoEngine/Rectangle.hpp"
#include "ResourceIDs.hpp"

using namespace DemoEngine;

class EntityEnemy : public CGameObjectFloat
{
    public:
        EntityEnemy()
        {
            auto& renderer = CSingleton<CRenderer>::Instance();
            auto screen = renderer->GetScreen();
            m_iScreenW = screen->w;
            m_iScreenH = screen->h;

            // Load ship image
            auto& m_enemyShipImg = ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_RED );

            // TODO: Rotate ship image
            m_iShipW = m_enemyShipImg->GetSurface()->w;
            m_iShipH = m_enemyShipImg->GetSurface()->h;

            // Set bounding box for collision detection
            auto& bounds = GetBounds();
            bounds.SetDimensions( 59-6, 43-6 );
            bounds.SetPosition( 0+3, 0+3 );
            #ifdef DEBUGCTORS
            cout << "EntityEnemy ctor called! (id = " << GetID() << ")" << endl;
            #endif
        }

        ~EntityEnemy()
        {
            #ifdef DEBUGCTORS
            cout << "EntityEnemy dtor called! (id = " << GetID() << ")" << endl;
            #endif
        }

        void Render( unique_ptr<CRenderer>& renderer ) override
        {
            if ( !IsDead() ) {
                int m_iShipX = GetX() - m_iShipW/2;
                int m_iShipY = GetY() - m_iShipH/2;
                if ( ( m_iShipX < -m_iShipW*2 /* shadow too! */ ) ||
                    ( m_iShipX > m_iScreenW+m_iShipW ) ||
                    ( m_iShipY < -m_iShipH*2 /* shadow too */ ) ||
                    ( m_iShipY > m_iScreenH+m_iShipH ) )
                {
                    // object is not visible, do nothing.
                }
                else
                {
                    ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_SHADOW )->SetAlpha(50);
                    renderer->Render( ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_SHADOW ), m_iShipX + 45, m_iShipY + 55 );
                    if ( !m_bHit )
                    {
                        if ( m_nEnemyType == 0 )
                            renderer->Render( ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_GREEN ), m_iShipX, m_iShipY );
                        else
                            renderer->Render( ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_RED ), m_iShipX, m_iShipY );
                    }
                    else
                    {
                        if ( m_nEnemyType == 0 )
                            renderer->Render( ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_GREEN_HIT ), m_iShipX, m_iShipY );
                        else
                            renderer->Render( ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_RED_HIT ), m_iShipX, m_iShipY );
                    }
                }
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
                r.SetColor( 255, 64, 64, 128 );
                renderer->Render( &r );
                Render( renderer );
            }
        }

        void Update( float fSeconds, float fRealSeconds ) override
        {
            DISCARD_UNUNSED_PARAMETER( fRealSeconds );

            m_fTime += fSeconds;

            m_vPosition[0] += GetSpeed()[0] * fSeconds;
            m_vPosition[1] += GetSpeed()[1] * fSeconds;

            float fDamageLevel = 1.0f-((float)GetHealth()/(float)GetMaxHealth());
            if ( fDamageLevel > 0.0f ) {
                // Fire particle effects depending on the damage level
                auto& psSmoke = SmokeSystemFactory::Instance()->Get( RESOURCE::PS_SMOKE );
                psSmoke->SetPosition( CVector2f( GetX(), GetY() ) );
                psSmoke->FireParticles( 2 + fDamageLevel*2, 10 );

            }

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
            SetBoundingBoxPos( GetX() - (m_iShipW/2), GetY() - (m_iShipH/2) );
        }

        float GetCooldownTimer()
        {
            return( m_fTime - m_fCooldownTime );
        }

        void Hit()
        {
            m_fHitTimer = 0.1f;
            m_bHit = true;
        }

        void Fire()
        {
            m_fCooldownTime = m_fTime;
        }

        void SetEnemyType( int nEnemyType )
        {
            m_nEnemyType = nEnemyType;
        }

        int GetEnemyType() { return m_nEnemyType; }

    protected:
    private:
        // Ship w/h
        int m_iShipW = 0;
        int m_iShipH = 0;
        // Screen w/h
        int m_iScreenW = 0;
        int m_iScreenH = 0;
        // Fire cooldown
        bool m_bFired = false;
        float m_fCooldownTime = 0.0f;
        float m_fTime = 0.0f;
        int m_nEnemyType = 0;
        // Hit timer
        float m_fHitTimer = 0.0f;
        bool m_bHit = false;
};

#endif // ENTITYENEMY_HPP
