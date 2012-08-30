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

#ifndef RESOURCEIDS_HPP
#define RESOURCEIDS_HPP

#include "DemoEngine/Singleton.hpp"
#include "DemoEngine/ResourceFactory.hpp"

using namespace DemoEngine;

typedef enum {

    BACKGROUND_INTRO = 0,
    BACKGROUND_TITLE,
    BACKGROUND_MOONSURFACE,
    BACKGROUND_CLOUDS1,
    BACKGROUND_CLOUDS2,
    BACKGROUND_HELP1,
    BACKGROUND_HELP2,
    BACKGROUND_DEATH,

    WINDOW_ICON,

    TIMER_SCENE_FADEIN,
    TIMER_SCENE_FADEOUT,
    TIMER_PLAYER_COOLDOWN,
    TIMER_LEVEL_START,

    IS_INTRO_SHOW,
    IS_LEVEL_START,

    SCROLLING_MOONSURFACE,
    SCROLLING_CLOUDS,

    PLAYER_PLANE,
    PLAYER_PLANE_HIT,
    PLAYER_PLANE_SHADOW,

    ENEMY_PLANE_GREEN,
    ENEMY_PLANE_RED,
    ENEMY_PLANE_GREEN_HIT,
    ENEMY_PLANE_RED_HIT,
    ENEMY_PLANE_SHADOW,

    PLAYER_PROJECTILE,
    PLAYER_PROJECTILE_GUIDED,
    ENEMY_PROJECTILE_SLOW,
    ENEMY_PROJECTILE_FAST,

    EXPLOSION,

    MENU_TEXT,
    MENU_TEXT_SELECTED,

    PS_EXPLOSION,
    IS_EXPLOSION_VELOCITY,
    IS_EXPLOSION_SIZE,
    IS_EXPLOSION_COLOR,
    IS_EXPLOSION_ALPHA,

    PS_SMOKE,
    IS_SMOKE_VELOCITY,
    IS_SMOKE_SIZE,
    IS_SMOKE_COLOR,
    IS_SMOKE_ALPHA,

    FONT_INFO,
    FONT_SCORE,
    FONT_SMALL,
    FONT_MENU,

    SOUND_INTRO_AMIGADISKFX,
    SOUND_PLAYER_FIRE,
    SOUND_ENEMY_FIRE,
    SOUND_EXPLOSION1,
    SOUND_EXPLOSION2,

    MUSIC_SPLASH,
    MUSIC_INGAME,
    MUSIC_GAMEOVER,
    MUSIC_HIGHSCORES,

    TEXT_STARTGAME,
    TEXT_SPACE_TO_CONTINUE,
    TEXT_ENTER_TO_CONTINUE,
    TEXT_BETA_BUILD_NOTE,
    TEXT_SCORE,
    TEXT_HIGHSCORES,
    TEXT_LEVEL_STARTING,
    TEXT_LEVEL,
    TEXT_LEVEL_INFO,
    TEXT_NEW_HIGHSCORE,
    TEXT_GAMEOVER_SCORE,
    TEXT_GAMEOVER_ENTERYOURNAME,
    TEXT_GAMEOVER_NAME,
    TEXT_GAMEOVER_NAME_PROMPT,
    TEXT_STATS_SCORE,
    TEXT_STATS_FIRED,
    TEXT_STATS_ACCURACY,
    TEXT_STATS_KILLS,
    TEXT_STATS_LEVEL,

    THREAD_EXPLOSION,
    MUTEX_MAINAPP

} RESOURCE;

#include "DemoEngine/Mutex.hpp"
#include "DemoEngine/Timer.hpp"
#include "DemoEngine/ImageColorkey.hpp"
#include "DemoEngine/ImageAlpha.hpp"
#include "DemoEngine/Animation.hpp"
#include "DemoEngine/Font.hpp"
#include "DemoEngine/Music.hpp"
#include "DemoEngine/Sound.hpp"
#include "DemoEngine/Text.hpp"
#include "DemoEngine/InterpolationSet.hpp"
#include "DemoEngine/ScrollingBackground.hpp"
#include "ExplosionSystem.hpp"
#include "SmokeSystem.hpp"
#include "ExplosionThread.hpp"

#endif // RESOURCEIDS_HPP
