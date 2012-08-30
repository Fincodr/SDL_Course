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

#include "SceneHelp.hpp"

void SceneHelp::Initialize() {
    cout << "SceneHelp::Initialize() called" << endl;
    InitHandlers();
    SetInitialized( true );
}

void SceneHelp::Load()
{
    auto& renderer = CSingleton<CRenderer>::Instance();
    auto screen = renderer->GetScreen();
    m_iScreenW = screen->w;
    m_iScreenH = screen->h;

    auto& noteText = TextFactory::Instance()->Get( RESOURCE::TEXT_SPACE_TO_CONTINUE );
    m_noteImgWidth = noteText->GetSurface()->w;

    SetLoaded( true );
}

void SceneHelp::OnEnter()
{
    m_iHelpPage = 0;
    m_iScreenAlpha = 0;
    m_bDisplayNote = false;

    SetState( (int)STATE::START );
}

void SceneHelp::InitHandlers()
{
    ///////////////////////////////////////////////////
    //
    // Update
    //
    ///////////////////////////////////////////////////
    m_Handlers["Update"] = {
        {
            //
            // STATE::START
            //
            (int)STATE::START,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                SetState( (int)STATE::FADE_IN );
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->Reset();
            }
        },
        {
            //
            // STATE::FADE_IN
            //
            (int)STATE::FADE_IN,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                // Calculate Fade-IN
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->Update();
                m_iScreenAlpha = Math::Interpolation::linearTween<float,float>( TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->GetPassedTimeReal(), 0, 255, 1.0f );
                if ( m_iScreenAlpha >= 255 )
                {
                    SetState( (int)STATE::DISPLAY );
                    m_bDisplayNote = true;
                }
            }

        },
        {
            //
            // STATE::FADE_TO_PAGE2
            //
            (int)STATE::FADE_TO_PAGE2,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                // Calculate Fade-IN
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->Update();
                m_iScreenAlpha = Math::Interpolation::linearTween<float,float>( TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->GetPassedTimeReal(), 0, 255, 1.0f );
                if ( m_iScreenAlpha >= 255 )
                {
                    SetState( (int)STATE::DISPLAY_PAGE2 );
                    m_bDisplayNote = true;
                }
            }

        },
        {
            //
            // STATE::FADE_OUT
            //
            (int)STATE::FADE_OUT,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                // Calculate Fade-OUT
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->Update();
                m_iScreenAlpha = Math::Interpolation::linearTween<float,float>( TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->GetPassedTimeReal(), 255, -255, 1.0f );
                if ( m_iScreenAlpha <= 0 )
                {
                    SetState( (int)STATE::END );
                }
            }

        }
    };

    ///////////////////////////////////////////////////
    //
    // OnKeyDown
    //
    ///////////////////////////////////////////////////
    m_Handlers["OnKeyDown"] = {
        {
            //
            // STATE::DISPLAY
            //
            (int)STATE::DISPLAY,
            [&](SDL_Event& ev){
                if ( ev.key.keysym.sym == SDLK_SPACE || ev.key.keysym.sym == SDLK_RETURN ) {
                    SetState( (int)STATE::FADE_TO_PAGE2 );
                    m_bDisplayNote = false;
                    TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->Reset();
                    m_iHelpPage++;
                }
            }
        },
        {
            //
            // STATE::DISPLAY_PAGE2
            //
            (int)STATE::DISPLAY_PAGE2,
            [&](SDL_Event& ev){
                if ( ev.key.keysym.sym == SDLK_SPACE || ev.key.keysym.sym == SDLK_RETURN ) {
                    SetState( (int)STATE::FADE_OUT );
                    m_bDisplayNote = false;
                    TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->Reset();
                }
            }
        }
    };

    m_Handlers["Render"] = {
        {
            //
            // STATE::FADE_IN
            //
            (int)STATE::FADE_IN,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                auto& renderer = CSingleton<CRenderer>::Instance();
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_HELP1 )->SetAlpha( m_iScreenAlpha );
                renderer->Render( CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( RESOURCE::BACKGROUND_HELP1 ), 0, 0 );
                if ( m_bDisplayNote ) renderer->Render( TextFactory::Instance()->Get( RESOURCE::TEXT_SPACE_TO_CONTINUE ), m_iScreenW/2-m_noteImgWidth/2, m_iScreenH-35 );
            }
        },
        {
            //
            // STATE::DISPLAY
            //
            (int)STATE::DISPLAY,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                auto& renderer = CSingleton<CRenderer>::Instance();
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_HELP1 )->SetAlpha( 255 );
                renderer->Render( CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( RESOURCE::BACKGROUND_HELP1 ), 0, 0 );
                if ( m_bDisplayNote ) renderer->Render( TextFactory::Instance()->Get( RESOURCE::TEXT_SPACE_TO_CONTINUE ), m_iScreenW/2-m_noteImgWidth/2, m_iScreenH-35 );
            }
        },
        {
            //
            // STATE::FADE_TO_PAGE2
            //
            (int)STATE::FADE_TO_PAGE2,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                auto& renderer = CSingleton<CRenderer>::Instance();
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_HELP1 )->SetAlpha( 255-m_iScreenAlpha );
                renderer->Render( CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( RESOURCE::BACKGROUND_HELP1 ), 0, 0 );
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_HELP2 )->SetAlpha( m_iScreenAlpha );
                renderer->Render( CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( RESOURCE::BACKGROUND_HELP2 ), 0, 0 );
            }
        },
        {
            //
            // STATE::DISPLAY_PAGE2
            //
            (int)STATE::DISPLAY_PAGE2,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                auto& renderer = CSingleton<CRenderer>::Instance();
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_HELP2 )->SetAlpha( 255 );
                renderer->Render( CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( RESOURCE::BACKGROUND_HELP2 ), 0, 0 );
                if ( m_bDisplayNote ) renderer->Render( TextFactory::Instance()->Get( RESOURCE::TEXT_SPACE_TO_CONTINUE ), m_iScreenW/2-m_noteImgWidth/2, m_iScreenH-35 );
            }
        },
        {
            //
            // STATE::FADE_OUT
            //
            (int)STATE::FADE_OUT,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                auto& renderer = CSingleton<CRenderer>::Instance();
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_HELP2 )->SetAlpha( m_iScreenAlpha );
                renderer->Render( CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( RESOURCE::BACKGROUND_HELP2 ), 0, 0 );
            }
        }
    };
}
