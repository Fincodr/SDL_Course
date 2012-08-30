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

#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include <cmath>
#include <iostream>
#include <cassert>
#include <forward_list>
#include <SDL.h>
#include <SDL_gfxPrimitives.h>
#include "Macros.hpp"
#include "IUpdateable.hpp"
#include "IRenderable.hpp"
#include "ImageAlpha.hpp"
#include "Vector2.hpp"
#include "Renderer.hpp"
#include "UniqueID.hpp"
#include "Interpolation.hpp"
#include "InterpolationSet.hpp"

#define PARTICLE_SYSTEM_THREADSAFE

#ifdef PARTICLE_SYSTEM_THREADSAFE
#include "Mutex.hpp"
#endif


namespace DemoEngine {

    using std::forward_list;
    using std::make_shared;
    using std::shared_ptr;

    class CStatistics
    {
    public:
        CStatistics() {};
        ~CStatistics() {};
        unsigned int ParticleCtorCount = 0;
        unsigned int ParticleDtorCount = 0;
    };

    class CParticle
    {
    public:
        CParticle() : m_vPos(), m_vPosLast(), m_vVelocity(), m_vInitialVelocity(), m_Color() {
            m_UID = CSingleton<CUniqueID>::Instance()->getID();
            #ifdef DEBUGCTORS
            cout << "CParticle ctor called for #" << m_UID << endl;
            #endif
        }
        virtual ~CParticle() {
            #ifdef DEBUGCTORS
            cout << "CParticle dtor called for #" << m_UID << endl;
            #endif
        }
        unsigned int GetID() { return m_UID; }
        CVector2f   m_vPos;
        CVector2f   m_vPosLast;
        CVector2f   m_vVelocity;
        CVector2f   m_vInitialVelocity;
        float       m_fEnergy = 0.0f;
        float       m_fTime = 0.0f;
        float       m_fSize = 0.0f;
        SDL_Color   m_Color;
    protected:
    private:
        unsigned int m_UID = 0;
    };

    class CParticleSystem : public IUpdateable, public IRenderable
    {
        //typedef forward_list<CParticle*> List_t;
        typedef forward_list<shared_ptr<CParticle>> List_t;

        // default values
        const float kEnergyDecrementPerSec = 1.0;

        public:

            CParticleSystem() : m_lstParticles(), m_lstDeadParticles(), m_lstTrailParticles(), m_lstDeadTrailParticles(), m_ListGuardMutex()
            {
                m_UID = CSingleton<CUniqueID>::Instance()->getID();
            }

            virtual ~CParticleSystem()
            {
                // delete lstParticles and CParticles
                if ( !m_lstParticles.empty() ) {
                    m_lstParticles.clear();
                }
                // delete lstDeadParticles and CParticles
                if ( !m_lstDeadParticles.empty() ) {
                    m_lstDeadParticles.clear();
                }
                // delete lstParticles and CParticles
                if ( !m_lstTrailParticles.empty() ) {
                    m_lstTrailParticles.clear();
                }
                // delete lstDeadParticles and CParticles
                if ( !m_lstDeadTrailParticles.empty() ) {
                    m_lstDeadTrailParticles.clear();
                }
            }

            void Initialize( size_t nSize = 500, size_t nTrailSize = 0 )
            {

                // Set default values
                m_fEnergyDecrementPerSec = kEnergyDecrementPerSec;

                // Set values
                m_nMaxParticles = nSize;
                m_nNumParticlesAlive = 0;

                m_nMaxTrailParticles = nTrailSize;
                m_nNumTrailParticlesAlive = 0;

                m_lstParticles.clear();
                m_lstDeadParticles.clear();

                m_lstTrailParticles.clear();
                m_lstDeadTrailParticles.clear();

                // Fill with Dead particles
                for ( size_t i=0; i!= m_nMaxParticles; ++i ) {
                    auto particle = make_shared<CParticle>();
                    m_lstDeadParticles.push_front( particle );
                }

                if ( nTrailSize > 0 ) {
                    // Fill with Dead particles
                    for ( size_t i=0; i!= m_nMaxTrailParticles; ++i ) {
                        auto particle = make_shared<CParticle>();
                        m_lstDeadTrailParticles.push_front( particle );
                    }
                }

                m_fMaxTime = GetDuration();

                bInitialized = true;
            }

