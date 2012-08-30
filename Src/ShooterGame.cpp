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

#include "ShooterGame.hpp"
#include "SceneSplashScreen.hpp"
#include "SceneLevel.hpp"
#include "SceneGameOver.hpp"
#include "SceneHighscores.hpp"
#include "SceneHelp.hpp"
#include "ResourceIDs.hpp"
#include "EventIDs.hpp"

CShooterGame::~CShooterGame() {
    #ifdef DEBUG
    cout << "CShooterGame dtor called" << endl;
    // Report loaded asset counts
    cout << "ResourceFactory<CImageAlpha> count = " << ImageAlphaFactory::Instance()->Count() << endl;
    ImageAlphaFactory::Instance()->print();
    cout << "ResourceFactory<CAnimation> count = " << AnimationFactory::Instance()->Count() << endl;
    AnimationFactory::Instance()->print();
    cout << "ResourceFactory<CFont> count = " << FontFactory::Instance()->Count() << endl;
    FontFactory::Instance()->print();
    cout << "ResourceFactory<CMusic> count = " << MusicFactory::Instance()->Count() << endl;
    MusicFactory::Instance()->print();
    cout << "ResourceFactory<CSound> count = " << SoundFactory::Instance()->Count() << endl;
    SoundFactory::Instance()->print();
    #endif
}

