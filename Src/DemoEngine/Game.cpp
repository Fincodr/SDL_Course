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

#include "Game.hpp"

namespace DemoEngine {

    /** \brief Ctor initializes graphics and sound systems. Sets framerate to 60fps and sets keyboard repeat delays to zero.
     */
    CGame::CGame() : m_Handlers(), m_fpsManager(), m_Timer(), m_mapNameToScene() {
        #ifdef DEBUGCTORS
        cout << "CGame ctor called!" << endl;
        #endif
    }

    CGame::~CGame() {
        #ifdef DEBUGCTORS
        cout << "CGame dtor called!" << endl;
        #endif
        CSingleton<CProperties>::Instance()->Property( "Game", "Running" ) = (bool)false;
    }

    void CGame::Initialize() {

        auto& properties = CSingleton<CProperties>::Instance();

        // Set default properties
        properties->Property( "Game", "Quit" ) = (bool)false;
        properties->Property( "Game", "Running" ) = (bool)true;
        properties->Property( "Game", "Speed" ) = (float)1.0f;
        properties->Property( "Game", "Music" ) = (bool)true;

        // Initialize Systems
        auto& renderer = CSingleton<CRenderer>::Instance();
        renderer->Init();

        auto& sound = CSingleton<CSoundServer>::Instance();
        sound->Init();

        // Default to 60fps
        SDL_initFramerate( &m_fpsManager );
        SDL_setFramerate( &m_fpsManager, 60 );

        // Set key delay to zero
        SDL_EnableKeyRepeat( 0, 0 );

        int w = (Uint32)properties->Property("Video","Width", (Uint32)640);
        int h = (Uint32)properties->Property("Video","Height", (Uint32)480);
        int b = (Uint32)properties->Property("Video","Bits", (Uint32)32);
        bool fs = (bool)properties->Property("Video","FullScreen", (bool)false);

        int flag = 0;
        if ( fs )
            flag = SDL_FULLSCREEN;

        renderer->OpenWindow( w, h, b, SDL_HWSURFACE|SDL_DOUBLEBUF|flag );
        renderer->SetClearColor( 0, 0, 0 );
    }

    /** \brief Gets the current scene listing (std::map)
     *
     * \return mapNameToScene_t&
     *
     */
    mapNameToScene_t& CGame::GetScenes() {
        return ( m_mapNameToScene );
    }

    /** \brief Adds a new scene to the scene listing
     *
     * \param iSceneID SCENEID_t
     * \param scene unique_ptr<CScene>&
     * \return void
     *
     */
    void CGame::AddScene( SCENEID_t iSceneID, unique_ptr<CScene>& scene ) {
        if ( !scene->IsInitialized() ) scene->Initialize();
        scene->SetID( iSceneID );
        m_mapNameToScene.insert( make_pair(iSceneID, move(scene)) );
    }

    /** \brief Removes scene from the scene listing
     *
     * \param iSceneID SCENEID_t
     * \return void
     *
     */
    void CGame::RemoveScene( SCENEID_t iSceneID ) {
        auto it = m_mapNameToScene.find( iSceneID );
        if ( it != m_mapNameToScene.end() ) {
            m_mapNameToScene.erase( it );
        }
    }

    /** \brief Gets a scene from the scene listing by the scene id
     *
     * \param iSceneID SCENEID_t
     * \return unique_ptr<CScene>&
     *
     */
    unique_ptr<CScene>& CGame::GetScene( SCENEID_t iSceneID ) throw(runtime_error) {
        auto it = m_mapNameToScene.find( iSceneID );
        if ( it != m_mapNameToScene.end() )
            return( it->second );
        else
            throw runtime_error("GetScene -> Can't find scene!" );
    }

    /** \brief Loads a specified scene into memory
     *
     * \param iSceneID SCENEID_t
     * \return void
     *
     */
    void CGame::LoadScene( SCENEID_t iSceneID )
    {
        auto& scene = GetScene( iSceneID );
        if ( !scene->IsLoaded() )
            scene->Load();
        scene->SetState( 0 );
    }

    /** \brief Loads specified scene into memory and also sets the running flag to true
     *
     * \param iSceneID SCENEID_t
     * \return void
     *
     */
    void CGame::LoadAndRunScene( SCENEID_t iSceneID )
    {
        auto& scene = GetScene( iSceneID );
        if ( !scene->IsLoaded() )
            scene->Load();
        scene->SetState( 0 );
        scene->SetRunning( true );
    }