            void SetVelocityOverTime( int nResourceID )
            {
                m_nVelocityOverTimeID = nResourceID;
            }

            void SetAlphaOverTime( int nResourceID )
            {
                m_nAlphaOverTimeID = nResourceID;
            }

            void SetTrailAlphaOverTime( int nResourceID )
            {
                m_nTrailAlphaOverTimeID = nResourceID;
            }

            void SetColorOverTime( int nResourceID )
            {
                m_nColorOverTimeID = nResourceID;
            }

            void SetTrailColorOverTime( int nResourceID )
            {
                m_nTrailColorOverTimeID = nResourceID;
            }

            void SetSizeOverTime( int nResourceID )
            {
                m_nSizeOverTimeID = nResourceID;
            }

            void SetGravityOverTime( int nResourceID )
            {
                m_nGravityOverTimeID = nResourceID;
            }

            void SetSprite( int nResourceID )
            {
                m_nSpriteID = nResourceID;
            }

            void SetPrimitiveType( int nType, int nTrailType = -1 )
            {
                m_nPrimitiveType = nType;
                if ( nTrailType == -1 )
                    m_nTrailPrimitiveType = m_nPrimitiveType;
                else
                    m_nTrailPrimitiveType = nTrailType;
            }

            void SetTrails( bool bTrails )
            {
                m_bTrails = bTrails;
            }

            unsigned int GetID() {
                return m_UID;
            }

            void FireParticles( size_t nNumParticles, int nMaxInSecond = -1 ) {

                if ( !bInitialized ) Initialize();

                if ( nNumParticles+m_nNumParticlesAlive >= m_nMaxParticles )
                {
                    nNumParticles = m_nMaxParticles-m_nNumParticlesAlive;
                }

                if ( nMaxInSecond != -1 ) {
                    // Limit new particle amount to "max particles in second"
                    //
                    float fDiff = m_fTime - m_fLastAddTime;
                    if ( fDiff > 100 )
                    {
                        int nAmountInSecond = static_cast<int>(m_nLastAddAmount/fDiff);
                        if ( nAmountInSecond > nMaxInSecond )
                            nNumParticles = 0;
                    }

                    // cout << "\rAmount: " << m_nNumParticlesAlive << ", Amount/s: " << nAmountInSecond << "    ";

                    m_fLastAddTime = m_fTime;
                    m_nLastAddAmount = nNumParticles;
                }

                if ( nNumParticles > 0 ) {

                    // Guard particle list access with Mutex
                    // because we can't be sure if other thread
                    // is moving/deleting our particle at the same
                    #ifdef PARTICLE_SYSTEM_THREADSAFE
                    m_ListGuardMutex.Lock();
                    #endif
                    {
                        // activate particles from dead list
                        for ( size_t i = 0; i != nNumParticles; ++i )
                        {
                            auto p = m_lstDeadParticles.front();
                            m_lstDeadParticles.pop_front();
                            m_lstParticles.push_front( p );
                            float f = (float)i/nNumParticles;
                            InitVelocity( p, f );
                            InitPosition( p, f );
                            InitEnergy( p, f );
                        }
                        m_nNumParticlesAlive += nNumParticles;
                    }
                    #ifdef PARTICLE_SYSTEM_THREADSAFE
                    m_ListGuardMutex.Release();
                    #endif
                }

            }

