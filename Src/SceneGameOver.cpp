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

#include "SceneGameOver.hpp"

void SceneGameOver::Initialize() {
    cout << "SceneGameOver::Initialize() called" << endl;
    InitHandlers();
    SetInitialized( true );
}

void SceneGameOver::Load()
{
    auto& renderer = CSingleton<CRenderer>::Instance();
    auto screen = renderer->GetScreen();
    m_iScreenW = screen->w;
    m_iScreenH = screen->h;

    auto& noteText = TextFactory::Instance()->Get( RESOURCE::TEXT_ENTER_TO_CONTINUE );
    m_noteImgWidth = noteText->GetSurface()->w;

    SetLoaded( true );
}

void SceneGameOver::OnEnter()
{
    m_iScreenAlpha = 0;
    m_bDisplayNote = false;

    auto& fontScore = FontFactory::Instance()->Get( RESOURCE::FONT_MENU );
    auto& fontInfo = FontFactory::Instance()->Get( RESOURCE::FONT_INFO );
    auto& textScore = TextFactory::Instance()->Get( RESOURCE::TEXT_STATS_SCORE );
    auto& textLevel = TextFactory::Instance()->Get( RESOURCE::TEXT_STATS_LEVEL );
    auto& textFired = TextFactory::Instance()->Get( RESOURCE::TEXT_STATS_FIRED );
    auto& textKills = TextFactory::Instance()->Get( RESOURCE::TEXT_STATS_KILLS );
    auto& textAccuracy = TextFactory::Instance()->Get( RESOURCE::TEXT_STATS_ACCURACY );
    auto& textName = TextFactory::Instance()->Get( RESOURCE::TEXT_GAMEOVER_NAME );

    auto& properties = CSingleton<CProperties>::Instance();
    cout << "Player score  : " << (int)properties->Property( "Player", "Score" ) << endl;
    cout << "Level         : " << (int)properties->Property( "Player", "Level" ) << endl;
    cout << "Bullets fired : " << (int)properties->Property( "Player", "Fired" ) << endl;
    cout << "Enemy kills   : " << (int)properties->Property( "Player", "Kills" ) << endl;
    cout << "Accuracy      : " << (int)properties->Property( "Player", "Accuracy" ) << endl;
    cout << "Name          : " << (std::string)properties->Property( "Player", "Name" ) << endl;

    // Write score
    {
        std::ostringstream ss;
        ss << std::setw( 7 ) << std::setfill( '0' ) << (int)properties->Property( "Player", "Score" );
        textScore->Create( fontScore, ss.str().c_str(), 255, 255, 64 );
    }

    // Write level
    {
        std::ostringstream ss;
        ss << "Level: " << (int)properties->Property( "Player", "Level" );
        textLevel->Create( fontInfo, ss.str().c_str(), 255, 255, 255 );
    }

    // Write bullets fired
    {
        std::ostringstream ss;
        ss << "Bullets fired: " << (int)properties->Property( "Player", "Fired" );
        textFired->Create( fontInfo, ss.str().c_str(), 255, 255, 255 );
    }

    // Write enemy kills
    {
        std::ostringstream ss;
        ss << "Enemies killed: " << (int)properties->Property( "Player", "Kills" );
        textKills->Create( fontInfo, ss.str().c_str(), 255, 255, 255 );
    }

    // Write accuracy
    {
        std::ostringstream ss;
        ss << "Accuracy: " << (int)properties->Property( "Player", "Accuracy" ) << "%";
        textAccuracy->Create( fontInfo, ss.str().c_str(), 255, 255, 255 );
    }

    // Write initial name
    {
        m_strPlayerName = (std::string)properties->Property( "Player", "Name" );
        std::ostringstream ss;
        ss << m_strPlayerName;
        textName->Create( fontScore, ss.str().c_str(), 255, 255, 0 );
    }


    SetState( (int)STATE::START );
}

void SceneGameOver::FadeMusicIn()
{
    auto& sound = CSingleton<CSoundServer>::Instance();
    sound->HaltMusic();
    sound->PlayMusic( RESOURCE::MUSIC_GAMEOVER );
}

void SceneGameOver::FadeMusicOut()
{
    auto& sound = CSingleton<CSoundServer>::Instance();
    sound->HaltMusic(); //FadeOutMusic( 1000 );
}

void SceneGameOver::RenderInfo( unique_ptr<CRenderer>& renderer )
{
    auto& textScore = TextFactory::Instance()->Get( RESOURCE::TEXT_STATS_SCORE );
    auto& textLevel = TextFactory::Instance()->Get( RESOURCE::TEXT_STATS_LEVEL );
    auto& textFired = TextFactory::Instance()->Get( RESOURCE::TEXT_STATS_FIRED );
    auto& textKills = TextFactory::Instance()->Get( RESOURCE::TEXT_STATS_KILLS );
    auto& textAccuracy = TextFactory::Instance()->Get( RESOURCE::TEXT_STATS_ACCURACY );
    auto& textScoreCaption = TextFactory::Instance()->Get( RESOURCE::TEXT_GAMEOVER_SCORE );
    auto& textEnterName = TextFactory::Instance()->Get( RESOURCE::TEXT_GAMEOVER_ENTERYOURNAME );
    auto& textName = TextFactory::Instance()->Get( RESOURCE::TEXT_GAMEOVER_NAME );
    auto& textPrompt = TextFactory::Instance()->Get( RESOURCE::TEXT_GAMEOVER_NAME_PROMPT );

    int y = m_nMenuYAnimator;
    m_Rect.SetDimensions( m_iScreenW, 40*4 + 45 );
    m_Rect.SetPosition( 0, 200+40*0 + y );
    m_Rect.SetColor( 0, 0, 0, 127 );
    renderer->Render( &m_Rect );

    renderer->Render( textScoreCaption, 200, 200+40*0 + y );
    renderer->Render( textScore, 200 + 128, 200+40*0 + y );

    renderer->Render( textEnterName, 200, 200+40*1 + y );

    renderer->Render( textName, 200, 200+40*2 + y );

    // Render prompt only for even seconds / 2
    TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->Update();
    if ( static_cast<int>((float)TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->GetPassedTimeReal()*6)%2 == 0 )
    {
        int nameWidth = textName->GetSurface()->w;
        renderer->Render( textPrompt, 200 + nameWidth, 200+40*2 + y );
    }

    renderer->Render( textLevel, 200, 300+20*1 + y );
    renderer->Render( textFired, 200, 300+20*2 + y  );
    renderer->Render( textKills, 200, 300+20*3 + y );
    renderer->Render( textAccuracy, 200, 300+20*4 + y );
}