void CShooterGame::Initialize()
{
    // Must call base class initializations first
    CGame::Initialize();

    #ifdef DEBUG
    cout << "CShooterGame::Initialize() called" << endl;
    #endif

    auto& renderer = CSingleton<CRenderer>::Instance();

    SDL_WM_SetCaption("Space Attackers!", "Space Attackers!");

    auto screen = renderer->GetScreen();
    m_iScreenW = screen->w;
    m_iScreenH = screen->h;

    // Create global mutex using Factory pattern
    {
        // Prepare Mutex singleton for access from multiple threads at the same time
        MutexFactory::Instance()->Initialize( RESOURCE::MUTEX_MAINAPP );
    }

    // Prepare rand() for multithreaded execution
    {
        CSingleton<CRandom>::Instance()->nextNumber();
    }

    // Load window icon using Factory pattern
    {
        ImageColorkeyFactory::Instance()->Get( RESOURCE::WINDOW_ICON )->Load( "Assets/Icon/icon.bmp" );
        ImageColorkeyFactory::Instance()->Get( RESOURCE::WINDOW_ICON )->SetColorKeyRGB( 0, 255, 255 );
        SDL_WM_SetIcon( ImageColorkeyFactory::Instance()->Get( RESOURCE::WINDOW_ICON )->GetSurface(), 0 );
    }

    // Load sprites using Factory pattern
    {
        ImageAlphaFactory::Instance()->Get( RESOURCE::PLAYER_PLANE )->Load( "Assets/Sprites/player_plane.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::PLAYER_PLANE_HIT )->Load( "Assets/Sprites/player_plane_hit.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::PLAYER_PLANE_SHADOW )->Load( "Assets/Sprites/player_plane_shadow_0.5x.png" );
        AnimationFactory::Instance()->Get( RESOURCE::PLAYER_PLANE )->LoadAnimation( "Assets/Sprites/player_plane.anim" );
        AnimationFactory::Instance()->Get( RESOURCE::PLAYER_PLANE_SHADOW )->LoadAnimation( "Assets/Sprites/player_plane_shadow_0.5x.anim" );

        ImageAlphaFactory::Instance()->Get( RESOURCE::PLAYER_PROJECTILE )->Load( "Assets/Sprites/plasma_up_yellow.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::PLAYER_PROJECTILE_GUIDED )->Load( "Assets/Sprites/plasma_ball_blue.png" );

        ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PROJECTILE_SLOW )->Load( "Assets/Sprites/plasma_ball_green.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PROJECTILE_FAST )->Load( "Assets/Sprites/plasma_ball_red.png" );

        ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_GREEN )->Load( "Assets/Sprites/plane_green_down.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_RED )->Load( "Assets/Sprites/plane_red_down.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_GREEN_HIT )->Load( "Assets/Sprites/plane_green_down_hit.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_RED_HIT )->Load( "Assets/Sprites/plane_red_down_hit.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::ENEMY_PLANE_SHADOW )->Load( "Assets/Sprites/plane_down_shadow_0.5x.png" );

        ImageAlphaFactory::Instance()->Get( RESOURCE::EXPLOSION )->Load( "Assets/Sprites/explosion.png" );
        AnimationFactory::Instance()->Get( RESOURCE::EXPLOSION )->LoadAnimation( "Assets/Sprites/explosion.anim" );
    }

    // Create interpolation sets for intro screen and level start message
    {
        auto& introOverTime = InterpolationSetFactory::Instance()->Get( RESOURCE::IS_INTRO_SHOW );
        introOverTime->AddValue( 0.0f, 0 );
        introOverTime->AddValue( 0.5f, 255, Math::Interpolation::easeOutCirc<float,float> );
        introOverTime->AddValue( 3.5f, 255 );
        introOverTime->AddValue( 5.0f, 0, Math::Interpolation::easeInCirc<float,float> );

        auto& levelStartOverTime = InterpolationSetFactory::Instance()->Get( RESOURCE::IS_LEVEL_START );
        levelStartOverTime->AddValue( 0.0f, m_iScreenH );
        levelStartOverTime->AddValue( 1.5f, m_iScreenH/2 - 32, Math::Interpolation::easeOutCirc<float,float> );
        levelStartOverTime->AddValue( 3.5f, m_iScreenH/2 - 32 );
        levelStartOverTime->AddValue( 5.0f, -64, Math::Interpolation::easeInCirc<float,float> );

    }

    // Load menu sprites using Factory pattern
    {
        ImageAlphaFactory::Instance()->Get( RESOURCE::MENU_TEXT )->Load( "Assets/Menu/texts.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::MENU_TEXT_SELECTED )->Load( "Assets/Menu/texts_selected.png" );
        AnimationFactory::Instance()->Get( RESOURCE::MENU_TEXT )->LoadAnimation( "Assets/Menu/texts.anim" );
    }

    // load backgrounds using Factory pattern
    {
        ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_INTRO )->Load( "Assets/Backgrounds/intro_screen.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_TITLE )->Load( "Assets/Backgrounds/title_screen.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_DEATH )->Load( "Assets/Backgrounds/death.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_HELP1 )->Load( "Assets/Backgrounds/help1.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_HELP2 )->Load( "Assets/Backgrounds/help2.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_MOONSURFACE )->Load( "Assets/Backgrounds/moon.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_CLOUDS1 )->Load( "Assets/Backgrounds/clouds1_tr50.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_CLOUDS2 )->Load( "Assets/Backgrounds/clouds2_tr50.png" );
        ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_MOONSURFACE )->SetAlpha(255);
        ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_CLOUDS1 )->SetAlpha(0);
        ImageAlphaFactory::Instance()->Get( RESOURCE::BACKGROUND_CLOUDS2 )->SetAlpha(0);
        // Create layers and set the bitmaps
        ScrollingBackgroundFactory::Instance()->Get( RESOURCE::SCROLLING_MOONSURFACE )->Initialize(1);
        ScrollingBackgroundFactory::Instance()->Get( RESOURCE::SCROLLING_CLOUDS )->Initialize(2);

        auto& moonSurfaceLayer = ScrollingBackgroundFactory::Instance()->Get( RESOURCE::SCROLLING_MOONSURFACE );
        moonSurfaceLayer->SetLayer( 0, RESOURCE::BACKGROUND_MOONSURFACE );
        moonSurfaceLayer->SetLayerSpeed( 0, 8, 8 );    // 1:15 movement speed
        moonSurfaceLayer->SetSpeed( 0, -250 );

        auto& cloudsLayers = ScrollingBackgroundFactory::Instance()->Get( RESOURCE::SCROLLING_CLOUDS );
        cloudsLayers->SetLayer( 0, RESOURCE::BACKGROUND_CLOUDS1 );
        cloudsLayers->SetLayer( 1, RESOURCE::BACKGROUND_CLOUDS2 );
        cloudsLayers->SetLayerSpeed( 0, 4, 4 );     // 1:7 movement speed
        cloudsLayers->SetLayerSpeed( 1, 1, 1 );     // 1:1 movement speed
        cloudsLayers->SetSpeed( 0, -250 );
    }

    // Load fonts using Factory pattern
    {
        FontFactory::Instance()->Get( RESOURCE::FONT_INFO )->Load( "Assets/Fonts/failed.ttf", 24 );
        FontFactory::Instance()->Get( RESOURCE::FONT_SCORE )->Load( "Assets/Fonts/digital-7_mono.ttf", 32 );
        FontFactory::Instance()->Get( RESOURCE::FONT_SMALL )->Load( "Assets/Fonts/ProFontWindows.ttf", 9 );
        FontFactory::Instance()->Get( RESOURCE::FONT_MENU )->Load( "Assets/Fonts/impact.ttf", 35 );
    }

    // Load sounds using Factory pattern
    {
        SoundFactory::Instance()->Get( RESOURCE::SOUND_INTRO_AMIGADISKFX )->Load( "Assets/Sounds/intro_disksound.ogg" );
        SoundFactory::Instance()->Get( RESOURCE::SOUND_PLAYER_FIRE )->Load( "Assets/Sounds/laser.ogg" );
        SoundFactory::Instance()->Get( RESOURCE::SOUND_ENEMY_FIRE )->Load( "Assets/Sounds/machinegun2.ogg" );
        SoundFactory::Instance()->Get( RESOURCE::SOUND_EXPLOSION1 )->Load( "Assets/Sounds/explosion_1.ogg" );
        SoundFactory::Instance()->Get( RESOURCE::SOUND_EXPLOSION2 )->Load( "Assets/Sounds/explosion_2.ogg" );
    }

    // Preload musics using Factory pattern
    {
        MusicFactory::Instance()->Get( RESOURCE::MUSIC_SPLASH )->Load( "Assets/Musics/cdk_-_Song_Of_Peace.ogg" );
        MusicFactory::Instance()->Get( RESOURCE::MUSIC_INGAME )->Load( "Assets/Musics/Astroboy_Sprung-160.ogg" );
        MusicFactory::Instance()->Get( RESOURCE::MUSIC_GAMEOVER )->Load( "Assets/Musics/Syenta_-_Cytheres_Reves_(Cytheres_Dreams)_2.ogg" );
    }

    // Create texts using Factory pattern
    {
        auto& fontSmall = FontFactory::Instance()->Get( RESOURCE::FONT_SMALL );
        auto& fontInfo = FontFactory::Instance()->Get( RESOURCE::FONT_INFO );
        auto& fontMenu = FontFactory::Instance()->Get( RESOURCE::FONT_MENU );
        TextFactory::Instance()->Get( RESOURCE::TEXT_BETA_BUILD_NOTE )->Create( fontSmall, "Space Attackers : Beta build, not for distribution! // Programming by Mika Luoma-aho <Fincodr@mxl.fi>", 80, 128, 75, 64 );
        TextFactory::Instance()->Get( RESOURCE::TEXT_STARTGAME )->Create( fontInfo, "< Press space to start a new game or ESC to quit >" );
        TextFactory::Instance()->Get( RESOURCE::TEXT_SPACE_TO_CONTINUE )->Create( fontInfo, "< Press space to continue >" );
        TextFactory::Instance()->Get( RESOURCE::TEXT_ENTER_TO_CONTINUE )->Create( fontInfo, "< Press enter to continue >" );
        TextFactory::Instance()->Get( RESOURCE::TEXT_HIGHSCORES )->Create( fontMenu, "< HI-SCORES >" );
        TextFactory::Instance()->Get( RESOURCE::TEXT_LEVEL_STARTING )->Create( fontMenu, "Get ready for" );
        TextFactory::Instance()->Get( RESOURCE::TEXT_GAMEOVER_SCORE )->Create( fontMenu, "SCORE" );
        TextFactory::Instance()->Get( RESOURCE::TEXT_GAMEOVER_ENTERYOURNAME )->Create( fontMenu, "ENTER YOUR NAME", 255, 220, 100 );
        TextFactory::Instance()->Get( RESOURCE::TEXT_GAMEOVER_NAME )->Create( fontMenu, "" );
        TextFactory::Instance()->Get( RESOURCE::TEXT_GAMEOVER_NAME_PROMPT )->Create( fontMenu, "_" /*«*/ );
    }

    // Create particle effects
    {
        // Create Explosion
        {
            auto& velocityOverTime = InterpolationSetFactory::Instance()->Get( RESOURCE::IS_EXPLOSION_VELOCITY );
            velocityOverTime->AddValue( 0.0f, 20.0f );
            velocityOverTime->AddValue( 0.7f, 10.5f, Math::Interpolation::easeOutCirc<float,float> );

            auto& alphaOverTime = InterpolationSetFactory::Instance()->Get( RESOURCE::IS_EXPLOSION_ALPHA );
            alphaOverTime->AddValue( 0.0f, 1.0f );
            alphaOverTime->AddValue( 1.0f, 0.0f, Math::Interpolation::linearTween<float,float> );

            auto& sizeOverTime = InterpolationSetFactory::Instance()->Get( RESOURCE::IS_EXPLOSION_SIZE );
            sizeOverTime->AddValue( 0.0f, 3.0f );
            sizeOverTime->AddValue( 1.0f, 1.0f, Math::Interpolation::linearTween<float,float> );

            // Flame colors
            // http://kuler.adobe.com/#themeID/177302
            //
            Math::Colors::HSV h1, h2, h3, h4, h5;
            h1 = { 41, 0.84f, 1.0f };
            h2 = { 43, 0.70f, 1.0f };
            h3 = { 63, 0.37f, 0.97f };
            h4 = { 30, 0.85f, 1.0f };
            h5 = { 17, 0.95f, 0.73f };

            auto& colorOverTime = SDLColorInterpolationSetFactory::Instance()->Get( RESOURCE::IS_EXPLOSION_COLOR );
            colorOverTime->AddValue( 0.0f, Math::Colors::HSV2SDLColor(h1) );
            colorOverTime->AddValue( 0.2f, Math::Colors::HSV2SDLColor(h2) );
            colorOverTime->AddValue( 0.4f, Math::Colors::HSV2SDLColor(h3) );
            colorOverTime->AddValue( 0.6f, Math::Colors::HSV2SDLColor(h4) );
            colorOverTime->AddValue( 0.8f, Math::Colors::HSV2SDLColor(h5) );

            // Set parameters
            auto& psExplosion = ExplosionSystemFactory::Instance()->Get( RESOURCE::PS_EXPLOSION );

            psExplosion->Initialize( 500 );
            psExplosion->SetVelocityOverTime( RESOURCE::IS_EXPLOSION_VELOCITY );
            psExplosion->SetAlphaOverTime( RESOURCE::IS_EXPLOSION_ALPHA );
            psExplosion->SetColorOverTime( RESOURCE::IS_EXPLOSION_COLOR );
            psExplosion->SetSizeOverTime( RESOURCE::IS_EXPLOSION_SIZE );
            psExplosion->SetPrimitiveType( 2 );
            psExplosion->SetTrails( false );
        }

        // Create Smoke
        {
            auto& velocityOverTime = InterpolationSetFactory::Instance()->Get( RESOURCE::IS_SMOKE_VELOCITY );
            velocityOverTime->AddValue( 0.0f, 15.0f );
            velocityOverTime->AddValue( 1.0f, 0.0f, Math::Interpolation::linearTween<float,float> );

            auto& alphaOverTime = InterpolationSetFactory::Instance()->Get( RESOURCE::IS_SMOKE_ALPHA );
            alphaOverTime->AddValue( 0.0f, 0.5f );
            alphaOverTime->AddValue( 1.0f, 0.1f, Math::Interpolation::easeInCirc<float,float> );

            auto& sizeOverTime = InterpolationSetFactory::Instance()->Get( RESOURCE::IS_SMOKE_SIZE );
            sizeOverTime->AddValue( 0.0f, 1.0f );
            sizeOverTime->AddValue( 1.0f, 3.0f, Math::Interpolation::linearTween<float,float> );

            // Smoke colors
            // http://kuler.adobe.com/#themeID/1262268
            //
            Math::Colors::HSV h1, h2, h3, h4;
            h1 = { 226, 0.25f, 0.16f };
            h2 = { 235, 0.07f, 0.34f };
            h3 = { 40, 0.08f, 0.41f };
            h4 = { 43, 0.15f, 0.53f };

            auto& colorOverTime = SDLColorInterpolationSetFactory::Instance()->Get( RESOURCE::IS_SMOKE_COLOR );
            colorOverTime->AddValue( 0.0f, Math::Colors::HSV2SDLColor(h1) );
            colorOverTime->AddValue( 0.3f, Math::Colors::HSV2SDLColor(h2) );
            colorOverTime->AddValue( 0.6f, Math::Colors::HSV2SDLColor(h3) );
            colorOverTime->AddValue( 1.0f, Math::Colors::HSV2SDLColor(h4) );

            // Set parameters
            auto& psSmoke = SmokeSystemFactory::Instance()->Get( RESOURCE::PS_SMOKE );

            psSmoke->Initialize( 300 );
            psSmoke->SetVelocityOverTime( RESOURCE::IS_SMOKE_VELOCITY );
            psSmoke->SetAlphaOverTime( RESOURCE::IS_SMOKE_ALPHA );
            psSmoke->SetColorOverTime( RESOURCE::IS_SMOKE_COLOR );
            psSmoke->SetSizeOverTime( RESOURCE::IS_SMOKE_SIZE );
            psSmoke->SetPrimitiveType( 2 );
            psSmoke->SetTrails( false );
        }
    }

    // Create explosion thread to be used in highscore screen
    {
        ExplosionThreadFactory::Instance()->Get( RESOURCE::THREAD_EXPLOSION )->SetExplosion( RESOURCE::PS_EXPLOSION );
    }

    // Create note
    {
        auto& noteText = TextFactory::Instance()->Get( RESOURCE::TEXT_BETA_BUILD_NOTE );
        m_noteImgWidth = noteText->GetSurface()->w;
    }

    // Load scenes into memory and set first scene as running
    {
        unique_ptr<CScene> SplashScene = unique_ptr<SceneSplashScreen>(new SceneSplashScreen);
        unique_ptr<CScene> Level1Scene = unique_ptr<SceneLevel>(new SceneLevel);
        unique_ptr<CScene> GameOverScene = unique_ptr<SceneGameOver>(new SceneGameOver);
        unique_ptr<CScene> HighscoresScene = unique_ptr<SceneHighscores>(new SceneHighscores);
        unique_ptr<CScene> HelpScene = unique_ptr<SceneHelp>(new SceneHelp);

        // Add scenes (order is also the render order!)
        AddScene( SCENE_0_Intro, SplashScene );
        AddScene( SCENE_1_Game, Level1Scene );
        AddScene( SCENE_2_GameOver, GameOverScene );
        AddScene( SCENE_3_Highscores, HighscoresScene );
        AddScene( SCENE_4_Help, HelpScene );
    }

    // Load and run the first scene
    //#ifdef DEBUG_SCENE1
    //LoadAndRunScene( SCENE_1_Game );
    //SetState( (int)STATE::GAME );
    auto& properties = CSingleton<CProperties>::Instance();
    #ifdef DEBUG_GAMEOVER
    properties->Property( "Player", "Name" ) = (std::string)"";
    properties->Property( "Player", "Score" ) = static_cast<int>(1234567);
    properties->Property( "Player", "Fired" ) = static_cast<int>(1000);
    properties->Property( "Player", "Kills" ) = static_cast<int>(20);
    properties->Property( "Player", "Accuracy" ) = static_cast<int>(98);
    properties->Property( "Player", "Level" ) = static_cast<int>(27);
    LoadAndRunScene( SCENE_2_GameOver );
    SetState( (int)STATE::GAME_OVER );
    #else
    properties->Property( "Player", "Name" ) = (std::string)"";
    LoadAndRunScene( SCENE_0_Intro );
    SetState( (int)STATE::SPLASH_SCREEN );
    #endif

    // Default to 60fps
    SDL_initFramerate( &m_fpsManager );
    SDL_setFramerate( &m_fpsManager, 60 );

    // Initialize state handlers
    InitHandlers();
}

