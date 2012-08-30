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

#include "SceneSplashScreen.hpp"
#include "ResourceIDs.hpp"
#include "EventIDs.hpp"

void SceneSplashScreen::Initialize() {
    #ifdef DEBUG
    cout << "SceneSplashScreen::Initialize() called" << endl;
    #endif
    InitHandlers();
    SetInitialized( true );
}

void SceneSplashScreen::Load()
{
    auto& renderer = CSingleton<CRenderer>::Instance();
    auto screen = renderer->GetScreen();
    m_iScreenW = screen->w;
    m_iScreenH = screen->h;

    SetLoaded( true );
}

void SceneSplashScreen::OnEnter()
{
    m_iScreenAlpha = 0;
    m_nMenuSelection = 0;
    m_nMenuYAnimator = -m_iScreenH;
    m_nMaxMenuSelection = AnimationFactory::Instance()->Get( RESOURCE::MENU_TEXT )->GetAnimationFrames( kAnimationName ) - 1;
    if ( m_bIntroShown )
        SetState( (int)STATE::START );
    else
    {
        m_bIntroShown = true;
        SetState( (int)STATE::INTRO );
    }
}

void SceneSplashScreen::FadeMusicIn()
{
    auto& sound = CSingleton<CSoundServer>::Instance();
    sound->FadeInMusic( RESOURCE::MUSIC_SPLASH, 800 );
}

void SceneSplashScreen::FadeMusicOut()
{
    auto& sound = CSingleton<CSoundServer>::Instance();
    sound->FadeOutMusic( 800 );
}

void SceneSplashScreen::RenderMenu( unique_ptr<CRenderer>& renderer )
{
    int w = 256;
    int h = 32;
    int x = m_iScreenW/2 - w/2;
    int y = m_iScreenH/2 - (((h-5)*3)/2) + m_nMenuYAnimator;
    auto& texts = ImageAlphaFactory::Instance()->Get( RESOURCE::MENU_TEXT );
    auto& texts_selected = ImageAlphaFactory::Instance()->Get( RESOURCE::MENU_TEXT_SELECTED );
    auto& texts_frames = AnimationFactory::Instance()->Get( RESOURCE::MENU_TEXT );
    for ( int i=0; i!= m_nMaxMenuSelection+1; ++i )
    {
        auto& rect = texts_frames->GetAnimationRect( "Menu texts", i );
        if ( i == m_nMenuSelection )
            renderer->Render( texts_selected, x, y, &rect );
        else
            renderer->Render( texts, x, y, &rect );
        y += h;
    }
}

