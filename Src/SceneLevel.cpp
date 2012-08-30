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

#include "SceneLevel.hpp"

void SceneLevel::Initialize() {
    cout << "SceneLevel::Initialize() called" << endl;
    // Initialize handlers
    InitHandlers();
    SetInitialized( true );
}

/** \brief
 *
 * \return void
 *
 */
void SceneLevel::Load()
{
    #ifdef DEBUG
    cout << "SceneLevel Load called." << endl;
    #endif

    SetLoaded( true );
}

void SceneLevel::OnEnter()
{
    #ifdef DEBUG
    cout << "SceneLevel OnEnter() called." << endl;
    #endif

    // Assert on lists that should be empty at this point
    TEST_ASSERT( GetRenderables().empty(), "Renderables must be empty when entering scene." );
    TEST_ASSERT( GetPreRenderables().empty(), "PreRenderables must be empty when entering scene." );
    TEST_ASSERT( GetPostRenderables().empty(), "PostRenderables must be empty when entering scene." );
    TEST_ASSERT( GetUpdateables().empty(), "Updateables must be empty when entering scene." );
    TEST_ASSERT( m_umapEnemies.empty(), "Enemies map must be empty when entering scene." );
    TEST_ASSERT( m_umapDeadEnemies.empty(), "Dead enemies map must be empty when entering scene." );
    TEST_ASSERT( m_umapBullets.empty(), "Bullets map must be empty when entering scene." );
    TEST_ASSERT( m_umapDeadBullets.empty(), "Dead bullets map must be empty when entering scene." );
    TEST_ASSERT( m_umapExplosions.empty(), "Explosions map must be empty when entering scene." );
    TEST_ASSERT( m_umapDeadExplosions.empty(), "Dead explosions map must be empty when entering scene." );

    auto& renderer = CSingleton<CRenderer>::Instance();
    auto screen = renderer->GetScreen();
    m_iScreenW = screen->w;
    m_iScreenH = screen->h;

    m_blankImg.SetWidth(m_iScreenW);
    m_blankImg.SetHeight(m_iScreenH);
    m_blankImg.SetPosition(0,0);

    // Create player entity
    m_pPlayer = make_shared<EntityPlayer>();
    m_pPlayer->SetPosition( m_iScreenW/2, m_iScreenH/2 );
    m_pPlayer->SetHealth( kPlayerHealth );
    m_pPlayer->SetAcceleration(0.0f, 0.0f);

    // Get references to particle effect
    auto& psExplosion = ExplosionSystemFactory::Instance()->Get( RESOURCE::PS_EXPLOSION );
    auto& psSmoke = SmokeSystemFactory::Instance()->Get( RESOURCE::PS_SMOKE );

    // Add player entity to update and renderable list
    auto& moonLayer = ScrollingBackgroundFactory::Instance()->Get( RESOURCE::SCROLLING_MOONSURFACE );
    auto& cloudsLayer = ScrollingBackgroundFactory::Instance()->Get( RESOURCE::SCROLLING_CLOUDS );

    auto& preRenderables = GetPreRenderables();
    auto& renderables = GetRenderables();
    auto& postRenderables = GetPostRenderables();
    auto& updateables = GetUpdateables();

    updateables = { { m_pPlayer->GetID(), m_pPlayer }, { psExplosion->GetID(), psExplosion }, { psSmoke->GetID(), psSmoke }, { moonLayer->GetID(), moonLayer }, { cloudsLayer->GetID(), cloudsLayer } };
    renderables = { { m_pPlayer->GetID(), m_pPlayer }, { psExplosion->GetID(), psExplosion }, { psSmoke->GetID(), psSmoke } };
    preRenderables = { { moonLayer->GetID(), moonLayer } };
    postRenderables = { { cloudsLayer->GetID(), cloudsLayer } };

    SetState( (int)STATE::START );

    // Reset score
    m_score = 0;
    m_scoreOld = -1;

    // Reset level
    m_level = 1;
    m_levelOld = -1;
    m_bLevelStarted = false;

    // Reset statistics
    m_iEnemyKilled = 0;
    m_iEnemyKilledTotal = 0;
    m_iPlayerFireCount = 0;
    m_iEnemyDeployedTotal = 0;
    m_iPlayerFiredTotal = 0;
    m_iEnemyHitTotal = 0;

    // Reset bullettime
    CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = 1.0f;
    m_iPlayerBulletTime = 0;
    m_iPlayerBulletTimeMax = kInitialPlayerBulletTime;
    m_iPlayerBulletTimeOld = -1;
    m_iPlayerBulletTimeStartTick = 0;
    m_iBulletTimeState = 0;
    m_fBulletTimeFactor = 1.0f;

    // Reset timers
    m_iCooldownTick = 0;
    m_iPlayerCooldown = kInitialPlayerCooldown;

    // Make sure no keys are down
    m_bKeyLeft = m_bKeyRight = m_bKeyUp = m_bKeyDown = m_bKeySpace = m_bFire = false;
}

void SceneLevel::OnExit()
{
    #ifdef DEBUG
    cout << "SceneLevel OnExit() called." << endl;
    #endif

    // Remove everything from update and renderable list
    auto& preRenderables = GetPreRenderables();
    auto& renderables = GetRenderables();
    auto& postRenderables = GetPostRenderables();
    auto& updateables = GetUpdateables();
    updateables.clear();
    preRenderables.clear();
    renderables.clear();
    postRenderables.clear();

    // Set player and enemies references to zero to call dtor's automatically
    m_pPlayer = nullptr;
    m_umapEnemies.clear();
    m_umapDeadEnemies.clear();
    m_umapBullets.clear();
    m_umapDeadBullets.clear();
    m_umapExplosions.clear();
    m_umapDeadExplosions.clear();
}

void SceneLevel::NextLevel()
{
    m_level++;
    m_iEnemyKilled = 0;
    m_bLevelStarted = false;

    // add bullet time
    m_iPlayerBulletTimeMax += kBulletTimeAdder;
    if ( m_iPlayerBulletTimeMax > kPlayerBulletTimeMax )
        m_iPlayerBulletTimeMax = kPlayerBulletTimeMax;

    // add some health
    int nHealth = static_cast<int>((float)m_pPlayer->GetHealth());
    int nMaxHealth = static_cast<int>((float)kPlayerHealth);
    nHealth += 250;
    if ( nHealth > nMaxHealth ) nHealth = nMaxHealth;
    m_pPlayer->SetHealth( nHealth );
}

shared_ptr<GameObject_t> SceneLevel::RespawnBullet()
{
    // Do we have dead bullet that we can use?
    if ( !m_umapDeadBullets.empty() ) {
        // Reuse dead bullet
        auto m_pBullet1 = m_umapDeadBullets.front();
        m_pBullet1->SetDead( false );
        // remove from list
        m_umapDeadBullets.pop_front();
        #ifdef DEBUG
        cout << "Reactivating bullet with ID " << m_pBullet1->GetID() << endl;
        #endif
        m_umapBullets[ m_pBullet1->GetID() ] = m_pBullet1;
        return ( m_pBullet1 );
    }
    else
    {
        // Instantiate new bullet
        auto m_pBullet1 = make_shared<EntityProjectile>();
        m_umapBullets[ m_pBullet1->GetID() ] = m_pBullet1;
        return ( m_pBullet1 );
    }
}

