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

#ifndef RESOURCEFACTORY_HPP
#define RESOURCEFACTORY_HPP

#include <memory>
#include <unordered_map>
#include <iostream>
#include <SDL.h>

namespace DemoEngine {

    using std::unordered_map;
    using std::unique_ptr;
    using std::shared_ptr;
    using std::make_shared;
    using std::cout;
    using std::endl;

    class CMutex;

    template< typename Key, typename T >
    class CResourceFactory
    {
        typedef unordered_map< Key, unique_ptr<T> > ResourceList_t;

        public:
            CResourceFactory() : m_ResourceList() {
                #ifdef DEBUGCTORS
                cout << "Called CResourceFactory ctor" << endl;
                #endif
            }
            virtual ~CResourceFactory() {
                #ifdef DEBUGCTORS
                cout << "Called CResourceFactory dtor" << endl;
                #endif
            }

            size_t Count() const {
                return m_ResourceList.size();
            }

            void Initialize( Key k ) {
                Get( k );
            }

            unique_ptr<T>& Get( Key k ) {
                auto it = m_ResourceList.find( k );
                if ( it != m_ResourceList.end() ) {
                    return it->second;
                }
                else {
                    m_ResourceList[k]=unique_ptr<T>(new T);
                    return m_ResourceList[k];
                }
            }

            void print() {
                #ifdef DEBUG
                for (auto& item : m_ResourceList )
                {
                    cout << " \"" << item.first << "\" (unique_ptr)" << endl;
                }
                #endif
            }

        protected:
        private:
            ResourceList_t m_ResourceList;
    };

    template< typename Key, typename T >
    class CMultithreadedResourceFactory
    {
        typedef unordered_map< Key, unique_ptr<T> > ResourceList_t;

        public:
            CMultithreadedResourceFactory() : m_ResourceList() {
                #ifdef DEBUGCTORS
                cout << "Called CMultithreadedResourceFactory ctor" << endl;
                #endif
                m_pMutex = SDL_CreateMutex();
            }
            virtual ~CMultithreadedResourceFactory() {
                #ifdef DEBUGCTORS
                cout << "Called CMultithreadedResourceFactory dtor" << endl;
                #endif
                if ( m_pMutex ) {
                    SDL_DestroyMutex( m_pMutex );
                    m_pMutex = nullptr;
                }
            }

            size_t Count() const {
                return m_ResourceList.size();
            }

            void Initialize( Key k ) {
                Get( k );
            }

            unique_ptr<T>& Get( Key k ) {
                SDL_LockMutex( m_pMutex );
                auto it = m_ResourceList.find( k );
                if ( it != m_ResourceList.end() ) {
                    SDL_UnlockMutex( m_pMutex );
                    return it->second;
                }
                else {
                    m_ResourceList[k]=unique_ptr<T>(new T);
                    SDL_UnlockMutex( m_pMutex );
                    return m_ResourceList[k];
                }
            }

            void print() {
                #ifdef DEBUG
                for (auto& item : m_ResourceList )
                {
                    cout << " \"" << item.first << "\" (unique_ptr)" << endl;
                }
                #endif
            }

            CMultithreadedResourceFactory(const CMultithreadedResourceFactory& other)=delete;             // Because we have pointer datamembers this class can't be automatically copied correctly.
            CMultithreadedResourceFactory& operator=(const CMultithreadedResourceFactory& other)=delete;  // Because we have pointer datamembers this class can't be automatically copied correctly.
        protected:
        private:
            ResourceList_t m_ResourceList;
            SDL_mutex* m_pMutex = nullptr;
    };

    template< typename Key, typename T >
    class CSharedResourceFactory
    {
        typedef unordered_map< Key, shared_ptr<T> > SharedResourceList_t;

        public:
            CSharedResourceFactory() : m_ResourceList() {
                #ifdef DEBUGCTORS
                cout << "Called CSharedResourceFactory ctor" << endl;
                #endif
            }
            virtual ~CSharedResourceFactory() {
                #ifdef DEBUGCTORS
                cout << "Called CSharedResourceFactory dtor" << endl;
                #endif
            }

            size_t Count() const {
                return m_ResourceList.size();
            }

            void Initialize( Key k ) {
                Get( k );
            }

            shared_ptr<T>& Get( Key k ) {
                auto it = m_ResourceList.find( k );
                if ( it != m_ResourceList.end() ) {
                    return it->second;
                }
                else {
                    m_ResourceList[k]=make_shared<T>();
                    return m_ResourceList[k];
                }
            }

            void print() {
                #ifdef DEBUG
                for (auto& item : m_ResourceList )
                {
                    cout << " \"" << item.first << "\" (shared_ptr)" << endl;
                }
                #endif
            }

        protected:
        private:
            SharedResourceList_t m_ResourceList;
    };


}

#endif // RESOURCEFACTORY_HPP