            void FireTrailParticle( shared_ptr<CParticle>& pOrig )
            {
                if ( !bInitialized ) Initialize();

                int nNumTrailParticles = 1;

                if ( nNumTrailParticles+m_nNumTrailParticlesAlive >= m_nMaxTrailParticles )
                {
                    nNumTrailParticles = m_nMaxTrailParticles-m_nNumTrailParticlesAlive;
                }

                if ( nNumTrailParticles > 0 ) {
                    // Particle list doesn't need to be guarded
                    // because no other thread can be updating
                    // trail particle lists at the same time

                    // activate particle from dead list
                    auto p = m_lstDeadTrailParticles.front();
                    m_lstDeadTrailParticles.pop_front();
                    m_lstTrailParticles.push_front( p );
                    // Copy values
                    p->m_fTime = pOrig->m_fTime;
                    p->m_fEnergy = 0.1f;
                    p->m_vPos = pOrig->m_vPos;
                    p->m_vPosLast = pOrig->m_vPosLast;
                    p->m_vVelocity = CVector2f(0,0);
                    m_nNumTrailParticlesAlive++;
                }
            }

            void Update( float fSeconds, float fRealSeconds ) override
            {
                DISCARD_UNUNSED_PARAMETER( fRealSeconds );

                m_fTime += fSeconds;

                int iCounter=0;

                m_lstTrailParticles.remove_if( [&](shared_ptr<CParticle>& p)
                    {
                        if ( p->m_fEnergy > 0.0f )
                        {
                            // Process particle here
                            p->m_fTime += fSeconds;
                            UpdateEnergy( p, fSeconds );
                            UpdateTrailColor( p, fSeconds );
                            return false; // keep item
                        }
                        else
                        {
                            m_lstDeadTrailParticles.push_front( std::move(p) );
                            --m_nNumTrailParticlesAlive;
                            return true; // remove item
                        }
                    }
                );

                // Guard particle list access with Mutex
                // because we can't be sure if other thread
                // is moving/deleting our particle at the same
                #ifdef PARTICLE_SYSTEM_THREADSAFE
                m_ListGuardMutex.Lock();
                #endif
                {
                    m_lstParticles.remove_if( [&](shared_ptr<CParticle>& p)
                        {
                            if ( p->m_fEnergy > 0.0f )
                            {
                                // If trail particles are active, create them now
                                if ( m_bTrails && (iCounter%20==0) ) {
                                    FireTrailParticle( p );
                                    UpdateTrailColor(p, fSeconds);
                                }
                                p->m_fTime += fSeconds;
                                p->m_vPosLast = p->m_vPos;
                                UpdateVelocity( p, fSeconds );
                                UpdatePosition( p, fSeconds );
                                UpdateEnergy( p, fSeconds );
                                UpdateColor( p, fSeconds );
                                UpdateSize( p, fSeconds );
                                return false; // keep item
                            }
                            else
                            {
                                m_lstDeadParticles.push_front( std::move(p) );
                                --m_nNumParticlesAlive;
                                return true; // remove item
                            }
                            ++iCounter;
                        }
                    );
                    //cout << "\r" << m_nNumParticlesAlive << "    " << endl;
                }
                #ifdef PARTICLE_SYSTEM_THREADSAFE
                m_ListGuardMutex.Release();
                #endif
            }

            virtual void RenderDebug( unique_ptr<CRenderer>& renderer ) override
            {
                // no debug rendering implemented, use normal rendering
                Render( renderer );
            }