void SceneSplashScreen::InitHandlers()
{
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
                switch ( ev.key.keysym.sym )
                {
                case SDLK_m:
                    // Toggle music
                    {
                        bool bMusic = CSingleton<CProperties>::Instance()->Property( "Game", "Music" );
                        bMusic == false ? bMusic = true : bMusic = false;
                        CSingleton<CProperties>::Instance()->Property( "Game", "Music" ) = bMusic;
                        cout << "Info: Music set to " << std::boolalpha << bMusic << endl;
                        if ( bMusic )
                            FadeMusicIn();
                        else
                            FadeMusicOut();
                    }
                    break;
                case SDLK_ESCAPE:
                    SendCustomEvent( (int)EVENT::END_GAME );
                    break;
                case SDLK_RETURN:
                case SDLK_SPACE:
                    {
                        switch ( m_nMenuSelection ) {
                        case 0: // NEW GAME
                            SendCustomEvent( (int)EVENT::START_NEW_GAME );
                            break;
                        case 1: // HIGHSCORES
                            SendCustomEvent( (int)EVENT::SHOW_HIGHSCORES );
                            break;
                        case 2: // HELP
                            SendCustomEvent( (int)EVENT::SHOW_HELP );
                            break;
                        case 3: // QUIT
                            SendCustomEvent( (int)EVENT::END_GAME );
                            break;
                        default:
                            break;
                        }
                    }
                    break;
                case SDLK_UP:
                    {
                        m_nMenuSelection--;
                        if ( m_nMenuSelection < 0 ) m_nMenuSelection = m_nMaxMenuSelection;
                    }
                    break;
                case SDLK_DOWN:
                    {
                        m_nMenuSelection++;
                        if ( m_nMenuSelection > m_nMaxMenuSelection ) m_nMenuSelection = 0;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    };

    ///////////////////////////////////////////////////
    //
    // Update
    //
    ///////////////////////////////////////////////////
    m_Handlers["Update"] = {
        {
            //
            // STATE::INTRO
            //
            (int)STATE::INTRO,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );

                // Prepare for Fade-IN-OUT
                SetState( (int)STATE::INTRO_FADE_IN_OUT );
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->Reset();
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_INTRO )->SetAlpha( 0 );
                SetState( (int)STATE::INTRO_FADE_IN_OUT );
                auto& sound = CSingleton<CSoundServer>::Instance();
                sound->Play( RESOURCE::SOUND_INTRO_AMIGADISKFX );
            }
        },
        {
            //
            // STATE::INTRO_FADE_IN_OUT
            //
            (int)STATE::INTRO_FADE_IN_OUT,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );

                // Calculate Fade-IN-OUT
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->Update();
                float fTime = TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->GetPassedTimeReal();
                m_iScreenAlpha = Interpolate<float,float>( fTime, InterpolationSetFactory::Instance()->Get( RESOURCE::IS_INTRO_SHOW ) );
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_INTRO )->SetAlpha( m_iScreenAlpha );
                if ( fTime >= 5.0f )
                {
                    SetState( (int)STATE::START );
                }
            }
        },
        {
            //
            // STATE::START
            //
            (int)STATE::START,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                if ( CSingleton<CProperties>::Instance()->Property( "Game", "Music" ) )
                    FadeMusicIn();
                SetState( (int)STATE::FADE_IN );
                m_nMenuYAnimator = m_iScreenH;
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->Reset();
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_TITLE )->SetAlpha( 0 );
            }
        },
        {
            //
            // STATE::START_WITHOUT_MUSIC
            //
            (int)STATE::START_NOMUSIC,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                SetState( (int)STATE::FADE_IN );
                m_nMenuYAnimator = m_iScreenH;
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->Reset();
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_TITLE )->SetAlpha( 0 );
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
                m_nMenuYAnimator = Math::Interpolation::easeOutCirc<float,float>( TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->GetPassedTimeReal(), m_iScreenH, -m_iScreenH, 1.0f );
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_TITLE )->SetAlpha( m_iScreenAlpha );
                if ( m_iScreenAlpha >= 255 )
                {
                    SetState( (int)STATE::DISPLAY );
                    m_nMenuYAnimator = 0;
                }
            }
        },
        {
            //
            // STATE::PRE_FADE_OUT
            //
            (int)STATE::PRE_FADE_OUT,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->Reset();
                if ( CSingleton<CProperties>::Instance()->Property( "Game", "Music" ) )
                    FadeMusicOut();
                SetState( (int)STATE::FADE_OUT );
            }
        },
        {
            //
            // STATE::PRE_FADE_OUT_NOMUSIC
            //
            (int)STATE::PRE_FADE_OUT_NOMUSIC,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->Reset();
                SetState( (int)STATE::FADE_OUT );
            }
        },        {
            //
            // STATE::FADE_OUT
            //
            (int)STATE::FADE_OUT,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                // Calculate Fade-OUT
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->Update();
                m_iScreenAlpha = Math::Interpolation::linearTween<float,float>( TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->GetPassedTimeReal(), 255, -255, 1.0f );
                m_nMenuYAnimator = Math::Interpolation::easeInCirc<float,float>( TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->GetPassedTimeReal(), 0, m_iScreenH, 1.0f );
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_TITLE )->SetAlpha( m_iScreenAlpha );
                if ( m_iScreenAlpha <= 0 )
                {
                    SetState( (int)STATE::END );
                }
            }

        }
    };

    m_Handlers["Render"] = {
        {
            //
            // STATE::INTRO
            //
            (int)STATE::INTRO,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                auto& renderer = CSingleton<CRenderer>::Instance();
                renderer->Render( CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( RESOURCE::BACKGROUND_INTRO ), 0, 0 );
                RenderMenu( renderer );
            }
        },
        {
            //
            // STATE::INTRO_FADE_IN_OUT
            //
            (int)STATE::INTRO_FADE_IN_OUT,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                auto& renderer = CSingleton<CRenderer>::Instance();
                renderer->Render( CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( RESOURCE::BACKGROUND_INTRO ), 0, 0 );
                RenderMenu( renderer );
            }
        },
        {
            //
            // STATE::FADE_IN
            //
            (int)STATE::FADE_IN,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                auto& renderer = CSingleton<CRenderer>::Instance();
                renderer->Render( CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( RESOURCE::BACKGROUND_TITLE ), 0, 0 );
                RenderMenu( renderer );
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
                renderer->Render( CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( RESOURCE::BACKGROUND_TITLE ), 0, 0 );
                RenderMenu( renderer );
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
                renderer->Render( CSingleton<CResourceFactory<int, CImageAlpha>>::Instance()->Get( RESOURCE::BACKGROUND_TITLE ), 0, 0 );
                RenderMenu( renderer );
            }
        }
    };
}
