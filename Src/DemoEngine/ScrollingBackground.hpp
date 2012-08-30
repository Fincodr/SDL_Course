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

#ifndef SCROLLINGBACKGROUND_HPP
#define SCROLLINGBACKGROUND_HPP

#include <vector>
#include <memory>
#include <iostream>
#include "Macros.hpp"
#include "GameObject.hpp"
#include "Singleton.hpp"
#include "ResourceFactory.hpp"
#include "Renderer.hpp"
#include "ImageAlpha.hpp"

namespace DemoEngine {

    using std::vector;
    using std::unique_ptr;
    using std::cout;
    using std::endl;

    class CBackgroundLayer
    {
        public:
            CBackgroundLayer() {};

            virtual ~CBackgroundLayer() {};

            void Load( int resourceID )
            {
                m_imgResourceId = resourceID;
            }

            void Render( unique_ptr<CRenderer>& renderer, int x0, int y0)
            {
                auto& m_pImage = CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( m_imgResourceId );
                int iImageHeight = m_pImage->GetSurface()->h;
                int iImageWidth = m_pImage->GetSurface()->w;
                int iScreenHeight = renderer->GetScreen()->h;
                int iScreenWidth = renderer->GetScreen()->w;

                int xStart = m_xSpeed == 0 ? -x0 : -x0/m_xSpeed;
                int yStart = m_ySpeed == 0 ? -y0 : -y0/m_ySpeed;

                // Module is your friend ;)
                xStart = xStart % iImageWidth;
                yStart = yStart % iImageHeight;

                if ( xStart > 0 )
                    xStart -= iImageWidth;

                if ( yStart > 0 )
                    yStart -= iImageHeight;

                // Fill screen
                for ( int y = yStart; y < iScreenHeight; y+= iImageHeight )
                {
                    for ( int x = xStart; x < iScreenWidth; x+= iImageWidth )
                    {
                        renderer->Render( m_pImage, x, y );
                    }
                }
            }

            void SetSpeed( int xspeed, int yspeed )
            {
                m_xSpeed = xspeed;
                m_ySpeed = yspeed;
            }

        protected:
            int m_imgResourceId = 0;
            // Default movement speed 1:1
            int m_xSpeed = 1;
            int m_ySpeed = 1;
        private:
    };

    class CScrollingBackground : public CGameObjectFloat
    {
        public:
            CScrollingBackground() : m_vLayers() {};

            virtual ~CScrollingBackground() {};

            void Initialize( int nCount )
            {
                m_vLayers.reserve(nCount);
                for (int i=0; i!=nCount; ++i)
                    m_vLayers.push_back(nullptr);
            }

            void SetLayer( int nLayerNum, int resourceID )
            {
                unique_ptr<CBackgroundLayer> p(new CBackgroundLayer );
                m_vLayers[nLayerNum] = std::move(p);
                m_vLayers[nLayerNum]->Load( resourceID );
            }

            void SetLayerSpeed( int nLayerNum, int xspeed, int yspeed )
            {
                m_vLayers[nLayerNum]->SetSpeed( xspeed, yspeed );
            }

            void Render( unique_ptr<CRenderer>& renderer ) override
            {
                for( auto& layer : m_vLayers )
                {
                    layer->Render( renderer, m_vPosition[0], m_vPosition[1] );
                }
            }

            void RenderDebug( unique_ptr<CRenderer>& renderer ) override
            {
                Render( renderer );
            }

            void Update( float fSeconds, float fRealSeconds ) override
            {
                DISCARD_UNUNSED_PARAMETER( fRealSeconds );
                // Update position and calculate new velocity
                m_vPosition[0] += GetSpeed()[0] * fSeconds;
                m_vPosition[1] += GetSpeed()[1] * fSeconds;
            }

        protected:
            // Store our layers
            vector< unique_ptr<CBackgroundLayer> > m_vLayers;
        private:
    };

    typedef CSingleton<CSharedResourceFactory<int, CScrollingBackground>> ScrollingBackgroundFactory;
}

#endif // SCROLLINGBACKGROUND_HPP
