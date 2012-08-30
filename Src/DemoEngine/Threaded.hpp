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

#ifndef THREADED_HPP
#define THREADED_HPP

#include <stdexcept>
#include <iostream>
#include "Singleton.hpp"
#include "ResourceFactory.hpp"
#include "UniqueID.hpp"

namespace DemoEngine {

    using std::cout;
    using std::endl;

    class CThreaded
    {
        public:
            CThreaded() : m_ThreadID(CSingleton<CUniqueID>::Instance()->getID()) {
                #ifdef DEBUGCTORS
                cout << "CThreaded ctor called for #" << m_ThreadID << endl;
                #endif
            }
            virtual ~CThreaded() {
                #ifdef DEBUGCTORS
                cout << "CThreaded dtor called for #" << m_ThreadID << endl;
                #endif
            }
            static int StaticThreadProc(void* pParam) { return ((CThreaded*)pParam)->Execute(); }
            virtual int Execute()=0;
            virtual void StartThread() {
                if ( !m_bThreadRunning ) {
                    SetThreadRunning( true );
                    m_pThread = SDL_CreateThread( StaticThreadProc, this );
                }
            }
            virtual void StopThread() {
                SetThreadRunning( false );
            }
            virtual void WaitThread() {
                SDL_WaitThread( m_pThread, NULL );
            }
            virtual void StopAndWaitThread() {
                StopThread();
                WaitThread();
            }
            virtual void KillThread() {
                if ( m_bThreadRunning ) {
                    SDL_KillThread( m_pThread );
                    SetThreadRunning( false );
                }
            }
            bool IsThreadRunning() const { return m_bThreadRunning; }
            void SetThreadRunning( bool bRunning ) {
                m_bThreadRunning = bRunning;
            }
            unsigned int GetThreadID() { return m_ThreadID; }
            CThreaded(const CThreaded& other)=delete;             // Because we have pointer datamembers this class can't be automatically copied correctly.
            CThreaded& operator=(const CThreaded& other)=delete;  // Because we have pointer datamembers this class can't be automatically copied correctly.
        protected:
        private:
            bool m_bThreadRunning = false;
            unsigned int m_ThreadID;
            SDL_Thread* m_pThread = nullptr;
    };

}


#endif // THREADED_HPP