    /** \brief Starts specified scene by setting running flag to true
     *
     * \param iSceneID SCENEID_t
     * \return void
     *
     */
    void CGame::StartScene( SCENEID_t iSceneID )
    {
        auto& scene = GetScene( iSceneID );
        if ( !scene->IsLoaded() )
            scene->Load();
        scene->SetRunning( true );
    }

    /** \brief Stops specified scene by setting running flag to false
     *
     * \param iSceneID SCENEID_t
     * \return void
     *
     */
    void CGame::StopScene( SCENEID_t iSceneID )
    {
        auto& scene = GetScene( iSceneID );
        scene->SetRunning( false );
    }

    /** \brief Sets the scene state
     *
     * \param iSceneID SCENEID_t
     * \return void
     *
     */
    void CGame::SetSceneState( SCENEID_t iSceneID, STATE_t iStateID )
    {
        auto& scene = GetScene( iSceneID );
        scene->SetState( iStateID );
    }

    /** \brief Stops and unloads specified scene from memory (if supported by scene)
     *
     * \param iSceneID SCENEID_t
     * \return void
     *
     */
    void CGame::StopAndUnloadScene( SCENEID_t iSceneID )
    {
        auto& scene = GetScene( iSceneID );
        scene->SetRunning( false );
        if ( scene->IsLoaded() )
            scene->Unload();
    }

    /** \brief Unloads specified scene from memory (if supported by scened)
     *
     * \return void
     * \param iSceneID SCENEID_t
     *
     */
    void CGame::UnloadScene( SCENEID_t iSceneID )
    {
        auto& scene = GetScene( iSceneID );
        if ( scene->IsLoaded() )
            scene->Unload();
    }

    void CGame::SendCustomEvent( int nEventCode )
    {
        // Send event about state change
        SDL_Event event;
        event.type = SDL_USEREVENT;
        event.user.code = EVENTTYPE::CUSTOM_EVENT;
        event.user.data1 = reinterpret_cast<void *>(nEventCode);
        event.user.data2 = reinterpret_cast<void *>(-1);            // For main game "scene" the id is always -1
        SDL_PushEvent(&event);
    }


    // Running state management functions

    bool CGame::bIsRunning() const {
        return ( m_bRunning );
    }

    void CGame::SetRunning(const bool bRunning) {
        m_bRunning = bRunning;
    }

    // State management functions

    void CGame::SetState(const STATE_t state) {
        #ifdef DEBUG
        cout << "Info: GLOBAL GAME STATE set to " << (int)state << endl;
        #endif
        m_State = state;
    }

    STATE_t CGame::GetState() const {
        return( m_State );
    }

