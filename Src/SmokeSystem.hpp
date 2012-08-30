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

#ifndef SMOKESYSTEM_HPP
#define SMOKESYSTEM_HPP

#include "DemoEngine/Macros.hpp"
#include "DemoEngine/ParticleSystem.hpp"
#include "DemoEngine/Math.hpp"

using namespace DemoEngine;

class CSmokeSystem : public CParticleSystem
{
    public:
        CSmokeSystem() : CParticleSystem(), m_vPos() {}

        const float kDuration = 0.5f;

        void SetPosition( const CVector2f& vPos )
        {
            m_vPos = vPos;
        }

        void InitVelocity( shared_ptr<CParticle>& p, float fNumOfTotal = 1.0f ) override
        {
            const float kInitialVelocity = Math::Interpolation::easeOutCirc( fNumOfTotal, 0, 10, 2.0f ) + rand()%2;
            int degrees = rand() % 360;

            float radians = (float)degrees * DemoEngine::Math::kPI / 180;

            p->m_vInitialVelocity[0] = kInitialVelocity * cos(radians);
            p->m_vInitialVelocity[1] = kInitialVelocity * sin(radians);
            p->m_fTime = (float)((float)(rand() % 35)/100);
        }

        void InitPosition( shared_ptr<CParticle>& p, float fNumOfTotal = 1.0f ) override
        {
            DISCARD_UNUNSED_PARAMETER( fNumOfTotal );
            p->m_vPos = m_vPos;
        }

        void InitEnergy ( shared_ptr<CParticle>& p, float fNumOfTotal = 1.0f ) override
        {
            DISCARD_UNUNSED_PARAMETER( fNumOfTotal );
            p->m_fEnergy = kDuration;
        }

        float GetDuration() override
        {
            return kDuration;
        }

    protected:
        CVector2f m_vPos;
    private:
};

typedef CSingleton<CSharedResourceFactory<int, CSmokeSystem>> SmokeSystemFactory;

#endif // SMOKESYSTEM_HPP