void SceneGameOver::InitHandlers()
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
                if ( CSingleton<CProperties>::Instance()->Property( "Game", "Music" ) )
                    FadeMusicIn();
                TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->Reset();
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_DEATH )->SetAlpha( 0 );
                m_nMenuYAnimator = m_iScreenH;
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
                ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_DEATH )->SetAlpha( m_iScreenAlpha );
                m_nMenuYAnimator = Math::Interpolation::easeOutCirc<float,float>( TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEIN )->GetPassedTimeReal(), m_iScreenH*3, -m_iScreenH*3, 3.0f );
                if ( m_iScreenAlpha >= 255 && m_nMenuYAnimator == 0 )
                {
                    SetState( (int)STATE::DISPLAY );
                    m_bDisplayNote = true;
                    m_nMenuYAnimator = 0;
                    #ifdef DEBUG_SCENE1
                    SetState( (int)STATE::FADE_OUT );
                    if ( CSingleton<CProperties>::Instance()->Property( "Game", "Music" ) )
                        FadeMusicOut();
                    m_bDisplayNote = false;
                    #endif
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
                //ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_DEATH )->SetAlpha( m_iScreenAlpha );
                //m_nMenuYAnimator = Math::Interpolation::easeInCirc<float,float>( TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->GetPassedTimeReal(), 0, m_iScreenH, 1.0f );
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
                if ( ev.key.keysym.sym == SDLK_RETURN ) {
                    // Add highscore entry
                    auto& properties = CSingleton<CProperties>::Instance();
                    CSingleton<CHighscoreList>::Instance()->AddEntry(
                        (string)properties->Property( "Player", "Name" ),
                        (int)properties->Property( "Player", "Score" ),
                        (int)properties->Property( "Player", "Level" ),
                        (int)properties->Property( "Player", "Accuracy" )
                    );
                    // Fade out
                    SetState( (int)STATE::FADE_OUT );
                    if ( CSingleton<CProperties>::Instance()->Property( "Game", "Music" ) )
                        FadeMusicOut();
                    m_bDisplayNote = false;
                    TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->Reset();
                }

                if ( m_strPlayerName.length() < 10 )
                {
                    // A-Z
                    if( ev.key.keysym.sym >= SDLK_a && ev.key.keysym.sym <= SDLK_z )
                    {
                        m_strPlayerName += (char)(ev.key.keysym.sym-SDLK_a+'A');
                    }
                    // 0-9
                    if( ev.key.keysym.sym >= SDLK_0 && ev.key.keysym.sym <= SDLK_9 )
                    {
                        m_strPlayerName +=(char)(ev.key.keysym.sym-SDLK_0+'0');
                    }
                    // SPACE
                    if( ev.key.keysym.sym == SDLK_SPACE )
                    {
                        m_strPlayerName += ' ';
                    }
                    // -
                    if( ev.key.keysym.sym == SDLK_MINUS )
                    {
                        m_strPlayerName += '-';
                    }
                }
                // BACKSPACE
                if ( ev.key.keysym.sym == SDLK_BACKSPACE )
                {
                    // remove one char from string
                    m_strPlayerName = m_strPlayerName.substr( 0, m_strPlayerName.length()-1 );
                }

                // Set name
                auto& fontScore = FontFactory::Instance()->Get( RESOURCE::FONT_MENU );
                auto& properties = CSingleton<CProperties>::Instance();
                auto& textName = TextFactory::Instance()->Get( RESOURCE::TEXT_GAMEOVER_NAME );
                properties->Property( "Player", "Name" ) = m_strPlayerName;
                std::ostringstream ss;
                ss << m_strPlayerName;
                textName->Create( fontScore, ss.str().c_str(), 255, 255, 0 );
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
                renderer->Render( ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_DEATH ), 0, 0 );
                RenderInfo( renderer );
                if ( m_bDisplayNote ) renderer->Render( TextFactory::Instance()->Get( RESOURCE::TEXT_ENTER_TO_CONTINUE ), m_iScreenW/2-m_noteImgWidth/2, m_iScreenH-35 );
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
                renderer->Render( ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_DEATH ), 0, 0 );
                RenderInfo( renderer );
                if ( m_bDisplayNote ) renderer->Render( TextFactory::Instance()->Get( RESOURCE::TEXT_ENTER_TO_CONTINUE ), m_iScreenW/2-m_noteImgWidth/2, m_iScreenH-35 );
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
                RenderInfo( renderer );
                renderer->Render( ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_DEATH ), 0, 0 );
            }
        }
    };
}