    /** \brief Handles SDL events and calls first the local handlers and then active scenes handlers
     *
     * \return void
     *
     */
    void CGame::HandleEvents()
    {
        // Poll events
        SDL_Event ev;
        while ( SDL_PollEvent( &ev ) )
        {
            switch( ev.type )
            {
                case SDL_KEYUP:
                    OnKeyUp( ev );  // Call host application first
                    // Call handler on active scenes
                    for( auto& scene : m_mapNameToScene ) {
                        if ( scene.second->IsRunning() ) scene.second->OnKeyUp( ev );
                    }
                    break;
                case SDL_KEYDOWN:
                    OnKeyDown( ev );
                    // Call handler on active scenes
                    for( auto& scene : m_mapNameToScene ) {
                        if ( scene.second->IsRunning() ) scene.second->OnKeyDown( ev );
                    }
                    break;
                case SDL_MOUSEMOTION:
                    OnMouseMotion( ev );
                    // Call handler on active scenes
                    for( auto& scene : m_mapNameToScene ) {
                        if ( scene.second->IsRunning() ) scene.second->OnMouseMotion( ev );
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    OnMousePress( ev );
                    // Call handler on active scenes
                    for( auto& scene : m_mapNameToScene ) {
                        if ( scene.second->IsRunning() ) scene.second->OnMousePress( ev );
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    OnMouseRelease( ev );
                    // Call handler on active scenes
                    for( auto& scene : m_mapNameToScene ) {
                        if ( scene.second->IsRunning() ) scene.second->OnMouseRelease( ev );
                    }
                    break;
                case SDL_QUIT:
                    OnQuitEvent( ev );
                    // Call handler on active scenes
                    for( auto& scene : m_mapNameToScene ) {
                        if ( scene.second->IsRunning() ) scene.second->OnQuitEvent( ev );
                    }
                    break;
                case SDL_USEREVENT:
                    OnUserEvent( ev );
                    break;
                default:
                    break;
            }
        }
    }

    /** \brief Handles events, Updates and Rendering for local and active scenes
     *
     * \return bool
     *
     */
    bool CGame::Execute()
    {
        if ( !IsInitialized() ) { Initialize(); SetInitialized( true ); }

        /// START OF FRAME
        ///
        #ifdef DEBUG_PERFORMANCE
        CSingleton<CPerformanceCounter>::Instance()->StartFrame();
        #endif

        HandleEvents();

        /// UPDATE
        {
            #ifdef DEBUG_PERFORMANCE
            CSingleton<CPerformanceCounter>::Instance()->Get( PERFORMANCECOUNTERID::UPDATE ).SetStart();
            #endif

            m_Timer.SetSpeed( (float)CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) );
            m_Timer.Update();
            m_fLastFrameTime = m_Timer.GetPassedTimeReal();
            m_fTime += m_Timer.GetPassedTime();
            m_fRealTime += m_fLastFrameTime;
            Update( m_Timer.GetPassedTime(), m_Timer.GetPassedTimeReal() );
            for( auto& scene : m_mapNameToScene ) {
                if ( scene.second->IsRunning() ) scene.second->Update( );
                if ( scene.second->IsRunning() ) scene.second->Update( m_Timer.GetPassedTime(), m_Timer.GetPassedTimeReal() );
            }
            m_Timer.Reset();

            #ifdef DEBUG_PERFORMANCE
            CSingleton<CPerformanceCounter>::Instance()->Get( PERFORMANCECOUNTERID::UPDATE ).SetEnd();
            #endif
        }

        /// RENDER
        {
            #ifdef DEBUG_PERFORMANCE
            CSingleton<CPerformanceCounter>::Instance()->Get( PERFORMANCECOUNTERID::RENDER ).SetStart();
            #endif

            PreRender();
            Render();
            auto& renderer = CSingleton<CRenderer>::Instance();
            for( auto& scene : m_mapNameToScene ) {
                if ( scene.second->IsRunning() ) scene.second->Render( );
                if ( scene.second->IsRunning() ) scene.second->Render( renderer );
                if ( scene.second->IsRunning() ) scene.second->PostRender( );
            }
            PostRender();

            #ifdef DEBUG_PERFORMANCE
            CSingleton<CPerformanceCounter>::Instance()->Get( PERFORMANCECOUNTERID::RENDER ).SetEnd();
            #endif
        }

        /// END OF FRAME
        ///
        #ifdef DEBUG_PERFORMANCE
        CSingleton<CPerformanceCounter>::Instance()->EndFrame();
        float fps = ( CSingleton<CPerformanceCounter>::Instance()->GetFrameCount()/(float)(CSingleton<CPerformanceCounter>::Instance()->GetElapsedAsMilliseconds()) ) * 1000;
        cout << "\rFPS: " << fps << "   ";
        #endif

        SDL_framerateDelay( &m_fpsManager );

        return ( bIsRunning() );
    }

    // Local handlers

    void CGame::Update( float fSeconds, float fRealSeconds )
    {
        DISCARD_UNUNSED_PARAMETER( fSeconds );
        DISCARD_UNUNSED_PARAMETER( fRealSeconds );
        // Run the handler if exists
        SDL_Event ev;
        if ( m_Handlers["Update"].count(m_State) != 0 )
            m_Handlers["Update"][m_State](ev);
        // Run the any-state handler if exists
        if ( m_Handlers["Update"].count((int)STATE::ANY_STATE) != 0 )
            m_Handlers["Update"][(int)STATE::ANY_STATE](ev);
    }

    void CGame::PreRender()
    {
        auto& renderer = CSingleton<CRenderer>::Instance();
        renderer->Begin();
        renderer->ClearScreen();
        // Run the handler if exists
        SDL_Event ev;
        if ( m_Handlers["PreRender"].count(m_State) != 0 )
            m_Handlers["PreRender"][m_State](ev);
        // Run the any-state handler if exists
        if ( m_Handlers["PreRender"].count((int)STATE::ANY_STATE) != 0 )
            m_Handlers["PreRender"][(int)STATE::ANY_STATE](ev);
    }

    void CGame::PostRender()
    {
        auto& renderer = CSingleton<CRenderer>::Instance();
        // Run the handler if exists
        SDL_Event ev;
        if ( m_Handlers["PostRender"].count(m_State) != 0 )
            m_Handlers["PostRender"][m_State](ev);
        // Run the any-state handler if exists
        if ( m_Handlers["PostRender"].count((int)STATE::ANY_STATE) != 0 )
            m_Handlers["PostRender"][(int)STATE::ANY_STATE](ev);
        renderer->End();
    }

    void CGame::Render()
    {
        // Run the handler if exists
        SDL_Event ev;
        if ( m_Handlers["Render"].count(m_State) != 0 )
            m_Handlers["Render"][m_State](ev);
        // Run the any-state handler if exists
        if ( m_Handlers["Render"].count((int)STATE::ANY_STATE) != 0 )
            m_Handlers["Render"][(int)STATE::ANY_STATE](ev);
    }

    // Local event handlers

    void CGame::OnKeyUp( SDL_Event& ev )
    {
        // Run the handler if exists
        if ( m_Handlers["OnKeyUp"].count(m_State) != 0 )
            m_Handlers["OnKeyUp"][m_State](ev);
        // Run the any-state handler if exists
        if ( m_Handlers["OnKeyUp"].count((int)STATE::ANY_STATE) != 0 )
            m_Handlers["OnKeyUp"][(int)STATE::ANY_STATE](ev);
    }

    void CGame::OnKeyDown( SDL_Event& ev )
    {
        // Run the handler if exists
        if ( m_Handlers["OnKeyDown"].count(m_State) != 0 )
            m_Handlers["OnKeyDown"][m_State](ev);
        // Run the any-state handler if exists
        if ( m_Handlers["OnKeyDown"].count((int)STATE::ANY_STATE) != 0 )
            m_Handlers["OnKeyDown"][(int)STATE::ANY_STATE](ev);
    }

    void CGame::OnMouseMotion( SDL_Event& ev )
    {
        // Run the handler if exists
        if ( m_Handlers["OnMouseMotion"].count(m_State) != 0 )
            m_Handlers["OnMouseMotion"][m_State](ev);
        // Run the any-state handler if exists
        if ( m_Handlers["OnMouseMotion"].count((int)STATE::ANY_STATE) != 0 )
            m_Handlers["OnMouseMotion"][(int)STATE::ANY_STATE](ev);
    }

    void CGame::OnMousePress( SDL_Event& ev )
    {
        // Run the handler if exists
        if ( m_Handlers["OnMousePress"].count(m_State) != 0 )
            m_Handlers["OnMousePress"][m_State](ev);
        // Run the any-state handler if exists
        if ( m_Handlers["OnMousePress"].count((int)STATE::ANY_STATE) != 0 )
            m_Handlers["OnMousePress"][(int)STATE::ANY_STATE](ev);
    }

    void CGame::OnMouseRelease( SDL_Event& ev )
    {
        // Run the handler if exists
        if ( m_Handlers["OnMouseRelease"].count(m_State) != 0 )
            m_Handlers["OnMouseRelease"][m_State](ev);
        // Run the any-state handler if exists
        if ( m_Handlers["OnMouseRelease"].count((int)STATE::ANY_STATE) != 0 )
            m_Handlers["OnMouseRelease"][(int)STATE::ANY_STATE](ev);
    }

    void CGame::OnUserEvent( SDL_Event& ev )
    {
        #ifdef DEBUG
        cout << "[State: " << GetState() << "] -> Custom event from scene: " << ev.user.code << ", state: " << ev.user.data1 << endl;
        #endif
        // Run the handler if exists
        if ( m_Handlers["OnUserEvent"].count(m_State) != 0 )
            m_Handlers["OnUserEvent"][m_State](ev);
        // Run the any-state handler if exists
        if ( m_Handlers["OnUserEvent"].count((int)STATE::ANY_STATE) != 0 )
            m_Handlers["OnUserEvent"][(int)STATE::ANY_STATE](ev);
    }

    void CGame::OnQuitEvent( SDL_Event& ev )
    {
        SetRunning( false );
        // Run the handler if exists
        if ( m_Handlers["OnQuitEvent"].count(m_State) != 0 )
            m_Handlers["OnQuitEvent"][m_State](ev);
        // Run the any-state handler if exists
        if ( m_Handlers["OnQuitEvent"].count((int)STATE::ANY_STATE) != 0 )
            m_Handlers["OnQuitEvent"][(int)STATE::ANY_STATE](ev);
    }
}
