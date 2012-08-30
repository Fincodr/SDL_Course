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

#include "Scene.hpp"

namespace DemoEngine {

    void CScene::Update( float fSeconds, float fRealSeconds ) {
        auto& lst = GetUpdateables();
        for ( auto& p : lst )
        {
            p.second->Update( fSeconds, fRealSeconds );
        }
    }

    void CScene::Render( unique_ptr<CRenderer>& renderer ) {
        // Render Pre-Renderables
        {
            auto& lst = GetPreRenderables();
            for ( auto& p : lst )
            {
                #ifdef DEBUG
                p.second->RenderDebug( renderer );
                #else
                p.second->Render( renderer );
                #endif
            }
        }
        // Render Renderables
        {
            auto& lst = GetRenderables();
            for ( auto& p : lst )
            {
                #ifdef DEBUG
                p.second->RenderDebug( renderer );
                #else
                p.second->Render( renderer );
                #endif
            }
        }
        // Render Post-Renderables
        {
            auto& lst = GetPostRenderables();
            for ( auto& p : lst )
            {
                #ifdef DEBUG
                p.second->RenderDebug( renderer );
                #else
                p.second->Render( renderer );
                #endif
            }
        }
    }

    void CScene::PreRender( unique_ptr<CRenderer>& renderer ) {
        renderer->Begin();
        renderer->ClearScreen();
    }

    void CScene::PostRender( unique_ptr<CRenderer>& renderer ) {
        renderer->End();
    }

    void CScene::SetState( const STATE_t state ) {
        m_State = state;
        SendStateChangeEvent(state);
        if ( state == 999 ) {
            SetRunning( false );
        }
    }

    void CScene::SendStateChangeEvent( const STATE_t state )
    {
        // Send event about state change
        SDL_Event event;
        event.type = SDL_USEREVENT;
        event.user.code = EVENTTYPE::SCENE_EVENT;
        event.user.data1 = reinterpret_cast<void *>(state);
        event.user.data2 = reinterpret_cast<void *>(GetID());
        SDL_PushEvent(&event);
    }

    void CScene::SendCustomEvent( int nEventCode )
    {
        // Send event about state change
        SDL_Event event;
        event.type = SDL_USEREVENT;
        event.user.code = EVENTTYPE::CUSTOM_EVENT;
        event.user.data1 = reinterpret_cast<void *>(nEventCode);
        event.user.data2 = reinterpret_cast<void *>(GetID());
        SDL_PushEvent(&event);
    }

    STATE_t CScene::GetState() const {
        return( m_State );
    }

    void CScene::SetRunning( bool bRunning )
    {
        if ( IsRunning() != bRunning ) {
            m_bIsRunning = bRunning;
            if ( m_bIsRunning )
                OnEnter();
            else
                OnExit();
        }
    }

    void CScene::OnKeyUp( SDL_Event& ev )
    {
        // Run the handler if exists
        if ( m_Handlers["OnKeyUp"].count(m_State) != 0 )
            m_Handlers["OnKeyUp"][m_State](ev);
    }

    void CScene::OnKeyDown( SDL_Event& ev )
    {
        // Run the handler if exists
        if ( m_Handlers["OnKeyDown"].count(m_State) != 0 )
            m_Handlers["OnKeyDown"][m_State](ev);
    }

    void CScene::OnMouseMotion( SDL_Event& ev )
    {
        // Run the handler if exists
        if ( m_Handlers["OnMouseMotion"].count(m_State) != 0 )
            m_Handlers["OnMouseMotion"][m_State](ev);
    }

    void CScene::OnMousePress( SDL_Event& ev )
    {
        // Run the handler if exists
        if ( m_Handlers["OnMousePress"].count(m_State) != 0 )
            m_Handlers["OnMousePress"][m_State](ev);
    }

    void CScene::OnMouseRelease( SDL_Event& ev )
    {
        // Run the handler if exists
        if ( m_Handlers["OnMouseRelease"].count(m_State) != 0 )
            m_Handlers["OnMouseRelease"][m_State](ev);
    }

    void CScene::Update( )
    {
        // Run the handler if exists
        SDL_Event ev;
        if ( m_Handlers["Update"].count(m_State) != 0 )
            m_Handlers["Update"][m_State](ev);
    }

    void CScene::Render( )
    {
        SDL_Event ev;
        if ( m_Handlers["Render"].count(m_State) != 0 )
            m_Handlers["Render"][m_State](ev);
    }

    void CScene::PostRender( )
    {
        SDL_Event ev;
        if ( m_Handlers["PostRender"].count(m_State) != 0 )
            m_Handlers["PostRender"][m_State](ev);
    }

    void CScene::PreRender( )
    {
        SDL_Event ev;
        if ( m_Handlers["PreRender"].count(m_State) != 0 )
            m_Handlers["PreRender"][m_State](ev);
    }
}
