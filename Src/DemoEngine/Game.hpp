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

#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include <memory>
#include <map>
#include <SDL.h>
#include <SDL_framerate.h>
#include "Macros.hpp"
#include "Assert.hpp"
#include "Renderer.hpp"
#include "SoundServer.hpp"
#include "Scene.hpp"
#include "Font.hpp"
#include "Timer.hpp"
#include "CollisionDetector.hpp"
#include "Properties.hpp"
#include "Random.hpp"

#ifdef DEBUG_PERFORMANCE
#include "PerformanceCounter.hpp"
#include "PerformanceCounterIDs.hpp"
#endif

#include "EventTypes.hpp"

namespace DemoEngine {

    using std::map;
    using std::string;
    using std::move;

    typedef map<SCENEID_t, unique_ptr<CScene>> mapNameToScene_t;
    typedef unsigned int STATE_t;

    class CGame : public CInitializable
    {
        public:

            typedef enum class {
                ANY_STATE = 9999
            } STATE;

            CGame();
            virtual ~CGame();

            // SDL Event handlers
            virtual void OnKeyUp( SDL_Event& ev );
            virtual void OnKeyDown( SDL_Event& ev );
            virtual void OnMouseMotion( SDL_Event& ev );
            virtual void OnMousePress( SDL_Event& ev );
            virtual void OnMouseRelease( SDL_Event& ev );
            virtual void OnUserEvent( SDL_Event& ev );
            virtual void OnQuitEvent( SDL_Event& ev );

            // Event handlers
            virtual void Initialize();
            virtual void InitHandlers() {};
            virtual void PreRender();
            virtual void PostRender();
            virtual void Render();
            virtual void Update( float fSeconds, float fRealSeconds );
            virtual void HandleEvents();
            virtual bool Execute();

            virtual float GetTime() const { return m_fTime; }
            virtual float GetRealTime() const { return m_fRealTime; }
            virtual float GetFrameTime() const { return m_fLastFrameTime; }

            // Scene handling
            virtual void AddScene( SCENEID_t iSceneID, unique_ptr<CScene>& scene );
            virtual void RemoveScene( SCENEID_t iSceneID );
            virtual unique_ptr<CScene>& GetScene( SCENEID_t iSceneID ) throw(runtime_error);
            virtual mapNameToScene_t& GetScenes();
            virtual void SetSceneState( SCENEID_t iSceneID, STATE_t iStateID );
            virtual void LoadScene( SCENEID_t iSceneID );
            virtual void LoadAndRunScene( SCENEID_t iSceneID );
            virtual void StartScene( SCENEID_t iSceneID );
            virtual void StopScene( SCENEID_t iSceneID );
            virtual void StopAndUnloadScene( SCENEID_t iSceneID );
            virtual void UnloadScene( SCENEID_t iSceneID );

            // Running state management functions
            bool bIsRunning() const;
            void SetRunning(const bool bRunning);

            // State management functions
            void SetState(const STATE_t state);
            STATE_t GetState() const;

            // Event management
            void SendCustomEvent( int nEventCode );

            /// Hashtables to store event handler callbacks (i.e. labmda functions)
            // store State to lambda function mappings
            typedef map<STATE_t, std::function<void(SDL_Event&)>> StateAndFunction_t;
            // store Event name to state with lambda function
            typedef unordered_map<string, StateAndFunction_t> NameAndStateMap_t;
            NameAndStateMap_t m_Handlers;
            STATE_t m_State = 0;
            FPSmanager m_fpsManager;

        protected:
        private:
            bool m_bInitialized = false;
            float m_fLastFrameTime = 0.0f;
            float m_fTime = 0.0f;
            float m_fRealTime = 0.0f;
            bool m_bRunning = true;   // C++11 allows member variable initialization here
            CTimer m_Timer;
            mapNameToScene_t m_mapNameToScene;
    };

}


#endif // GAME_HPP