shared_ptr<GameObject_t> SceneLevel::RespawnEnemy()
{
    // Do we have dead enemy that we can use?
    if ( !m_umapDeadEnemies.empty() ) {
        // Reuse dead enemy
        auto m_pEnemy = m_umapDeadEnemies.front();
        m_pEnemy->SetDead( false );
        // remove from list
        m_umapDeadEnemies.pop_front();
        #ifdef DEBUG
        cout << "Reactivating enemy with ID " << m_pEnemy->GetID() << endl;
        #endif
        m_umapEnemies[ m_pEnemy->GetID() ] = m_pEnemy;
        return ( m_pEnemy );
    }
    else
    {
        // Instantiate new enemy
        auto m_pEnemy = make_shared<EntityEnemy>();
        m_umapEnemies[ m_pEnemy->GetID() ] = m_pEnemy;
        return ( m_pEnemy );
    }
}

shared_ptr<GameObject_t> SceneLevel::RespawnExplosion()
{
    // Do we have dead explosion that we can use?
    if ( !m_umapDeadExplosions.empty() ) {
        // Reuse dead explosion
        auto m_pExplosion = std::static_pointer_cast<EntityExplosion>(m_umapDeadExplosions.front());
        // remove from list
        m_umapDeadExplosions.pop_front();
        #ifdef DEBUG
        cout << "Reactivating explosion with ID " << m_pExplosion->GetID() << endl;
        #endif
        m_pExplosion->SetFrame( 0 );
        m_pExplosion->SetDead( false );
        m_umapExplosions[ m_pExplosion->GetID() ] = m_pExplosion;
        return ( m_pExplosion );
    }
    else
    {
        // Instantiate new explosion
        auto m_pExplosion = make_shared<EntityExplosion>();
        m_umapExplosions[ m_pExplosion->GetID() ] = m_pExplosion;
        return ( m_pExplosion );
    }
}
/** \brief
 *
 * \return void
 *
 */
void SceneLevel::PlayerFire()
{
    if ( !m_pPlayer->IsDead() ) {

        m_iPlayerFireCount++;

        auto& sound = CSingleton<CSoundServer>::Instance();
        sound->Play( RESOURCE::SOUND_PLAYER_FIRE );

        // Add to updateable and renderable list
        auto& updateables = GetUpdateables();
        auto& renderables = GetRenderables();

        int objID;

        int X = m_pPlayer->GetX();
        int Y = m_pPlayer->GetY();

        int iBulletDelta = 12;

        if ( m_level <= 2 ) {

            shared_ptr<EntityProjectile> m_pBullet1 = std::dynamic_pointer_cast<EntityProjectile>(RespawnBullet());
            m_pBullet1->SetPosition( X, Y-5 );
            m_pBullet1->UpdateBoundingBox();
            m_pBullet1->SetOwner( m_pPlayer->GetID() );
            m_pBullet1->SetSpeed( 0, -300 - Math::Limits::clampmax<int>(m_level*100,700) );
            m_pBullet1->SetProjectile( RESOURCE::PLAYER_PROJECTILE );
            m_pBullet1->SetHealth( kBulletDamage );
            objID = m_pBullet1->GetID();
            updateables[objID] = m_pBullet1;
            renderables[objID] = m_pBullet1;
            m_iPlayerFiredTotal++;
        }
        else
        {
            shared_ptr<EntityProjectile> m_pBullet1 = std::dynamic_pointer_cast<EntityProjectile>(RespawnBullet());
            m_pBullet1->SetPosition( X-iBulletDelta, Y-5 );
            m_pBullet1->UpdateBoundingBox();
            m_pBullet1->SetOwner( m_pPlayer->GetID() );
            m_pBullet1->SetSpeed( 0, -300 - Math::Limits::clampmax<int>(m_level*100,700) );
            m_pBullet1->SetProjectile( RESOURCE::PLAYER_PROJECTILE );
            m_pBullet1->SetHealth( kBulletDamage );
            objID = m_pBullet1->GetID();
            updateables[objID] = m_pBullet1;
            renderables[objID] = m_pBullet1;
            m_iPlayerFiredTotal++;

            shared_ptr<EntityProjectile> m_pBullet2 = std::dynamic_pointer_cast<EntityProjectile>(RespawnBullet());
            m_pBullet2->SetPosition( X+iBulletDelta, Y-5 );
            m_pBullet2->UpdateBoundingBox();
            m_pBullet2->SetOwner( m_pPlayer->GetID() );
            m_pBullet2->SetSpeed( 0, -300 - Math::Limits::clampmax<int>(m_level*100,700) );
            m_pBullet2->SetProjectile( RESOURCE::PLAYER_PROJECTILE );
            m_pBullet2->SetHealth( kBulletDamage );
            objID = m_pBullet2->GetID();
            updateables[objID] = m_pBullet2;
            renderables[objID] = m_pBullet2;
            m_iPlayerFiredTotal++;
        }

        if ( m_level > 8 )
        {

            shared_ptr<EntityProjectile> m_pBullet3 = std::dynamic_pointer_cast<EntityProjectile>(RespawnBullet());
            m_pBullet3->SetPosition( X-iBulletDelta, Y-5 );
            m_pBullet3->UpdateBoundingBox();
            m_pBullet3->SetOwner( m_pPlayer->GetID() );
            m_pBullet3->SetSpeed( -350, -1000 );
            m_pBullet3->SetProjectile( RESOURCE::PLAYER_PROJECTILE );
            m_pBullet3->SetHealth( kBulletDamage );
            objID = m_pBullet3->GetID();
            updateables[objID] = m_pBullet3;
            renderables[objID] = m_pBullet3;
            m_iPlayerFiredTotal++;

            shared_ptr<EntityProjectile> m_pBullet4 = std::dynamic_pointer_cast<EntityProjectile>(RespawnBullet());
            m_pBullet4->SetPosition( X+iBulletDelta, Y-5 );
            m_pBullet4->UpdateBoundingBox();
            m_pBullet4->SetOwner( m_pPlayer->GetID() );
            m_pBullet4->SetSpeed( 350, -1000 );
            m_pBullet4->SetProjectile( RESOURCE::PLAYER_PROJECTILE );
            m_pBullet4->SetHealth( kBulletDamage );
            objID = m_pBullet4->GetID();
            updateables[objID] = m_pBullet4;
            renderables[objID] = m_pBullet4;
            m_iPlayerFiredTotal++;
        }

        int x2 = -1;
        int y2 = -1;
        float fDistanceMin = 99999.0f;
        int xs = -1;
        int ys = -1;
        if ( m_level > 12 ) {
            // Guided projectile for player
            // Check which enemy is the closest to the player
            // check collision between enemy and player
            for ( auto& enemy : m_umapEnemies ) {
                // is the enemy alive and on screen?
                if ( enemy.second->GetY() > 0 && !enemy.second->IsDead() ) {
                    float fDistance = Math::Coordinates::GetDistance( X, Y, enemy.second->GetX(), enemy.second->GetY() );
                    if ( fDistance < fDistanceMin )
                    {
                        fDistanceMin = fDistance;
                        x2 = enemy.second->GetX();
                        y2 = enemy.second->GetY();
                        xs = enemy.second->GetSpeed()[0];
                        ys = enemy.second->GetSpeed()[1];
                    }
                }
            }
        }

        if ( x2 != -1 && ( m_iPlayerFireCount% (10-Math::Limits::clampmax<int>(m_level-12,5))==0) && m_level > 12 )
        {
            shared_ptr<EntityProjectile> m_pBullet5 = std::dynamic_pointer_cast<EntityProjectile>(RespawnBullet());
            m_pBullet5->SetPosition( X, Y+5 );
            m_pBullet5->UpdateBoundingBox();
            m_pBullet5->SetOwner( m_pPlayer->GetID() );
            m_pBullet5->SetProjectile( RESOURCE::PLAYER_PROJECTILE_GUIDED );
            m_pBullet5->SetHealth( kGuidedBulletDamage );
            float fProjectileSpeed = 300.0f + Math::Limits::clampmax<int>((m_level-12)*10,500);
            float fFlyTime = fDistanceMin / fProjectileSpeed;
            int x3 = x2 + xs*fFlyTime;
            int y3 = y2 + ys*fFlyTime;
            // Direct bullets to player position
            float degrees = Math::Coordinates::GetAngleForLine( X, Y+5, x3, y3 );
            float radians = ((float)degrees-180) * DemoEngine::Math::kPI / 180;
            m_pBullet5->SetSpeed( sin(radians)*fProjectileSpeed, cos(radians)*fProjectileSpeed );
            objID = m_pBullet5->GetID();
            updateables[objID] = m_pBullet5;
            renderables[objID] = m_pBullet5;
            m_iPlayerFiredTotal++;
        }
    }
}

