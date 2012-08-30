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

#ifndef TIMER_HPP
#define TIMER_HPP

#include <SDL.h>
#include "Singleton.hpp"
#include "ResourceFactory.hpp"

namespace DemoEngine
{
    class CTimer
    {
        public:
            CTimer()
            {
                Reset();
            }

            virtual ~CTimer()=default;

            void Update()
            {
                m_CurrentTime = SDL_GetTicks();
            }

            void Reset()
            {
                m_PreviousTime = m_CurrentTime = SDL_GetTicks();
            }

            float GetPassedTime()
            {
                return ((m_CurrentTime - m_PreviousTime) * 0.001f) * m_fSpeed;
            }

            float GetPassedTimeReal()
            {
                return ((m_CurrentTime - m_PreviousTime) * 0.001f);
            }

            Uint32 GetPassedTimeMS()
            {
                return (m_CurrentTime - m_PreviousTime) * m_fSpeed;
            }

            void SetSpeed( float fSpeed )
            {
                m_fSpeed = fSpeed;
            }

        protected:
        private:
            Uint32 m_CurrentTime = 0;
            Uint32 m_PreviousTime = 0;
            float m_fSpeed = 0.0f;
    };

    typedef CSingleton<CResourceFactory<int, CTimer>> TimerFactory;
}

#endif // TIMER_HPP
