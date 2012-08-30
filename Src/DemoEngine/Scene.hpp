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

#ifndef SCENE_HPP
#define SCENE_HPP

#include <unordered_map>
#include <memory>
#include <algorithm>
#include <SDL.h>
#include "Macros.hpp"
#include "Initializable.hpp"
#include "Renderer.hpp"
#include "IRenderable.hpp"
#include "IUpdateable.hpp"
#include "GameObject.hpp"
#include "CollisionDetector.hpp"
#include "EventTypes.hpp"

namespace DemoEngine {

    using std::unique_ptr;
    using std::shared_ptr;
    using std::for_each;
    using std::unordered_map;

    typedef unsigned int SCENEID_t;

    typedef unordered_map<Uint32, shared_ptr<IRenderable>> RenderableList_t;
    typedef unordered_map<Uint32, shared_ptr<IUpdateable>> UpdateableList_t;

    typedef unsigned int STATE_t;

    class CScene : public IRenderable, public IUpdateable, public CInitializable
    {
        public:
            CScene() {
                #ifdef DEBUGCTORS
                cout << "CScene ctor called" << endl;
                #endif
            }
            virtual ~CScene() {
                #ifdef DEBUGCTORS
                cout << "CScene dtor called" << endl;
                #endif
                if ( IsLoaded() ) Unload();
            }

            inline RenderableList_t& GetPreRenderables() { return m_umapPreRenderables; }
            inline RenderableList_t& GetRenderables() { return m_umapRenderables; }
            inline RenderableList_t& GetPostRenderables() { return m_umapPostRenderables; }
            inline UpdateableList_t& GetUpdateables() { return m_umapUpdateables; }

            // can be implemented on subclass
            virtual void OnKeyUp( SDL_Event& ev );
            virtual void OnKeyDown( SDL_Event& ev );
            virtual void OnMouseMotion( SDL_Event& ev );
            virtual void OnMousePress( SDL_Event& ev );
            virtual void OnMouseRelease( SDL_Event& ev );
            virtual void OnUserEvent( SDL_Event& ev ) { DISCARD_UNUNSED_PARAMETER( ev ); }
            virtual void OnQuitEvent( SDL_Event& ev ) { DISCARD_UNUNSED_PARAMETER( ev ); }

            // Updateables
            virtual void Update( float fSeconds, float fRealSeconds );

            // Renderables
            virtual void PostRender( unique_ptr<CRenderer>& renderer );
            virtual void PreRender( unique_ptr<CRenderer>& renderer );
            virtual void Render( unique_ptr<CRenderer>& renderer );
            virtual void RenderDebug( unique_ptr<CRenderer>& r ) { DISCARD_UNUNSED_PARAMETER( r ); }

            virtual void Update( );

            virtual void PreRender( );
            virtual void Render( );
            virtual void PostRender( );

            inline bool IsRunning() { return m_bIsRunning; }
            void SetRunning( bool bRunning );

            inline bool IsLoaded() { return m_bIsLoaded; }
            inline void SetLoaded( bool bLoaded ) { m_bIsLoaded = bLoaded; }

            inline Uint8 GetAlpha() { return m_iAlpha; }
            inline void SetAlpha( Uint8 a ) { m_iAlpha = a; }

            inline void SetID( SCENEID_t id ) { m_iSceneID = id; }
            inline SCENEID_t GetID() { return m_iSceneID; }

            virtual void InitHandlers() {};

            virtual void OnEnter() {};
            virtual void OnExit() {};
            virtual void Load() {};
            virtual void Unload() {};

            // State handling
            void SetState(const STATE_t state);
            STATE_t GetState() const;
            STATE_t m_State = 0;

            // Event handling
            void SendStateChangeEvent( const STATE_t state );
            void SendCustomEvent( int nEventCode );

            /// Hashtables to store event handler callbacks (i.e. labmda functions)
            // store State to lambda function mappings
            typedef unordered_map<STATE_t, std::function<void(SDL_Event&)>> StateAndFunction_t;
            // store Event name to state with lambda function
            typedef unordered_map<string, StateAndFunction_t> NameAndStateMap_t;
            NameAndStateMap_t m_Handlers = {};

        protected:
        private:
            RenderableList_t m_umapPreRenderables = {};
            RenderableList_t m_umapRenderables = {};
            RenderableList_t m_umapPostRenderables = {};
            UpdateableList_t m_umapUpdateables = {};
            bool m_bIsRunning = false;
            bool m_bIsLoaded = false;
            SCENEID_t m_iSceneID = 0;
            Uint8 m_iAlpha = 255;
    };

}

#endif // SCENE_HPP
