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

#include "SceneHighscores.hpp"

void SceneHighscores::Initialize() {
    cout << "SceneHighscores::Initialize() called" << endl;
    InitHandlers();
    SetInitialized( true );
}

void SceneHighscores::Load()
{
    auto& renderer = CSingleton<CRenderer>::Instance();
    auto screen = renderer->GetScreen();
    m_iScreenW = screen->w;
    m_iScreenH = screen->h;

    auto& noteText = TextFactory::Instance()->Get( RESOURCE::TEXT_HIGHSCORES );
    m_noteImgWidth = noteText->GetSurface()->w;

    SetLoaded( true );
}

void SceneHighscores::OnEnter()
{
    m_iScreenAlpha = 0;
    m_bDisplayNote = false;

    // Get references to particle effect
    auto& psExplosion = ExplosionSystemFactory::Instance()->Get( RESOURCE::PS_EXPLOSION );

    // Add entities to update and renderable list
    auto& renderables = GetRenderables();
    auto& updateables = GetUpdateables();
    updateables = { { psExplosion->GetID(), psExplosion } };
    renderables = { { psExplosion->GetID(), psExplosion } };

    SetState( (int)STATE::START );
}

void SceneHighscores::OnExit()
{
    // Remove everything from update and renderable list
    auto& preRenderables = GetPreRenderables();
    auto& renderables = GetRenderables();
    auto& postRenderables = GetPostRenderables();
    auto& updateables = GetUpdateables();
    updateables.clear();
    preRenderables.clear();
    renderables.clear();
    postRenderables.clear();
    ExplosionThreadFactory::Instance()->Get( RESOURCE::THREAD_EXPLOSION )->WaitThread();
}

void SceneHighscores::RenderHighscores( unique_ptr<CRenderer>& renderer )
{
    int y = m_nMenuYAnimator;

    auto& texts = ImageAlphaFactory::Instance()->Get( RESOURCE::MENU_TEXT );
    //auto& texts_selected = ImageAlphaFactory::Instance()->Get( RESOURCE::MENU_TEXT_SELECTED );
    auto& texts_frames = AnimationFactory::Instance()->Get( RESOURCE::MENU_TEXT );
    auto& rect = texts_frames->GetAnimationRect( "Menu texts", 1 );
    renderer->Render( texts, m_iScreenW/2 - rect.w/2, 30 + y, &rect );

    auto& highscoreList = CSingleton<CHighscoreList>::Instance()->GetList();
    int i = 0;
    for ( auto& item : highscoreList )
    {
        auto& fontScore = FontFactory::Instance()->Get( RESOURCE::FONT_SCORE );
        auto& textScore = TextFactory::Instance()->Get( RESOURCE::TEXT_SCORE );
        std::ostringstream ss;
        ss << std::setw( 7 ) << std::setfill( '0' ) << item.score;
        ss << " ";
        ss << std::left << std::setw( 10 ) << std::setfill( ' ' ) << item.name;
        ss << std::right << std::setw( 0 ) << " Level: " << std::setw(3) << item.level << std::setw(0) << ", Acc: " << std::setw(3) << item.accuracy << std::setw(0) << "%";
        textScore->Create( fontScore, ss.str().c_str(), 255, 255, i*25 );
        //cout << item.score << " " << item.name << " " << item.level << " " << item.accuracy << "%" << endl;
        renderer->Render( textScore, 16, 86 + i*35 + y );
        i++;
        if ( i > 9 )
            break;
    }
}

void SceneHighscores::InitHandlers()
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
                m_nMenuYAnimator = Math::Interpolation::easeOutCirc<float,float>( TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->GetPassedTimeReal(), m_iScreenH, -m_iScreenH, 1.0f );
                if ( m_iScreenAlpha >= 255 )
                {
                    SetState( (int)STATE::DISPLAY );
                    m_bDisplayNote = true;
                    ExplosionThreadFactory::Instance()->Get( RESOURCE::THREAD_EXPLOSION )->StartThread();
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
                // Stop the threads
                ExplosionThreadFactory::Instance()->Get( RESOURCE::THREAD_EXPLOSION )->StopThread();
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->Reset();
                SetState( (int)STATE::FADE_OUT );
                m_bDisplayNote = false;
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
                m_nMenuYAnimator = Math::Interpolation::easeInCirc<float,float>( TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->GetPassedTimeReal(), 0, m_iScreenH, 1.0f );
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
                DISCARD_UNUNSED_PARAMETER( ev );
                if ( ev.key.keysym.sym == SDLK_SPACE ) {
                    SetState( (int)STATE::PRE_FADE_OUT );
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
                RenderHighscores( renderer );
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
                RenderHighscores( renderer );
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
                RenderHighscores( renderer );
            }
        }
    };
}
