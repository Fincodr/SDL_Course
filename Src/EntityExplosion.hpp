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

#ifndef ENTITYEXPLOSION_HPP
#define ENTITYEXPLOSION_HPP

#include "DemoEngine/Game.hpp"
#include "DemoEngine/GameObject.hpp"
#include "DemoEngine/Rectangle.hpp"
#include "ResourceIDs.hpp"

using namespace DemoEngine;

class EntityExplosion : public CGameObjectFloat
{
    public:

        const string kAnimationName = "Blow up";

        EntityExplosion()
        {
            auto& renderer = CSingleton<CRenderer>::Instance();
            auto screen = renderer->GetScreen();
            m_iScreenW = screen->w;
            m_iScreenH = screen->h;

            auto& anim = AnimationFactory::Instance()->Get( RESOURCE::EXPLOSION );
            m_iFrames = anim->GetAnimationFrames( kAnimationName );
            auto& rect = anim->GetAnimationRect( kAnimationName, m_iFrame );
            m_iW = rect.w;
            m_iH = rect.h;

            #ifdef DEBUGCTORS
            cout << "EntityExplosion ctor called! (id = " << GetID() << ")" << endl;
            #endif
        }

        ~EntityExplosion()
        {
            #ifdef DEBUGCTORS
            cout << "EntityExplosion dtor called! (id = " << GetID() << ")" << endl;
            #endif
        }

        void Fire();

        void Render( unique_ptr<CRenderer>& renderer ) override
        {
            if ( !IsDead() ) {
                int m_iX = GetX() - m_iW/2;
                int m_iY = GetY() - m_iH/2;
                if ( ( m_iX < -m_iW ) ||
                    ( m_iX > m_iScreenW+m_iW ) ||
                    ( m_iY < -m_iH ) ||
                    ( m_iY > m_iScreenH+m_iH ) )
                {
                    // object is not visible, do nothing.
                }
                else
                {
                    auto& img = ImageAlphaFactory::Instance()->Get( RESOURCE::EXPLOSION );
                    auto& anim = AnimationFactory::Instance()->Get( RESOURCE::EXPLOSION );
                    auto& rect = anim->GetAnimationRect( kAnimationName, m_iFrame );
                    renderer->Render( img, m_iX, m_iY, &rect );
                }
            }
        }

        void RenderDebug( unique_ptr<CRenderer>& renderer ) override
        {
            Render( renderer );
        }

        void Update( float fSeconds, float fRealSeconds ) override
        {
            DISCARD_UNUNSED_PARAMETER( fRealSeconds );

            m_fTime += fSeconds;
            m_iFrame = static_cast<int>(m_fTime * m_iFPS);
            if ( m_iFrame >= m_iFrames ) {
                SetDead( true );
            }
        }

        void UpdateBoundingBox() override {};

        void SetFrame( int frame )
        {
            // Set time so that we get the specified frame of animation
            m_fTime = (float)frame / (float)m_iFPS;
        }

        void SetFPS( int fps ) { m_iFPS = fps; }

        void Execute()
        {
            // Fire particle effects too
            auto& psExplosion = ExplosionSystemFactory::Instance()->Get( RESOURCE::PS_EXPLOSION );
            psExplosion->SetPosition( CVector2f( GetX(), GetY() ) );
            psExplosion->FireParticles(100);
        }

    protected:
    private:
        // Explosion w/h
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
};

#endif // ENTITYEXPLOSION_HPP
