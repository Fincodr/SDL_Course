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

#ifndef SCENEHIGHSCORES_HPP
#define SCENEHIGHSCORES_HPP

#include <iostream>
#include <memory>
#include <sstream>
#include <iomanip>
#include "ResourceIDs.hpp"
#include "DemoEngine/Singleton.hpp"
#include "DemoEngine/Scene.hpp"
#include "DemoEngine/Game.hpp"
#include "DemoEngine/Font.hpp"
#include "HighscoreList.hpp"

using namespace DemoEngine;
using std::cout;
using std::endl;
using std::make_shared;

class SceneHighscores : public CScene
{
    public:

        typedef enum class {
            START = 0,          // Start the music fading
            FADE_IN,            // Handle the fading
            DISPLAY,            // Fade completed, just waiting here
            PRE_FADE_OUT,       // Prepare to fade out
            FADE_OUT,           // Handle the fading
            END = 999           // Stop scene automatically (=999)
        } STATE;

        SceneHighscores() {};
        virtual ~SceneHighscores() {};

        void Initialize() override;

        void InitHandlers() override;
        void Load() override;
        void OnEnter() override;
        void OnExit() override;

        void FadeMusicIn();
        void FadeMusicOut();

        void RenderHighscores( unique_ptr<CRenderer>& renderer );

    protected:
        Uint8 m_iScreenAlpha = 0;

    private:
        bool m_bDisplayNote = true;
        int m_noteImgWidth = 0;

        // Screen info
        int m_iScreenW = 0;
        int m_iScreenH = 0;

        int m_nMenuYAnimator = 0;
};

#endif // SCENEHIGHSCORES_HPP
