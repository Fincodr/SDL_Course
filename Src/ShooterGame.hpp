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

#ifndef SHOOTERGAME_HPP
#define SHOOTERGAME_HPP

#include <iostream>
#include <memory>
#include <SDL_framerate.h>
#include "DemoEngine/Singleton.hpp"
#include "DemoEngine/Game.hpp"
#include "DemoEngine/Font.hpp"
#include "DemoEngine/Scene.hpp"

using std::cout;
using std::endl;
using std::array;
using std::unique_ptr;

using namespace DemoEngine;

class CShooterGame : public CGame
{
    public:
        CShooterGame() {
            #ifdef DEBUGCTORS
            cout << "CShooterGame ctor called" << endl;
            #endif
        };
        ~CShooterGame();

        static const SCENEID_t SCENE_0_Intro        = 100;
        static const SCENEID_t SCENE_1_Game         = 10;
        static const SCENEID_t SCENE_2_GameOver     = 20;
        static const SCENEID_t SCENE_3_Highscores   = 30;
        static const SCENEID_t SCENE_4_Help         = 40;

        typedef enum class {
            SPLASH_SCREEN = 0,
            GAME,
            GAME_OVER,
            HIGHSCORES,
            HELP,
            END,
            TESTS
        } STATE;

        void InitHandlers() override;
        void Initialize() override;

    protected:
    private:
        int m_noteImgWidth = 0;
        int m_iScreenW = 0;
        int m_iScreenH = 0;
};

#endif // SHOOTERGAME_HPP
