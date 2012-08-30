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

#ifndef EXPLOSIONTHREAD_H
#define EXPLOSIONTHREAD_H

#include "DemoEngine/Threaded.hpp"
#include "DemoEngine/Properties.hpp"
#include "ResourceIDs.hpp"

using namespace DemoEngine;

class CExplosionThread : public CThreaded
{
public:
    void SetExplosion( unsigned int nResourceID )
    {
        m_nResourceID = nResourceID;
    }

    int Execute() override
    {
        auto& properties = CSingleton<CProperties>::Instance();
        int w = (Uint32)properties->Property("Video","Width", (Uint32)640);
        int h = (Uint32)properties->Property("Video","Height", (Uint32)480);

        auto& ps = ExplosionSystemFactory::Instance()->Get( m_nResourceID );

        while ( IsThreadRunning() ) {

            // Delay random amount of time
            SDL_Delay( 500 + rand() % 300 );

            // Lock app wide mutex
            MutexFactory::Instance()->Get( RESOURCE::MUTEX_MAINAPP )->Lock();

            int x = rand() % w;
            int y = rand() % h;
            ps->SetPosition( CVector2f(x, y) );
            ps->FireParticles( 100+rand()%100 );

            while( ps->IsAlive() && IsThreadRunning() )
            {
                SDL_Delay(10);
            }

            // Unlock app wide mutex
            MutexFactory::Instance()->Get( RESOURCE::MUTEX_MAINAPP )->Release();
        }
        SetThreadRunning( false );
        return( 0 );
    }
private:
    unsigned int m_nResourceID = -1;
};

typedef CSingleton<CSharedResourceFactory<int, CExplosionThread>> ExplosionThreadFactory;

#endif // EXPLOSIONTHREAD_H
