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

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>
#include <chrono>
#include <functional>
#include <cstdlib>
#include "Mutex.hpp"

namespace DemoEngine {

    using std::mt19937;
    using std::uniform_int_distribution;

    class CRandom
    {
        friend class CSingleton<CRandom>;
        public:
            CRandom() : m_pGenerator(nullptr), m_Mutex() {
                unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
                #ifdef DEBUG
                seed = 0;
                #endif
                m_pGenerator = new Generator(seed);
            }
            virtual ~CRandom() {
                if ( m_pGenerator )
                    delete m_pGenerator;
                m_pGenerator = nullptr;
            }
            inline unsigned int nextNumber() {
                static Distribution dist(0,RAND_MAX);
                return dist(*m_pGenerator);
            }
            inline unsigned int nextSafeNumber() {
                m_Mutex.Lock();
                unsigned int tmp = nextNumber();
                m_Mutex.Release();
                return tmp;
            }
            CRandom(const CRandom& other)=delete;             // Because we have pointer datamembers this class can't be automatically copied correctly.
            CRandom& operator=(const CRandom& other)=delete;  // Because we have pointer datamembers this class can't be automatically copied correctly.
        protected:
        private:
            typedef mt19937 Generator;
            typedef uniform_int_distribution<int> Distribution;
            Generator* m_pGenerator;
            CMutex m_Mutex;
    };

    #undef rand
    #define rand CSingleton<CRandom>::Instance()->nextNumber
    #define saferand CSingleton<CRandom>::Instance()->nextSafeNumber
}

#endif // RANDOM_HPP