/** \brief Initializers the event handlers
 *
 * \return void
 *
 */
void CShooterGame::InitHandlers() {

    ///////////////////////////////////////////////////
    //
    // On User Events
    //
    ///////////////////////////////////////////////////
    m_Handlers["OnUserEvent"] = {
        {
            //
            // STATE::SPLASH_SCREEN
            //
            (int)STATE::SPLASH_SCREEN,
            [&](SDL_Event& ev){
                int nEventType = ev.user.code;
                if ( nEventType == DemoEngine::EVENTTYPE::SCENE_EVENT ) {
                    int nState = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data1));
                    int nScene = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data2));
                    switch ( nScene ) {
                    case SCENE_0_Intro:
                        //
                        // SCENE::INTRO
                        //
                        if ( nState == (int)SceneSplashScreen::STATE::FADE_OUT )
                        {
                            if ( !CSingleton<CProperties>::Instance()->Property( "Game", "Quit" ) ) {
                                LoadAndRunScene( SCENE_1_Game );
                                SetState( (int)STATE::GAME );
                            }
                        }
                        if ( nState == (int)SceneSplashScreen::STATE::END )
                        {
                            if ( CSingleton<CProperties>::Instance()->Property( "Game", "Quit" ) ) {
                                SetRunning( false );
                            }
                        }
                        break;
                    default:
                        break;

                    }

                }
                if ( nEventType == DemoEngine::EVENTTYPE::CUSTOM_EVENT ) {
                    int nEventID = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data1));
                    int nScene = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data2));
                    switch ( nEventID ) {
                    case (int)EVENT::START_NEW_GAME:
                        SetSceneState( nScene, (int)SceneSplashScreen::STATE::PRE_FADE_OUT );
                        LoadAndRunScene( SCENE_1_Game );
                        SetState( (int)STATE::GAME );
                        break;
                    case (int)EVENT::SHOW_HIGHSCORES:
                        SetSceneState( nScene, (int)SceneSplashScreen::STATE::PRE_FADE_OUT_NOMUSIC );
                        LoadAndRunScene( SCENE_3_Highscores );
                        SetState( (int)STATE::HIGHSCORES );
                        break;
                    case (int)EVENT::SHOW_HELP:
                        SetSceneState( nScene, (int)SceneSplashScreen::STATE::PRE_FADE_OUT_NOMUSIC );
                        LoadAndRunScene( SCENE_4_Help );
                        SetState( (int)STATE::HELP );
                        break;
                    case (int)EVENT::END_GAME:
                        SetSceneState( nScene, (int)SceneSplashScreen::STATE::PRE_FADE_OUT );
                        CSingleton<CProperties>::Instance()->Property( "Game", "Quit" ) = (bool)true;
                        break;
                    default:
                        break;
                    }

                }
            }
        },
        {
            //
            // STATE::HIGHSCORES
            //
            (int)STATE::HIGHSCORES,
            [&](SDL_Event& ev){
                int nEventType = ev.user.code;
                if ( nEventType == DemoEngine::EVENTTYPE::SCENE_EVENT ) {
                    int nState = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data1));
                    int nScene = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data2));
                    switch ( nScene )
                    {
                    case SCENE_3_Highscores:
                        //
                        // SCENE::HELP
                        //
                        if ( nState == (int)SceneHighscores::STATE::FADE_OUT )
                        {
                            LoadAndRunScene( SCENE_0_Intro );
                            SetSceneState( SCENE_0_Intro, (int)SceneSplashScreen::STATE::START_NOMUSIC );
                            SetState( (int)STATE::SPLASH_SCREEN );
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        },
        {
            //
            // STATE::HELP
            //
            (int)STATE::HELP,
            [&](SDL_Event& ev){
                int nEventType = ev.user.code;
                if ( nEventType == DemoEngine::EVENTTYPE::SCENE_EVENT ) {
                    int nState = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data1));
                    int nScene = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data2));
                    switch ( nScene )
                    {
                    case SCENE_4_Help:
                        //
                        // SCENE::HELP
                        //
                        if ( nState == (int)SceneHelp::STATE::FADE_OUT )
                        {
                            LoadAndRunScene( SCENE_0_Intro );
                            SetSceneState( SCENE_0_Intro, (int)SceneSplashScreen::STATE::START_NOMUSIC );
                            SetState( (int)STATE::SPLASH_SCREEN );
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        },
        {
            //
            // STATE::GAME
            //
            (int)STATE::GAME,
            [&](SDL_Event& ev){
                int nEventType = ev.user.code;
                if ( nEventType == DemoEngine::EVENTTYPE::SCENE_EVENT ) {
                    int nState = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data1));
                    int nScene = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data2));
                    switch ( nScene )
                    {
                    case SCENE_1_Game:
                        //
                        // SCENE::GAME
                        //
                        if ( nState == (int)SceneLevel::STATE::FADE_OUT )
                        {
                            LoadAndRunScene( SCENE_2_GameOver );
                            SetState( (int)STATE::GAME_OVER );
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        },
        {
            //
            // STATE::GAME_OVER
            //
            (int)STATE::GAME_OVER,
            [&](SDL_Event& ev){
                int nEventType = ev.user.code;
                if ( nEventType == DemoEngine::EVENTTYPE::SCENE_EVENT ) {
                    int nState = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data1));
                    int nScene = static_cast<int>(reinterpret_cast<uintptr_t>(ev.user.data2));
                    switch ( nScene )
                    {
                    case SCENE_2_GameOver:
                        //
                        // SCENE::GAME_OVER
                        //
                        if ( nState == (int)SceneGameOver::STATE::FADE_OUT )
                        {
                            #ifdef DEBUG_SCENE1
                            cout << "Restarting Scene 1 because DEBUG_SCENE1 defined." << endl;
                            StopScene( SCENE_1_Game );
                            LoadAndRunScene( SCENE_1_Game );
                            SetState( (int)STATE::GAME );
                            #else
                            LoadAndRunScene( SCENE_0_Intro );
                            SetState( (int)STATE::SPLASH_SCREEN );
                            #endif
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    };

    ///////////////////////////////////////////////////
    //
    // Post Render
    //
    ///////////////////////////////////////////////////
    m_Handlers["PostRender"] = {
        {
            //
            // STATE::SPLASH_SCREEN
            //
            (int)CGame::STATE::ANY_STATE,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );

                auto& renderer = CSingleton<CRenderer>::Instance();
                int x = m_iScreenW + m_noteImgWidth - ((int)(GetRealTime()*60) % (m_iScreenW + m_noteImgWidth*2));
                renderer->Render( TextFactory::Instance()->Get( RESOURCE::TEXT_BETA_BUILD_NOTE ), x, m_iScreenH-12 );
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
            // STATE::TESTS
            //
            (int)CGame::STATE::ANY_STATE,
            [&](SDL_Event& ev){
                switch ( ev.key.keysym.sym ) {
                    #ifdef DEBUG
                    case SDLK_1:
                        cout << "Setting Game speed to 0.1" << endl;
                        CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = (float)0.1f;
                        break;
                    case SDLK_2:
                        cout << "Setting Game speed to 0.3" << endl;
                        CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = (float)0.3f;
                        break;
                    case SDLK_3:
                        cout << "Setting Game speed to 0.5" << endl;
                        CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = (float)0.5f;
                        break;
                    case SDLK_4:
                        cout << "Setting Game speed to 0.7" << endl;
                        CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = (float)0.7f;
                        break;
                    case SDLK_5:
                        cout << "Setting Game speed to 1.0" << endl;
                        CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = (float)1.0f;
                        break;
                    case SDLK_6:
                        cout << "Setting Game speed to 1.3" << endl;
                        CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = (float)1.3f;
                        break;
                    case SDLK_7:
                        cout << "Setting Game speed to 1.5" << endl;
                        CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = (float)1.5f;
                        break;
                    case SDLK_8:
                        cout << "Setting Game speed to 1.7" << endl;
                        CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = (float)1.7f;
                        break;
                    #endif
                    case SDLK_F1:
                        cout << "Setting FPS to 10" << endl;
                        SDL_setFramerate( &m_fpsManager, 10 );
                        break;
                    case SDLK_F2:
                        cout << "Setting FPS to 20" << endl;
                        SDL_setFramerate( &m_fpsManager, 20 );
                        break;
                    case SDLK_F3:
                        cout << "Setting FPS to 30" << endl;
                        SDL_setFramerate( &m_fpsManager, 30 );
                        break;
                    case SDLK_F4:
                        cout << "Setting FPS to 40" << endl;
                        SDL_setFramerate( &m_fpsManager, 40 );
                        break;
                    case SDLK_F5:
                        cout << "Setting FPS to 50" << endl;
                        SDL_setFramerate( &m_fpsManager, 50 );
                        break;
                    case SDLK_F6:
                        cout << "Setting FPS to 60" << endl;
                        SDL_setFramerate( &m_fpsManager, 60 );
                        break;
                    case SDLK_F7:
                        cout << "Setting FPS to 120" << endl;
                        SDL_setFramerate( &m_fpsManager, 120 );
                        break;
                    default:
                        break;
                }
            }
        }
    };

}