void SceneLevel::EnemyFire( shared_ptr<GameObject_t> enemy )
{
    if ( !enemy->IsDead() && !m_pPlayer->IsDead() ) {

        auto enemyClass = std::static_pointer_cast<EntityEnemy>(enemy);

        auto& sound = CSingleton<CSoundServer>::Instance();
        sound->Play( RESOURCE::SOUND_ENEMY_FIRE );

        auto m_pBullet1 = std::dynamic_pointer_cast<EntityProjectile>(RespawnBullet());

        int m_iX = enemy->GetX();
        int m_iY = enemy->GetY();
        int m_iX2 = m_pPlayer->GetX();
        int m_iY2 = m_pPlayer->GetY();
        m_pBullet1->SetPosition( m_iX, m_iY+3 );
        m_pBullet1->SetHealth( kBulletDamage );

        int nEnemyType = enemyClass->GetEnemyType();

        float fProjectileSpeed;
        if ( nEnemyType == 0 )
        {
            if ( m_level < 3 )
            {
                fProjectileSpeed = 100.0f + Math::Limits::clampmax<int>(m_level*10,300);
                // Direct bullets to random positions (only forward of enemy plane)
                float degrees = 180; //90 + rand()%180;
                float radians = ((float)degrees-180) * DemoEngine::Math::kPI / 180;
                m_pBullet1->SetSpeed( sin(radians)*fProjectileSpeed, cos(radians)*fProjectileSpeed );
            }
            else
            {
                fProjectileSpeed = 100.0f + Math::Limits::clampmax<int>(m_level*10,300);
                float fDistance = Math::Coordinates::GetDistance( m_iX, m_iY+3, m_iX2, m_iY2 );
                float fFlyTime = fDistance / fProjectileSpeed;
                int m_iX3 = m_iX2 + Math::Limits::clampmax<float>(m_pPlayer->GetSpeed()[0],250.f)*fFlyTime;
                int m_iY3 = m_iY2 + Math::Limits::clampmax<float>(m_pPlayer->GetSpeed()[1],250.f)*fFlyTime;
                // if the calculated x position is outside of the screen we should just fire at the player position instead
                if ( m_iX3 < 0 || m_iX3 > m_iScreenW-1 )
                {
                    m_iX3 = m_iX2;
                    m_iY3 = m_iY2;
                }
                // Direct bullets to player position
                float degrees = Math::Coordinates::GetAngleForLine( m_iX, m_iY+3, m_iX3, m_iY3 );
                // randomize accuracy a bit
                int randDegreesMax = 45 - Math::Limits::clampmax<int>(m_level*2,44);
                degrees += -randDegreesMax/2 + rand()%randDegreesMax;
                // only shoot forward (so degrees must be >90 and <270
                if ( degrees < 90 || degrees > 270 )
                {
                    degrees = 180;
                }
                float radians = ((float)degrees-180) * DemoEngine::Math::kPI / 180;
                m_pBullet1->SetSpeed( sin(radians)*fProjectileSpeed, cos(radians)*fProjectileSpeed );
            }
        }
        else
        {
            fProjectileSpeed = 300.0f + Math::Limits::clampmax<int>(m_level*10,100);
            float fDistance = Math::Coordinates::GetDistance( m_iX, m_iY+3, m_iX2, m_iY2 );
            float fFlyTime = fDistance / fProjectileSpeed;
            int m_iX3 = m_iX2 + Math::Limits::clampmax<float>(m_pPlayer->GetSpeed()[0],250.f)*fFlyTime;
            int m_iY3 = m_iY2 + Math::Limits::clampmax<float>(m_pPlayer->GetSpeed()[1],250.f)*fFlyTime;
            // if the calculated x position is outside of the screen we should just fire at the player position instead
            if ( m_iX3 < 0 || m_iX3 > m_iScreenW-1 )
            {
                m_iX3 = m_iX2;
                m_iY3 = m_iY2;
            }
            // Direct bullets to player position
            float degrees = Math::Coordinates::GetAngleForLine( m_iX, m_iY+3, m_iX3, m_iY3 );
            // randomize accuracy a bit (not that much for harded enemies)
            int randDegreesMax = 20 - Math::Limits::clampmax<int>(m_level*2,19);
            degrees += -randDegreesMax/2 + rand()%randDegreesMax;
            float radians = ((float)degrees-180) * DemoEngine::Math::kPI / 180;
            m_pBullet1->SetSpeed( sin(radians)*fProjectileSpeed, cos(radians)*fProjectileSpeed );
        }

        m_pBullet1->UpdateBoundingBox();
        m_pBullet1->SetOwner( enemy->GetID() );
        if ( nEnemyType == 0 )
            m_pBullet1->SetProjectile( RESOURCE::ENEMY_PROJECTILE_SLOW );
        else
            m_pBullet1->SetProjectile( RESOURCE::ENEMY_PROJECTILE_FAST );

        // Add to updateable and renderable list
        auto& updateables = GetUpdateables();
        auto& renderables = GetRenderables();

        int objID = m_pBullet1->GetID();
        updateables[objID] = m_pBullet1;
        renderables[objID] = m_pBullet1;
    }
}

