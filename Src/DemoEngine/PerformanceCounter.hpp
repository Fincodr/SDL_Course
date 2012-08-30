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

#ifndef PERFORMANCECOUNTER_HPP
#define PERFORMANCECOUNTER_HPP

#include <chrono>
#include <ratio>
#include <unordered_map>
#include "Singleton.hpp"

namespace DemoEngine {

    using std::unordered_map;
    using std::chrono::high_resolution_clock;
    using std::ratio;

    struct CPerformanceCounterItem
    {
        high_resolution_clock::time_point Start;
        high_resolution_clock::time_point End;
        high_resolution_clock::duration Duration;
        high_resolution_clock::duration DurationMin;
        high_resolution_clock::duration DurationMax;
        high_resolution_clock::duration DurationAcc;
        void SetStart() { this->Start = high_resolution_clock::now(); }
        void SetEnd() {
            this->End = high_resolution_clock::now();
            this->Duration = this->End - this->Start;
            this->DurationAcc += this->Duration;
            if ( this->Duration < DurationMin )
                DurationMin = this->Duration;
            if ( this->Duration > DurationMax )
                DurationMax = this->Duration;
        }
        float GetAsMilliseconds() {
            return ( std::chrono::duration<double,std::milli>( Duration ).count() );
        }
    };

    typedef unordered_map<int,CPerformanceCounterItem> perflist_t;

    class CPerformanceCounter
    {
        friend class CSingleton<CPerformanceCounter>;

        public:

            CPerformanceCounter() {
                m_tpMin.Duration = high_resolution_clock::duration::max();
                m_tpMax.Duration = high_resolution_clock::duration::max();
            }

            virtual ~CPerformanceCounter() {};

            virtual void StartFrame() {
                m_tpCurrent.Start = high_resolution_clock::now();
                if ( !m_bStarted )
                {
                    m_tpElapsed = m_tpCurrent;
                    m_bStarted = true;
                }
                m_nFrameCount++;
            }

            virtual void EndFrame() {
                m_tpCurrent.End = high_resolution_clock::now();
                m_tpCurrent.Duration = m_tpCurrent.End - m_tpCurrent.Start;
                m_durationAcc += m_tpCurrent.Duration;
                if ( m_tpCurrent.Duration < m_durationMin )
                    m_durationMin = m_tpCurrent.Duration;
                if ( m_tpCurrent.Duration > m_durationMax )
                    m_durationMax = m_tpCurrent.Duration;
            }

            virtual CPerformanceCounterItem& Get( int nCounterID ) {
                return m_Counters[nCounterID];
            }

            virtual float GetAsMilliseconds() {
                return ( std::chrono::duration<double,std::milli>( m_tpCurrent.Duration ).count() );
            }

            virtual float GetElapsedAsMilliseconds() {
                m_tpElapsed.SetEnd();
                return ( std::chrono::duration<double,std::milli>( m_tpElapsed.Duration ).count() );
            }

            virtual unsigned int GetFrameCount() {
                return m_nFrameCount;
            }

        protected:
        private:
            unsigned int m_nFrameCount = 0;
            CPerformanceCounterItem m_tpCurrent;
            CPerformanceCounterItem m_tpMin;
            CPerformanceCounterItem m_tpMax;
            CPerformanceCounterItem m_tpElapsed;
            bool m_bStarted = false;
            high_resolution_clock::duration m_durationMin;
            high_resolution_clock::duration m_durationMax;
            high_resolution_clock::duration m_durationAcc;
            perflist_t m_Counters;

    };

}

#endif // PERFORMANCECOUNTER_HPP