            virtual void Render( unique_ptr<CRenderer>& renderer ) override
            {
                auto screen = renderer->GetScreen();

                for( auto& p : m_lstTrailParticles )
                {
                    if ( p->m_Color.unused > 0 )
                    {
                        float s = p->m_fSize;
                        int x = p->m_vPos[0];
                        int y = p->m_vPos[1];

                        if ( m_nSpriteID != -1 ) {
                            auto& sprite = ImageAlphaFactory::Instance()->Get( m_nSpriteID );
                            renderer->Render( sprite, x, y );
                        }
                        else
                        {
                            int x1 = x-s/2;
                            int y1 = y-s/2;
                            int x2 = x+s/2;
                            int y2 = y+s/2;
                            switch ( m_nTrailPrimitiveType ) {
                            default:
                            case 0: // pixel
                                pixelRGBA( screen,
                                static_cast<int>(x),
                                static_cast<int>(y),
                                static_cast<int>(p->m_Color.r),
                                static_cast<int>(p->m_Color.g),
                                static_cast<int>(p->m_Color.b),
                                static_cast<int>(p->m_Color.unused) );
                                break;
                            case 1: // line
                                lineRGBA( screen,
                                    static_cast<int>(x),
                                    static_cast<int>(y),
                                    static_cast<int>(p->m_vPosLast[0]),
                                    static_cast<int>(p->m_vPosLast[1]),
                                    static_cast<int>(p->m_Color.r),
                                    static_cast<int>(p->m_Color.g),
                                    static_cast<int>(p->m_Color.b),
                                    static_cast<int>(p->m_Color.unused) );
                                break;
                            case 2: // box
                                boxRGBA( screen,
                                    static_cast<int>(x1),
                                    static_cast<int>(y1),
                                    static_cast<int>(x2),
                                    static_cast<int>(y2),
                                    static_cast<int>(p->m_Color.r),
                                    static_cast<int>(p->m_Color.g),
                                    static_cast<int>(p->m_Color.b),
                                    static_cast<int>(p->m_Color.unused) );
                                break;
                            case 3: // star
                                hlineRGBA( screen,
                                    static_cast<int>(x1),
                                    static_cast<int>(x2),
                                    static_cast<int>(y),
                                    static_cast<int>(p->m_Color.r),
                                    static_cast<int>(p->m_Color.g),
                                    static_cast<int>(p->m_Color.b),
                                    static_cast<int>(p->m_Color.unused) );
                                vlineRGBA( screen,
                                    static_cast<int>(x),
                                    static_cast<int>(y1),
                                    static_cast<int>(y2),
                                    static_cast<int>(p->m_Color.r),
                                    static_cast<int>(p->m_Color.g),
                                    static_cast<int>(p->m_Color.b),
                                    static_cast<int>(p->m_Color.unused) );
                                break;
                            }
                        }
                    }
                }

                for( auto& p : m_lstParticles )
                {
                    if ( p->m_Color.unused > 0 )
                    {
                        float s = p->m_fSize;
                        int x = p->m_vPos[0];
                        int y = p->m_vPos[1];
                        int x1 = x-s/2;
                        int y1 = y-s/2;
                        int x2 = x+s/2;
                        int y2 = y+s/2;

                        if ( m_nSpriteID != -1 ) {
                            auto& sprite = ImageAlphaFactory::Instance()->Get( m_nSpriteID );
                            renderer->Render( sprite, x, y );
                        }
                        else
                        {
                            switch ( m_nPrimitiveType ) {
                            default:
                            case 0: // pixel
                                pixelRGBA( screen,
                                static_cast<int>(x),
                                static_cast<int>(y),
                                static_cast<int>(p->m_Color.r),
                                static_cast<int>(p->m_Color.g),
                                static_cast<int>(p->m_Color.b),
                                static_cast<int>(p->m_Color.unused) );
                                break;
                            case 1: // line
                                lineRGBA( screen,
                                    static_cast<int>(x),
                                    static_cast<int>(y),
                                    static_cast<int>(p->m_vPosLast[0]),
                                    static_cast<int>(p->m_vPosLast[1]),
                                    static_cast<int>(p->m_Color.r),
                                    static_cast<int>(p->m_Color.g),
                                    static_cast<int>(p->m_Color.b),
                                    static_cast<int>(p->m_Color.unused) );
                                break;
                            case 2: // box
                                boxRGBA( screen,
                                    static_cast<int>(x1),
                                    static_cast<int>(y1),
                                    static_cast<int>(x2),
                                    static_cast<int>(y2),
                                    static_cast<int>(p->m_Color.r),
                                    static_cast<int>(p->m_Color.g),
                                    static_cast<int>(p->m_Color.b),
                                    static_cast<int>(p->m_Color.unused) );
                                break;
                            case 3: // star
                                hlineRGBA( screen,
                                    static_cast<int>(x1),
                                    static_cast<int>(x2),
                                    static_cast<int>(y),
                                    static_cast<int>(p->m_Color.r),
                                    static_cast<int>(p->m_Color.g),
                                    static_cast<int>(p->m_Color.b),
                                    static_cast<int>(p->m_Color.unused) );
                                vlineRGBA( screen,
                                    static_cast<int>(x),
                                    static_cast<int>(y1),
                                    static_cast<int>(y2),
                                    static_cast<int>(p->m_Color.r),
                                    static_cast<int>(p->m_Color.g),
                                    static_cast<int>(p->m_Color.b),
                                    static_cast<int>(p->m_Color.unused) );
                                break;
                            }
                        }
                    }
                }
            }