void SceneLevel::DeployEnemy()
{
    #ifdef DEBUG
    cout << "Deploying enemy!" << endl;
    #endif
    m_iEnemyDeployedTotal++;

    int YMAX = -(65*2);
    for ( auto& enemy : m_umapEnemies ) {
        if ( enemy.second->GetY() < YMAX ) {
            YMAX = enemy.second->GetY();
        }
    }

    shared_ptr<GameObject_t> m_pEnemy = RespawnEnemy();

    auto enemyClass = std::static_pointer_cast<EntityEnemy>(m_pEnemy);
    int nEnemyType;
    if ( m_iEnemyDeployedTotal%(10-Math::Limits::clampmax<int>(static_cast<int>((float)m_level/3),9)) == 0 )
        nEnemyType = 1;
    else
        nEnemyType = 0;

    enemyClass->SetPosition( rand() % ( m_iScreenW - (65) ) + 65/2, YMAX - 65*3 );
    enemyClass->SetHealth( ( (float)kEnemyHealth * (1.0f+(float)nEnemyType) ) * (1.0f+Math::Limits::clamp<float>(static_cast<float>((float)(m_level-10)/10.0f),0.0f,3.0f)) );
    enemyClass->SetSpeed( 0, 25 + (rand()%50 + (nEnemyType*50)) + (Math::Limits::clampmax<float>(static_cast<float>((float)m_level*5.0f),100.0f)) );
    enemyClass->SetEnemyType( nEnemyType );

    // Add to updateable and renderable list
    auto& updateables = GetUpdateables();
    auto& renderables = GetRenderables();

    int objID = enemyClass->GetID();
    updateables[objID] = m_pEnemy;
    renderables[objID] = m_pEnemy;
}

void SceneLevel::Explosion( int x, int y, int frame, int fps )
{
    shared_ptr<EntityExplosion> m_pExplosion = std::static_pointer_cast<EntityExplosion>(RespawnExplosion());

    // Add to updateable and renderable list
    auto& updateables = GetUpdateables();
    auto& renderables = GetRenderables();

    int objID = m_pExplosion->GetID();
    updateables[objID] = m_pExplosion;
    renderables[objID] = m_pExplosion;

    m_pExplosion->SetPosition( x, y );
    m_pExplosion->SetFrame( frame );
    m_pExplosion->SetFPS( fps );
    m_pExplosion->Execute();
}

void SceneLevel::FadeMusicIn() {
    auto& sound = CSingleton<CSoundServer>::Instance();
    sound->FadeInMusic( RESOURCE::MUSIC_INGAME, 2000 );
}

void SceneLevel::FadeMusicOut() {
    auto& sound = CSingleton<CSoundServer>::Instance();
    sound->FadeOutMusic( 1000 );
}

void SceneLevel::KillPlayer() {
    // Set player to dead
    m_pPlayer->SetDead( true );

    // Deploy Plane explosion
    Explosion( m_pPlayer->GetX(), m_pPlayer->GetY(), 0, 15 );

    // Deploy secondary explosions
    Explosion( m_pPlayer->GetX() + -25+rand()%50, m_pPlayer->GetY() + -25+rand()%50, 8, 15 );
    Explosion( m_pPlayer->GetX() + -25+rand()%50, m_pPlayer->GetY() + -25+rand()%50, 8, 15 );
    Explosion( m_pPlayer->GetX() + -25+rand()%50, m_pPlayer->GetY() + -25+rand()%50, 8, 15 );
    Explosion( m_pPlayer->GetX() + -15+rand()%30, m_pPlayer->GetY() + -15+rand()%30, 4, 6 );
    Explosion( m_pPlayer->GetX() + -15+rand()%30, m_pPlayer->GetY() + -15+rand()%30, 4, 6 );
    Explosion( m_pPlayer->GetX() + -15+rand()%30, m_pPlayer->GetY() + -15+rand()%30, 4, 6 );

    // Play sound effect
    auto& sound = CSingleton<CSoundServer>::Instance();
    sound->Play( RESOURCE::SOUND_EXPLOSION1 );

    // Store score in properties
    auto& properties = CSingleton<CProperties>::Instance();
    int accuracy = 0;
    if ( m_iPlayerFiredTotal > 0 && m_iEnemyHitTotal > 0 )
        accuracy = static_cast<int>(((float)m_iEnemyHitTotal/(float)m_iPlayerFiredTotal)*100);
    properties->Property( "Player", "Score" ) = static_cast<int>(m_score);
    properties->Property( "Player", "Fired" ) = static_cast<int>(m_iPlayerFiredTotal);
    properties->Property( "Player", "Kills" ) = static_cast<int>(m_iEnemyKilledTotal);
    properties->Property( "Player", "Accuracy" ) = static_cast<int>(accuracy);
    properties->Property( "Player", "Level" ) = static_cast<int>(m_level);
}

/** \brief
 *
 * \return void
 *
 */
