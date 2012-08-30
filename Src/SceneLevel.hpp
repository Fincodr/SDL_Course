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

#ifndef SCENELEVEL1_HPP
#define SCENELEVEL1_HPP

#include <iostream>
#include <memory>
#include <unordered_map>
#include <forward_list>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include "ResourceIDs.hpp"
#include "DemoEngine/Scene.hpp"
#include "DemoEngine/Game.hpp"
#include "DemoEngine/ScrollingBackground.hpp"
#include "DemoEngine/Rectangle.hpp"
#include "DemoEngine/Interpolation.hpp"
#include "DemoEngine/Math.hpp"
#include "EntityPlayer.hpp"
#include "EntityEnemy.hpp"
#include "EntityProjectile.hpp"
#include "EntityExplosion.hpp"

using namespace DemoEngine;
using std::cout;
using std::endl;
using std::make_pair;
using std::unique_ptr;
using std::unordered_map;
using std::forward_list;
using std::vector;

typedef CGameObjectFloat GameObject_t;
typedef unordered_map<Uint32, shared_ptr<GameObject_t>> GameObjectList_t;
typedef forward_list<shared_ptr<GameObject_t>> DeadGameObjectList_t;

class SceneLevel : public CScene
{
    public:
        // if we want to limit the play area (no we dont)
        const int kPlayerXMargins = 0;
        const int kPlayerYMargins = 32;

        const int kEnemyHealth = 100;
        const int kPlayerHealth = 500;

        const int kBulletDamage = 25;
        const int kGuidedBulletDamage = 100;
        const int kEnemyDamage = 100;

        const int kPlayerDamage = 200;

        const int kEnemyScore = 100;
        const int kBulletTimeAdder = 250;

        const int kInitialPlayerCooldown = 300;
        const int kInitialPlayerBulletTime = 1000;
        const int kPlayerBulletTimeMin = 500;
        const int kPlayerBulletTimeMax = 5000;

        const float kfEasyEnemyCooldown = 1.50f;
        const float kfHardEnemyCooldown = 0.30f;

        typedef enum class {
            START = 0,
            FADE_IN,
            GAME,
            FADE_OUT,
            END = 999           // Stop scene automatically (=999)
        } STATE;

        SceneLevel() : m_umapEnemies(), m_umapDeadEnemies(), m_umapBullets(), m_umapDeadBullets(), m_umapExplosions(), m_umapDeadExplosions(), tmpDeadLst(), m_blankImg(), m_RectShields(), m_RectEnergy() {};
        ~SceneLevel() {};

        void Initialize() override;

        void InitHandlers() override;
        void Load() override;
        void OnEnter() override;
        void OnExit() override;

        void PlayerFire();
        void EnemyFire( shared_ptr<GameObject_t> enemy );
        void DeployEnemy();
        void Explosion( int x, int y, int frame = 0, int fps = 30 );
        void KillPlayer();
        void NextLevel();

        shared_ptr<GameObject_t> RespawnBullet();
        shared_ptr<GameObject_t> RespawnEnemy();
        shared_ptr<GameObject_t> RespawnExplosion();

        void FadeMusicIn();
        void FadeMusicOut();

    protected:
    private:
        shared_ptr<EntityPlayer> m_pPlayer = nullptr;

        // Enemies
        GameObjectList_t m_umapEnemies;
        DeadGameObjectList_t m_umapDeadEnemies;

        // Bullets
        GameObjectList_t m_umapBullets;
        DeadGameObjectList_t m_umapDeadBullets;

        // Explosions
        GameObjectList_t m_umapExplosions;
        DeadGameObjectList_t m_umapDeadExplosions;

        vector<Uint32> tmpDeadLst;

        int m_score = 0;
        int m_scoreOld = -1;

        int m_level = 0;
        int m_levelOld = -1;
        Uint32 m_levelStartTick = 0;

        // unordered_map< shared_ptr<Explosion>> m_umapExplosions;

        CRectangle m_blankImg;
        Uint8 m_iScreenAlpha = 0;

        // Keys
        bool m_bKeyUp = false;
        bool m_bKeyDown = false;
        bool m_bKeyLeft = false;
        bool m_bKeyRight = false;
        bool m_bKeySpace = false;
        bool m_bKeyZ = false;
        bool m_bFire = false;

        // Statistics
        int m_iPlayerFireCount = 0;
        int m_iPlayerFiredTotal = 0;
        int m_iEnemyKilledTotal = 0;
        int m_iEnemyKilled = 0;
        int m_iEnemyDeployedTotal = 0;
        int m_iEnemyHitTotal = 0;
        bool m_bLevelStarted = false;
        bool m_bImmortal = false;

        // Firing cool down timer
        bool m_bBulletTime = false;
        float m_fBulletTimeFactor = 1.0f;
        int m_iBulletTimeState = 0; // states: 0=normal, 1=slowing_down, 2=slow, 3=speeding_up
        int m_iPlayerCooldown = 0;
        Uint32 m_iCooldownTick = 0;
        Uint32 m_iPlayerBulletTimeStartTick = 0;
        int m_iPlayerBulletTime = 0;
        int m_iPlayerBulletTimeOld = 0;
        int m_iPlayerBulletTimeMax = 0;

        // Screen info
        int m_iScreenW = 0;
        int m_iScreenH = 0;

        // Shields indicator
        CRectangle m_RectShields;

        // Energy indicator
        CRectangle m_RectEnergy;

        // Ship deceleration timers
        Uint32 m_iTurnTick = 0;
};

#endif // SCENELEVEL1_HPP