            inline bool IsAlive() const
            {
                return (m_nNumParticlesAlive > 0);
            }

            virtual void UpdateEnergy( shared_ptr<CParticle>& p, float fSeconds )
            {
                p->m_fEnergy -= GetEnergyDecrementPerSec() * fSeconds;
            }

            virtual void UpdatePosition( shared_ptr<CParticle>& p, float fSeconds )
            {
                p->m_vPos += (p->m_vVelocity * fSeconds);
            }

            virtual void UpdateColor( shared_ptr<CParticle>& p, float fSeconds )
            {
                DISCARD_UNUNSED_PARAMETER( fSeconds );

                int r = 255;
                int g = 255;
                int b = 255;
                int a = 255;

                if ( m_nColorOverTimeID != -1 )
                {
                    // use InterpolationSet for velocity changes
                    auto& cSet = SDLColorInterpolationSetFactory::Instance()->Get( m_nColorOverTimeID );
                    SDL_Color color = InterpolateSDLColor( (float)(p->m_fTime/m_fMaxTime), cSet );
                    r = color.r;
                    g = color.g;
                    b = color.b;
                }

                if ( m_nAlphaOverTimeID != -1 )
                {
                    // use InterpolationSet for velocity changes
                    auto& aSet = InterpolationSetFactory::Instance()->Get( m_nAlphaOverTimeID );
                    a  = 255 * Interpolate<float,float>( (float)(p->m_fTime/m_fMaxTime), aSet );
                }

                p->m_Color = { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a };
            }

            virtual void UpdateTrailColor( shared_ptr<CParticle>& p, float fSeconds )
            {
                DISCARD_UNUNSED_PARAMETER( fSeconds );

                int r = 255;
                int g = 255;
                int b = 255;
                int a = 255;

                if ( m_nTrailColorOverTimeID != -1 )
                {
                    // use InterpolationSet for velocity changes
                    auto& cSet = SDLColorInterpolationSetFactory::Instance()->Get( m_nTrailColorOverTimeID );
                    SDL_Color color = InterpolateSDLColor( (float)(p->m_fTime/m_fMaxTime), cSet );
                    r = color.r;
                    g = color.g;
                    b = color.b;
                }
                else
                {
                    r = p->m_Color.r;
                    g = p->m_Color.g;
                    b = p->m_Color.b;
                }

                if ( m_nTrailAlphaOverTimeID != -1 )
                {
                    // use InterpolationSet for velocity changes
                    auto& aSet = InterpolationSetFactory::Instance()->Get( m_nTrailAlphaOverTimeID );
                    a  = 255 * Interpolate<float,float>( (float)(p->m_fTime/m_fMaxTime), aSet );
                }
                else
                {
                    a = p->m_Color.unused - 25;
                    if ( a < 0 ) a = 0;
                }

                p->m_Color = { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a };
            }