void SceneLevel::InitHandlers()
{
    #ifdef DEBUG
    cout << "SceneLevel InitHandlers() called." << endl;
    #endif
    ///////////////////////////////////////////////////
    //
    // OnKeyUp
    //
    ///////////////////////////////////////////////////
    m_Handlers["OnKeyUp"] = {
        {
            //
            // STATE::GAME
            //
            (int)STATE::GAME,
            [&](SDL_Event& ev){
                switch ( ev.key.keysym.sym ) {
                    case SDLK_z:
                        m_bKeyZ = false;
                        break;
                    case SDLK_UP:
                        m_bKeyUp = false;
                        break;
                    case SDLK_DOWN:
                        m_bKeyDown = false;
                        break;
                    case SDLK_LEFT:
                        m_bKeyLeft = false;
                        break;
                    case SDLK_RIGHT:
                        m_bKeyRight = false;
                        break;
                    case SDLK_SPACE:
                        m_bKeySpace = false;
                        break;
                    default:
                        break;
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
            // STATE::GAME
            //
            (int)STATE::GAME,
            [&](SDL_Event& ev){
                switch ( ev.key.keysym.sym ) {
                    case SDLK_z:
                        if ( !m_bKeyZ ) {
                            m_bKeyZ = true;
                        }
                        break;
                    case SDLK_UP:
                        m_bKeyUp = true;
                        break;
                    case SDLK_DOWN:
                        m_bKeyDown = true;
                        break;
                    case SDLK_LEFT:
                        m_bKeyLeft = true;
                        break;
                    case SDLK_RIGHT:
                        m_bKeyRight = true;
                        break;
                    case SDLK_n:
                        // Destroy all enemies and bullets
                        {
                            for ( auto& enemy : m_umapEnemies ) {
                                enemy.second->SetDead( true );
                            }
                            for ( auto& bullet : m_umapBullets ) {
                                bullet.second->SetDead( true );
                            }
                        }
                        NextLevel();
                        break;
                    case SDLK_i:
                        // Toggle immortal
                        {
                            m_bImmortal == false ? m_bImmortal = true : m_bImmortal = false;
                            cout << "Info: Immortal set to " << std::boolalpha << m_bImmortal << endl;
                        }
                        break;
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
                        KillPlayer();
                        break;
                    case SDLK_SPACE:
                        m_bKeySpace = true;
                        m_bFire = true;
                        break;
                    #ifdef DEBUG
                    case SDLK_t:
                        {
                            auto& renderables = GetRenderables();
                            auto& updateables = GetUpdateables();
                            shared_ptr<GameObject_t> m_pEnemy = RespawnEnemy();
                            m_pEnemy->SetPosition( m_iScreenW/2, 64 ); //m_iScreenW/2, 32 ); //-(1024 + (rand() % (m_iScreenH*5))) );
                            m_pEnemy->SetHealth( kEnemyHealth );
                            m_pEnemy->SetSpeed( 0, 0 ); //25 + (rand()%75) );
                            int objID = m_pEnemy->GetID();
                            updateables[objID] = m_pEnemy;
                            renderables[objID] = m_pEnemy;
                        }
                        break;
                    case SDLK_r:
                        m_pPlayer->GetAcceleration()[0] = 0.0f;
                        m_pPlayer->GetAcceleration()[1] = 0.0f;
                        break;
                    #endif
                    case SDLK_d:
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
                    m_iScreenAlpha = 255;
                    if ( CSingleton<CProperties>::Instance()->Property( "Game", "Music" ) )
                        FadeMusicIn();
                    SetState( (int)STATE::GAME );
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
                if ( m_iScreenAlpha <=0 )
                {
                    SetState( (int)STATE::END );
                }
            }
        },
        {
            //
            // STATE::GAME
            //
            (int)STATE::GAME,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );

                // Fire if player is pressing space
                if ( m_bKeySpace || m_bFire ) {
                    if ( SDL_GetTicks() - m_iCooldownTick >= m_iPlayerCooldown / (1.0f+Math::Limits::clampmax<float>((float)m_level/(float)20,3.0f)) )
                    {
                        m_bFire = false;
                        PlayerFire();
                        m_iCooldownTick = SDL_GetTicks();
                    }
                }

                // If player is pressing Z key start bullet time
                // (if not already in bullet time)
                if ( m_bKeyZ )
                {
                    if ( !m_bBulletTime )
                    {
                        // Start new bullet time
                        m_bBulletTime = true;
                        #ifdef DEBUG
                        cout << "BULLETTIME::TRY_TO_START" << endl;
                        #endif
                    }
                }
                else
                {
                    if ( m_bBulletTime )
                    {
                        // End bullet time
                        m_bBulletTime = false;
                        #ifdef DEBUG
                        cout << "BULLETTIME::END" << endl;
                        #endif
                    }
                }

                switch ( m_iBulletTimeState )
                {

                // normal gameplay speed
                default:
                case 0:
                    {
                        // bullet time is requested, check if player has enough bullet time to start
                        if ( m_bBulletTime && m_iPlayerBulletTime-kPlayerBulletTimeMin>0 )
                        {
                            // yes, start bullet time (slowing down)
                            m_iPlayerBulletTimeStartTick = SDL_GetTicks();
                            m_iPlayerBulletTimeOld = m_iPlayerBulletTime;
                            m_iBulletTimeState++;
                            #ifdef DEBUG
                            cout << "BULLETTIME::STATE==SLOWING_DOWN" << endl;
                            #endif
                        }
                        else
                        {
                            if ( m_iPlayerBulletTimeOld == -1 )
                            {
                                m_iPlayerBulletTimeStartTick = SDL_GetTicks();
                                m_iPlayerBulletTimeOld = m_iPlayerBulletTime;
                            }

                            int iTime = SDL_GetTicks() - m_iPlayerBulletTimeStartTick;
                            m_iPlayerBulletTime = m_iPlayerBulletTimeOld + iTime / 2;
                            if ( m_iPlayerBulletTime > m_iPlayerBulletTimeMax )
                            {
                                m_iPlayerBulletTime = m_iPlayerBulletTimeMax;
                                m_iPlayerBulletTimeOld = m_iPlayerBulletTime;
                                m_iPlayerBulletTimeStartTick = SDL_GetTicks();
                            }
                        }

                    }
                    break;

                // slowing down
                case 1:
                    {
                        int iTime = SDL_GetTicks() - m_iPlayerBulletTimeStartTick;
                        if ( iTime < 500 ) {
                            float factor = 1.0f - ((float)iTime/500);
                            if ( factor < 0.05f ) factor = 0.05f;
                            CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = factor;
                            m_iPlayerBulletTime = m_iPlayerBulletTimeOld - iTime;
                        }
                        else
                        {
                            m_iPlayerBulletTime = m_iPlayerBulletTimeOld - kPlayerBulletTimeMin;
                            CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = 0.05f;
                            m_iBulletTimeState++;
                            m_iPlayerBulletTimeStartTick = SDL_GetTicks();
                            m_iPlayerBulletTimeOld = m_iPlayerBulletTime;
                            #ifdef DEBUG
                            cout << "BULLETTIME::STATE==SLOW" << endl;
                            #endif
                        }
                    }
                    break;

                // normal slowed down gameplay speed
                case 2:
                    {
                        // eat bullet time at constant rate
                        int iTime = SDL_GetTicks() - m_iPlayerBulletTimeStartTick;
                        if ( m_iPlayerBulletTimeOld - iTime > 0 )
                            m_iPlayerBulletTime = m_iPlayerBulletTimeOld - iTime;
                        else
                        {
                            m_bBulletTime = false;
                            m_iPlayerBulletTime = 0;
                            m_bKeyZ = false;
                        }
                        if ( !m_bBulletTime )
                        {
                            m_iBulletTimeState++;
                            m_iPlayerBulletTimeStartTick = SDL_GetTicks();
                            #ifdef DEBUG
                            cout << "BULLETTIME::STATE==SPEEDING_UP" << endl;
                            #endif
                            m_iPlayerBulletTimeStartTick = SDL_GetTicks();
                            m_iPlayerBulletTimeOld = m_iPlayerBulletTime;
                        }
                    }
                    break;

                // speedind up
                case 3:
                    int iTime = SDL_GetTicks() - m_iPlayerBulletTimeStartTick;
                    if ( iTime < 1000 ) {
                        float factor = ((float)iTime/1000);
                        if ( factor < 0.05f ) factor = 0.05f;
                        CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = factor;
                        m_iPlayerBulletTime = m_iPlayerBulletTimeOld + iTime / 2;
                        if ( m_iPlayerBulletTime > m_iPlayerBulletTimeMax )
                        {
                            m_iPlayerBulletTime = m_iPlayerBulletTimeMax;
                        }
                    }
                    else
                    {
                        CSingleton<CProperties>::Instance()->Property( "Game", "Speed" ) = 1.0f;
                        m_iBulletTimeState = 0;
                        #ifdef DEBUG
                        cout << "BULLETTIME::STATE==NORMAL" << endl;
                        #endif
                    }
                    break;
                }

                /// DO ENEMY AI
                for ( auto& enemy : m_umapEnemies ) {

                    auto enemyClass = std::static_pointer_cast<EntityEnemy>(enemy.second);

                    if ( enemyClass->GetY() > 0 && !enemyClass->IsDead() ) {

                        if ( enemyClass->GetY() > m_iScreenH+65 )
                        {
                            if ( m_pPlayer->IsDead() ) {
                                // kill enemy because player is dead too
                                enemyClass->SetDead( true );
                            } else {
                                // Respawn to last
                                int YMAX = -(65*2);
                                for ( auto& enemyTemp : m_umapEnemies ) {
                                    if ( enemyTemp.second->GetY() < YMAX ) {
                                        YMAX = enemyTemp.second->GetY();
                                    }
                                }
                                enemyClass->SetPosition( rand() % ( m_iScreenW - (65) ) + 65/2, YMAX - 65*2 );
                            }
                        }

                        if ( m_level > 1 )
                        {
                            int nEnemyType = enemyClass->GetEnemyType();
                            if ( nEnemyType == 0 )
                            {
                                // easy enemy
                                if ( enemyClass->GetCooldownTimer() >= kfEasyEnemyCooldown / (0.1f+Math::Limits::clampmax<float>((float)m_level/(float)25,4.0f)) )
                                {
                                    enemyClass->Fire();
                                    EnemyFire( enemy.second );
                                }
                            }
                            else
                            {
                                // hard enemy
                                if ( enemyClass->GetCooldownTimer() >= kfHardEnemyCooldown / (0.1f+Math::Limits::clampmax<float>((float)m_level/(float)25,4.0f)) )
                                {
                                    enemyClass->Fire();
                                    EnemyFire( enemy.second );
                                }
                            }
                        }

                    }

                }

                /// CHECK COLLISIONS FROM LAST RENDERER SCENE

                // check bullets vs enemies and player
                for ( auto& bullet : m_umapBullets ) {

                    // is the bullet alive
                    if ( !bullet.second->IsDead() )
                    {

                        // is the bullet from player ship
                        if ( bullet.second->GetOwner() == m_pPlayer->GetID() ) {

                            // check vs enemy bullet
                            for ( auto& bullet2 : m_umapBullets ) {
                                if ( bullet2.second->GetOwner() != m_pPlayer->GetID() ) {

                                    // Check collision between bullet vs bullet
                                    if ( CCollisionDetector::Collides( bullet.second->GetBoundingBox(), bullet2.second->GetBoundingBox() ) ) {

                                        bullet.second->SetDead( true );
                                        bullet2.second->SetDead( true );

                                        // Fire small particle explosion(s) at the hit points
                                        auto& psExplosion = ExplosionSystemFactory::Instance()->Get( RESOURCE::PS_EXPLOSION );
                                        psExplosion->SetPosition( CVector2f( bullet.second->GetX(), bullet.second->GetY() ) );
                                        psExplosion->FireParticles(4);
                                        psExplosion->SetPosition( CVector2f( bullet2.second->GetX(), bullet2.second->GetY() ) );
                                        psExplosion->FireParticles(4);
                                    }
                                }
                            }

                            // check vs enemy ship
                            for ( auto& enemy : m_umapEnemies ) {

                                auto enemyClass = std::static_pointer_cast<EntityEnemy>(enemy.second);

                                // is the enemy alive and on screen?
                                if ( enemy.second->GetY() > 0 && !enemy.second->IsDead() ) {

                                    // Check collision between bullet vs enemy
                                    if ( CCollisionDetector::Collides( enemy.second->GetBoundingBox(), bullet.second->GetBoundingBox() ) ) {

                                        #ifdef DEBUG
                                        cout << "Enemy #" << enemy.second->GetID() << " collided with Bullet #" << bullet.second->GetID() << endl;
                                        #endif

                                        // destroy bullet
                                        bullet.second->SetDead( true );

                                        // deploy explosion at the bullet hit point
                                        Explosion( bullet.second->GetX(), bullet.second->GetY(), 10 );

                                        auto& sound = CSingleton<CSoundServer>::Instance();
                                        sound->Play( RESOURCE::SOUND_EXPLOSION2 );

                                        enemyClass->Hit();

                                        m_iEnemyHitTotal++;

                                        // decrease health of enemy ship by bullet damage amount
                                        if ( enemy.second->DecreaseHealth( bullet.second->GetHealth() ) ) {

                                            // Deploy Plane explosion
                                            Explosion( enemy.second->GetX(), enemy.second->GetY() );

                                            // Deploy secondary explosions
                                            Explosion( enemy.second->GetX() + -25+rand()%50, enemy.second->GetY() + -25+rand()%50, 8 );
                                            Explosion( enemy.second->GetX() + -25+rand()%50, enemy.second->GetY() + -25+rand()%50, 8 );
                                            Explosion( enemy.second->GetX() + -25+rand()%50, enemy.second->GetY() + -25+rand()%50, 14 );
                                            Explosion( enemy.second->GetX() + -25+rand()%50, enemy.second->GetY() + -25+rand()%50, 14 );

                                            sound->Play( RESOURCE::SOUND_EXPLOSION1 );

                                            // Kill enemy
                                            enemy.second->SetDead( true );

                                            // Add statistics
                                            m_iEnemyKilled++;
                                            m_iEnemyKilledTotal++;

                                            m_score+=kEnemyScore;

                                        }

                                    }

                                }

                            }

                        }
                        else
                        {
                            // bullet must be from enemy ships
                            // check collision against player ship

                            // is the player alive
                            if ( !m_pPlayer->IsDead() ) {

                                // Check collision between bullet vs player
                                if ( CCollisionDetector::Collides( m_pPlayer->GetBoundingBox(), bullet.second->GetBoundingBox() ) ) {

                                    #ifdef DEBUG
                                    cout << "Player #" << m_pPlayer->GetID() << " collided with Bullet #" << bullet.second->GetID() << endl;
                                    #endif

                                    // destroy bullet
                                    bullet.second->SetDead( true );

                                    // deploy explosion at the bullet hit point
                                    Explosion( bullet.second->GetX(), bullet.second->GetY(), 10 );

                                    auto& sound = CSingleton<CSoundServer>::Instance();
                                    sound->Play( RESOURCE::SOUND_EXPLOSION2 );

                                    m_pPlayer->Hit();

                                    if ( !m_bImmortal )
                                    {
                                        // decrease health of player ship by enemy damage amount
                                        if ( m_pPlayer->DecreaseHealth( bullet.second->GetHealth() ) ) {
                                            KillPlayer();
                                        }
                                    }

                                }

                            }

                        }

                    }

                }

                // check collision between enemy and player
                for ( auto& enemy : m_umapEnemies ) {

                    auto enemyClass = std::static_pointer_cast<EntityEnemy>(enemy.second);

                    // is the enemy alive and on screen?
                    if ( enemy.second->GetY() > 0 && !enemy.second->IsDead() ) {

                        // check player vs enemy
                        if ( !m_pPlayer->IsDead() && CCollisionDetector::Collides( enemy.second->GetBoundingBox(), m_pPlayer->GetBoundingBox() ) ) {

                            auto& sound = CSingleton<CSoundServer>::Instance();
                            sound->Play( RESOURCE::SOUND_EXPLOSION1 );

                            m_pPlayer->Hit();
                            enemyClass->Hit();

                            // decrease health of enemy ship by player damage amount
                            if ( enemy.second->DecreaseHealth( kPlayerDamage ) ) {

                                // Deploy Plane explosion
                                Explosion( enemy.second->GetX(), enemy.second->GetY() );

                                // Deploy secondary explosions
                                Explosion( enemy.second->GetX() + -25+rand()%50, enemy.second->GetY() + -25+rand()%50, 8 );
                                Explosion( enemy.second->GetX() + -25+rand()%50, enemy.second->GetY() + -25+rand()%50, 8 );
                                Explosion( enemy.second->GetX() + -25+rand()%50, enemy.second->GetY() + -25+rand()%50, 14 );
                                Explosion( enemy.second->GetX() + -25+rand()%50, enemy.second->GetY() + -25+rand()%50, 14 );

                                // Kill enemy
                                enemy.second->SetDead( true );

                                // Add statistics
                                m_iEnemyKilled++;
                                m_iEnemyKilledTotal++;

                                m_score+=kEnemyScore;
                            }

                            if ( !m_bImmortal )
                            {
                                // decrease health of player ship by enemy damage amount
                                if ( m_pPlayer->DecreaseHealth( kEnemyDamage ) ) {
                                    KillPlayer();
                                }
                                else
                                {
                                    // Deploy Hit explosion
                                    Explosion( m_pPlayer->GetX(), m_pPlayer->GetY() );
                                }
                            }

                        }

                    }

                }

                /// UPDATE FOR NEXT RENDERING

                // 1. Find dead bullets
                tmpDeadLst.clear();
                for ( auto& p : m_umapBullets ) {
                    if ( p.second->IsDead() ) tmpDeadLst.push_back( p.second->GetID() );
                }

                // 2. Transfer to dead list
                for ( auto id : tmpDeadLst ) {
                    auto& pBullet = m_umapBullets[id];
                    m_umapDeadBullets.push_front( std::move(pBullet) );
                    m_umapBullets[id] = nullptr;
                    m_umapBullets.erase( id );
                    // remove also from updateables and renderables
                    auto& updateables = GetUpdateables();
                    auto& renderables = GetRenderables();
                    updateables[id] = nullptr;
                    renderables[id] = nullptr;
                    updateables.erase( id );
                    renderables.erase( id );
                }

                // 1. Find dead enemies
                tmpDeadLst.clear();
                for ( auto& p : m_umapEnemies ) {
                    if ( p.second->IsDead() ) tmpDeadLst.push_back( p.second->GetID() );
                }

                // 2. Transfer to dead list
                for ( auto id : tmpDeadLst ) {
                    auto& pEnemy = m_umapEnemies[id];
                    m_umapDeadEnemies.push_front( std::move(pEnemy) );
                    m_umapEnemies[id] = nullptr;
                    m_umapEnemies.erase( id );
                    // remove also from updateables and renderables
                    auto& updateables = GetUpdateables();
                    auto& renderables = GetRenderables();
                    updateables[id] = nullptr;
                    renderables[id] = nullptr;
                    updateables.erase( id );
                    renderables.erase( id );
                }

                if ( m_bLevelStarted )
                {
                    // Deploy enemies if there are under the needed amount
                    if ( m_umapEnemies.size() < static_cast<unsigned int>(m_level-m_iEnemyKilled) )
                    {
                        DeployEnemy();
                    }

                    if ( m_umapEnemies.size() == 0 )
                    {
                        // No enemies left, We must start new level
                        NextLevel();
                    }
                }


                // 1. Find dead explosions
                tmpDeadLst.clear();
                for ( auto& p : m_umapExplosions ) {
                    if ( p.second->IsDead() ) tmpDeadLst.push_back( p.second->GetID() );
                }

                // 2. Transfer to dead list
                for ( auto id : tmpDeadLst ) {
                    auto& pExplosion = m_umapExplosions[id];
                    m_umapDeadExplosions.push_front( std::move(pExplosion) );
                    m_umapExplosions[id] = nullptr;
                    m_umapExplosions.erase( id );
                    // remove also from updateables and renderables
                    auto& updateables = GetUpdateables();
                    auto& renderables = GetRenderables();
                    updateables[id] = nullptr;
                    renderables[id] = nullptr;
                    updateables.erase( id );
                    renderables.erase( id );
                }

                // Go to FADE_OUT it player is dead and no explosions are active
                if ( m_pPlayer->IsDead() && m_umapExplosions.empty() ) {
                    SetState( (int)STATE::FADE_OUT );
                    TimerFactory::Instance()->Get( RESOURCE::TIMER_SCENE_FADEOUT )->Reset();
                }

                int X = m_pPlayer->GetX();
                int Y = m_pPlayer->GetY();

                // Update ship position (and wrap on screen limits)

                if ( kPlayerXMargins == 0 )
                {
                    if ( X < 0 )
                        m_pPlayer->GetPosition()[0] = m_iScreenW-1;

                    if ( X > m_iScreenW-1 )
                        m_pPlayer->GetPosition()[0] = 0;
                }
                else
                {
                    if ( X < kPlayerXMargins )
                    {
                        m_pPlayer->GetPosition()[0] = kPlayerXMargins;
                        m_pPlayer->SetMovingX( false );
                        m_pPlayer->GetSpeed()[0] = 0;
                        m_pPlayer->GetAcceleration()[0] = 0;
                        m_bKeyLeft = false;
                    }
                    if ( X > m_iScreenW-kPlayerXMargins-1 )
                    {
                        m_pPlayer->GetPosition()[0] = m_iScreenW-kPlayerXMargins-1;
                        m_pPlayer->SetMovingX( false );
                        m_pPlayer->GetSpeed()[0] = 0;
                        m_pPlayer->GetAcceleration()[0] = 0;
                        m_bKeyRight = false;
                    }
                }

                if ( kPlayerYMargins == 0 )
                {
                    if ( Y < 0 )
                        m_pPlayer->GetPosition()[1] = m_iScreenH-1;
                    if ( Y > m_iScreenH-1 )
                        m_pPlayer->GetPosition()[1] = 0;
                }
                else
                {
                    if ( Y < kPlayerYMargins )
                    {
                        m_pPlayer->GetPosition()[1] = kPlayerYMargins;
                        m_pPlayer->SetMovingY( false );
                        m_pPlayer->GetSpeed()[1] = 0;
                        m_pPlayer->GetAcceleration()[1] = 0;
                        m_bKeyUp = false;
                    }
                    if ( Y > m_iScreenH-kPlayerYMargins-1 )
                    {
                        m_pPlayer->GetPosition()[1] = m_iScreenH-kPlayerYMargins-1;
                        m_pPlayer->SetMovingY( false );
                        m_pPlayer->GetSpeed()[1] = 0;
                        m_pPlayer->GetAcceleration()[1] = 0;
                        m_bKeyDown = false;
                    }
                }

                if ( !m_bKeyLeft && !m_bKeyRight )
                {
                    m_pPlayer->SetMovingX( false );
                }

                if ( !m_bKeyUp && !m_bKeyDown )
                {
                    m_pPlayer->SetMovingY( false );
                }

                if ( m_bKeyLeft ) {
                    m_pPlayer->GetAcceleration()[0] = -500.0f;
                    m_pPlayer->SetMovingX( true );
                }
                if ( m_bKeyRight ) {
                    m_pPlayer->GetAcceleration()[0] = 500.0f;
                    m_pPlayer->SetMovingX( true );
                }
                if ( m_bKeyUp ) {
                    m_pPlayer->GetAcceleration()[1] = -500.0f;
                    m_pPlayer->SetMovingY( true );
                }
                if ( m_bKeyDown ) {
                    m_pPlayer->GetAcceleration()[1] = 500.0f;
                    m_pPlayer->SetMovingY( true );
                }

                int m_iShipFrame = m_pPlayer->GetFrame();

                Uint32 ticks = SDL_GetTicks();

                if ( m_pPlayer->IsMovingX() )
                {
                    if ( m_pPlayer->GetAcceleration()[0] < 0.0f )
                    {
                        if ( m_iShipFrame > 2 ) {
                            m_iShipFrame = 1;
                            m_iTurnTick = SDL_GetTicks();
                        }
                        if ( ticks-m_iTurnTick > 700 ) {
                            if ( m_iShipFrame > 0 ) {
                                m_iShipFrame--;
                            }
                            m_iTurnTick = SDL_GetTicks();
                        }
                    }

                    if ( m_pPlayer->GetAcceleration()[0] > 0.0f )
                    {
                        if ( m_iShipFrame < 2 ) {
                            m_iShipFrame = 3;
                            m_iTurnTick = SDL_GetTicks();
                        }
                        if ( ticks-m_iTurnTick > 700 ) {
                            if ( m_iShipFrame < 4 ) {
                                m_iShipFrame++;
                            }
                            m_iTurnTick = SDL_GetTicks();
                        }
                    }

                }

                if ( !m_pPlayer->IsMovingX() )
                {
                    if ( m_iShipFrame > 2 ) {
                        if ( ticks-m_iTurnTick > 100 ) {
                            m_iShipFrame--;
                            m_iTurnTick = SDL_GetTicks();
                        }
                    }

                    if ( m_iShipFrame < 2 ) {
                        if ( ticks-m_iTurnTick > 100 ) {
                            m_iShipFrame++;
                            m_iTurnTick = SDL_GetTicks();
                        }
                    }
                }

                m_pPlayer->SetFrame( m_iShipFrame );
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
            // STATE::GAME
            //
            (int)STATE::GAME,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                auto& renderer = CSingleton<CRenderer>::Instance();
                auto& fontScore = FontFactory::Instance()->Get( RESOURCE::FONT_SCORE );
                auto& fontInfo = FontFactory::Instance()->Get( RESOURCE::FONT_SMALL );
                auto& textScore = TextFactory::Instance()->Get( RESOURCE::TEXT_SCORE );
                auto& textLevelStarting = TextFactory::Instance()->Get( RESOURCE::TEXT_LEVEL_STARTING );
                auto& textLevel = TextFactory::Instance()->Get( RESOURCE::TEXT_LEVEL );
                auto& textLevelInfo = TextFactory::Instance()->Get( RESOURCE::TEXT_LEVEL_INFO );

                // Render score
                if ( m_score != m_scoreOld || textScore == nullptr ) {
                    // create string
                    std::ostringstream ss;
                    ss << std::setw( 7 ) << std::setfill( '0' ) << m_score;
                    textScore->Create( fontScore, ss.str().c_str(), 255, 255, 255 );
                    m_scoreOld = m_score;
                }
                renderer->Render( textScore, m_iScreenW - (textScore->GetSurface()->w) - 10, 10 );

                // Render level info
                if ( m_level != m_levelOld || textLevel == nullptr ) {
                    // start timer
                    TimerFactory::Instance()->Get( RESOURCE::TIMER_LEVEL_START )->Reset();
                    // create string
                    std::ostringstream ss;
                    ss << "LEVEL " << std::setw( 3 ) << std::setfill( '0' ) << m_level;
                    textLevel->Create( fontScore, ss.str().c_str(), 255, 255, 255 );

                    std::ostringstream ss2;
                    if ( m_level > 1 && m_iEnemyHitTotal > 0 && m_iEnemyKilledTotal > 0 )
                        ss2 << "Enemies Killed: " << std::setw(0) << m_iEnemyKilledTotal << "  Bullets Fired: " << m_iPlayerFiredTotal << "  Accuracy: " << static_cast<int>(((float)m_iEnemyHitTotal/(float)m_iPlayerFiredTotal)*100) << "%";
                    else
                        ss2 << " ";
                    textLevelInfo->Create( fontInfo, ss2.str().c_str(), 220, 220, 220 );
                    m_levelOld = m_level;
                }
                TimerFactory::Instance()->Get( RESOURCE::TIMER_LEVEL_START )->Update();
                float fTime = TimerFactory::Instance()->Get( RESOURCE::TIMER_LEVEL_START )->GetPassedTimeReal();
                if ( fTime <= 5.0f )
                {
                    int iLevelStartY = Interpolate<float,float>( fTime, InterpolationSetFactory::Instance()->Get( RESOURCE::IS_LEVEL_START ) );
                    //auto& aSet = InterpolationSetFactory::Instance()->Get( m_nAlphaOverTimeID );
                    // Math::Interpolation::easeInCirc<float,float>( TimerFactory::Instance()->Get( RESOURCE::TIMER_LEVEL_START )->GetPassedTimeReal(), m_iScreenH, -m_iScreenH/2 - 24, 3.0f );
                    renderer->Render( textLevelStarting, m_iScreenW/2 - (textLevelStarting->GetSurface()->w/2) - 10, iLevelStartY  );
                    renderer->Render( textLevel, m_iScreenW/2 - (textLevel->GetSurface()->w/2) - 10, iLevelStartY + 38 );
                    renderer->Render( textLevelInfo, m_iScreenW/2 - (textLevelInfo->GetSurface()->w/2) - 10, iLevelStartY + 38 + 28 );
                }
                else
                {
                    if ( !m_bLevelStarted )
                        m_bLevelStarted = true;
                }

                // Draw shields indicator
                int nHealth = static_cast<int>((float)m_pPlayer->GetHealth() / 2.5f);
                int nMaxHealth = static_cast<int>((float)kPlayerHealth / 2.5f);

                m_RectShields.SetPosition( 32-1, m_iScreenH - 32-1 );
                m_RectShields.SetDimensions( nMaxHealth + 1, 7 );
                m_RectShields.SetFilled( false );
                m_RectShields.SetColor( 130, 200, 130, 128 );
                renderer->Render( &m_RectShields );

                m_RectShields.SetPosition( 32, m_iScreenH - 32 );
                m_RectShields.SetDimensions( nHealth, 6 );
                m_RectShields.SetFilled( true );
                m_RectShields.SetColor( 64, 255, 64, 128 );
                renderer->Render( &m_RectShields );
                m_RectShields.SetPosition( 32 + nHealth, m_iScreenH - 32 );
                m_RectShields.SetDimensions( nMaxHealth - nHealth, 6 );
                m_RectShields.SetColor( 0, 64, 0, 128 );
                renderer->Render( &m_RectShields );

                // Draw bullet-time energy indicator
                int nEnergy = static_cast<int>((float)m_iPlayerBulletTime / 15.0f);
                int nMaxEnergy = static_cast<int>((float)m_iPlayerBulletTimeMax / 15.0f);
                m_RectEnergy.SetPosition( 32-1, m_iScreenH - 32 + 9 );
                m_RectEnergy.SetDimensions( nMaxEnergy + 1, 7 );
                m_RectEnergy.SetFilled( false );
                m_RectEnergy.SetColor( 180, 180, 180, 128 );
                renderer->Render( &m_RectEnergy );

                m_RectEnergy.SetPosition( 32, m_iScreenH - 32 + 10 );
                m_RectEnergy.SetDimensions( nEnergy, 6 );
                m_RectEnergy.SetFilled( true );
                m_RectEnergy.SetColor( 200, 200, 200, 128 );
                renderer->Render( &m_RectEnergy );

                m_RectEnergy.SetPosition( 32 + nEnergy, m_iScreenH - 32 + 10 );
                m_RectEnergy.SetDimensions( nMaxEnergy - nEnergy, 6 );
                m_RectEnergy.SetColor( 100, 100, 100, 128 );
                renderer->Render( &m_RectEnergy );
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
                m_blankImg.SetColor(0,0,0,255-m_iScreenAlpha);
                renderer->Render( &m_blankImg );
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
                m_blankImg.SetColor(0,0,0,255-m_iScreenAlpha);
                renderer->Render( &m_blankImg );
            }
        },
        {
            //
            // STATE::END
            //
            (int)STATE::END,
            [&](SDL_Event& ev){
                DISCARD_UNUNSED_PARAMETER( ev );
                auto& renderer = CSingleton<CRenderer>::Instance();
                m_blankImg.SetColor(0,0,0,255-m_iScreenAlpha);
                renderer->Render( &m_blankImg );
            }
        }
    };
}