            virtual void UpdateSize( shared_ptr<CParticle>& p, float fSeconds )
            {
                DISCARD_UNUNSED_PARAMETER( fSeconds );

                float sizeValue = 1.0;
                if ( m_nSizeOverTimeID != -1 )
                {
                    // use InterpolationSet for size changes
                    auto& aSet = InterpolationSetFactory::Instance()->Get( m_nSizeOverTimeID );
                    sizeValue = Interpolate<float,float>( (float)(p->m_fTime/m_fMaxTime), aSet );
                }
                p->m_fSize = sizeValue;
            }

            virtual void UpdateVelocity( shared_ptr<CParticle>& p, float fSeconds )
            {
                DISCARD_UNUNSED_PARAMETER( fSeconds );

                float gravity = 0;
                if ( m_nGravityOverTimeID != -1 )
                {
                    // use InterpolationSet for gravity changes
                    auto& velSet = InterpolationSetFactory::Instance()->Get( m_nGravityOverTimeID );
                    gravity = Interpolate<float,float>( (float)(p->m_fTime/m_fMaxTime), velSet );
                }

                float v = 1.0f;
                if ( m_nVelocityOverTimeID != -1 )
                {
                    // use InterpolationSet for velocity changes
                    auto& velSet = InterpolationSetFactory::Instance()->Get( m_nVelocityOverTimeID );
                    v = Interpolate<float,float>( (float)(p->m_fTime/m_fMaxTime), velSet );
                }

                // add velocity to the vector
                CVector2<float> vec = (p->m_vInitialVelocity * v); // * fSeconds;

                // add gravity to vector
                vec += CVector2<float>(0, (gravity*p->m_fTime));

                // set velocity value back to particle
                p->m_vVelocity = vec;
            }

            inline float GetEnergyDecrementPerSec()
            {
                return m_fEnergyDecrementPerSec;
            }

            virtual float GetDuration()
            {
                return 1.0f;
            }

            // Must be implemented on subclass
            virtual void InitVelocity( shared_ptr<CParticle>& p, float fNumOfTotal = 1.0f ) = 0;
            virtual void InitPosition( shared_ptr<CParticle>& p, float fNumOfTotal = 1.0f ) = 0;
            virtual void InitEnergy( shared_ptr<CParticle>& p, float fNumOfTotal = 1.0f ) = 0;

        protected:
            float   m_fEnergyDecrementPerSec = 0.0f;
            size_t  m_nNumParticlesAlive = 0;
            size_t  m_nMaxParticles = 0;
            List_t  m_lstParticles;
            List_t  m_lstDeadParticles;
            int     m_nVelocityOverTimeID = -1;
            int     m_nAlphaOverTimeID = -1;
            int     m_nColorOverTimeID = -1;
            int     m_nGravityOverTimeID = -1;
            int     m_nSizeOverTimeID = -1;
            int     m_nSpriteID = -1;
            float   m_fTime = 0.0f;
            float   m_fLastAddTime = 0.0f;
            int     m_nLastAddAmount = 0;
            float   m_fMaxTime = 0.0f;
            int     m_nPrimitiveType = 0;       // 0=pixel, 1=line, 2=rectangle (supports size over time)

            // Trails (copies of drawn objects automatically fading away, usually no other animation)
            size_t  m_nNumTrailParticlesAlive = 0;
            size_t  m_nMaxTrailParticles = 0;
            bool    m_bTrails = false;
            int     m_nTrailSteps = 5;
            int     m_nTrailColorOverTimeID = -1;
            int     m_nTrailAlphaOverTimeID = -1;
            List_t  m_lstTrailParticles;
            List_t  m_lstDeadTrailParticles;
            int     m_nTrailPrimitiveType = 0;       // 0=pixel, 1=line, 2=rectangle (supports size over time)
        private:
            unsigned int m_UID = 0;
            bool bInitialized = false;
            #ifdef PARTICLE_SYSTEM_THREADSAFE
            CMutex  m_ListGuardMutex;
            #endif
    };

}

#endif // PARTICLESYSTEM_HPP
