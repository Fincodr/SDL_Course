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

#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Positional.hpp"
#include "Accelerational.hpp"
#include "IRenderable.hpp"
#include "IUpdateable.hpp"
#include "Rectangle.hpp"
#include "Vector2.hpp"
#include "UniqueID.hpp"

namespace DemoEngine {

    class CGameObjectFloat : public CPositional<float>, public CAccelerational<float>, public IRenderable, public IUpdateable
    {
        public:
            CGameObjectFloat() : m_BoundingBox(), m_Bounds() { m_ObjectID = CSingleton<CUniqueID>::Instance()->getID(); };

            // Bounding box functions
            virtual void UpdateBoundingBox() {};
            inline void SetBoundingBoxPos( int x, int y ) { m_BoundingBox = m_Bounds; m_BoundingBox.SetPosition( x + m_Bounds.GetX(), y + m_Bounds.GetY() ); }
            inline CRectangle& GetBoundingBox() { return m_BoundingBox; }
            inline CRectangle& GetBounds() { return m_Bounds; }

            inline Uint32 GetID() { return m_ObjectID; }
            inline void SetDead( bool bDead ) { m_bDead = bDead; }
            inline bool IsDead() { return m_bDead; }

            inline Uint32 GetMaxHealth() { return m_iMaxHealth; }
            inline void SetHealth( Uint32 health ) { m_iHealth = health; m_iMaxHealth = health; }
            inline Uint32 GetHealth() { return m_iHealth; }
            inline bool DecreaseHealth( Uint32 health ) {
                if ( (int)(m_iHealth - health) <= 0 )
                {
                    m_iHealth = 0;
                    return true;
                }
                else
                    m_iHealth -= health;
                return false;
            }

            void SetOwner( int nOwner ) { m_nOwnerID = nOwner; }
            Uint32 GetOwner() { return m_nOwnerID; }

        protected:
        private:
            Uint32 m_nOwnerID = 0;
            CRectangle m_BoundingBox;
            CRectangle m_Bounds;
            Uint32 m_ObjectID = 0;
            Uint32 m_iHealth = 1;
            Uint32 m_iMaxHealth = 1;
            bool m_bDead = false;
    };

    class CGameObjectInt : public CPositional<int>, public CAccelerational<float>, public IRenderable, public IUpdateable
    {
        public:
            CGameObjectInt() : m_BoundingBox(), m_Bounds(), m_ObjectID(CSingleton<CUniqueID>::Instance()->getID()) {};

            // Bounding box functions
            virtual void UpdateBoundingBox() {};
            inline void SetBoundingBoxPos( int x, int y ) { m_BoundingBox = m_Bounds; m_BoundingBox.SetPosition( x + m_Bounds.GetX(), y + m_Bounds.GetY() ); }
            inline CRectangle& GetBoundingBox() { return m_BoundingBox; }
            inline CRectangle& GetBounds() { return m_Bounds; }

            inline Uint32 GetID() { return m_ObjectID; }
            inline void SetDead( bool bDead ) { m_bDead = bDead; }
            inline bool IsDead() { return m_bDead; }

            inline Uint32 GetMaxHealth() { return m_iMaxHealth; }
            inline void SetHealth( Uint32 health ) { m_iHealth = health; m_iMaxHealth = health; }
            inline Uint32 GetHealth() { return m_iHealth; }
            inline bool DecreaseHealth( Uint32 health ) {
                if ( (int)(m_iHealth - health) <= 0 )
                {
                    m_iHealth = 0;
                    return true;
                }
                else
                    m_iHealth -= health;
                return false;
            }

            void SetOwner( int nOwner ) { m_nOwnerID = nOwner; }
            Uint32 GetOwner() { return m_nOwnerID; }

        protected:
        private:
            Uint32 m_nOwnerID = 0;
            CRectangle m_BoundingBox;
            CRectangle m_Bounds;
            Uint32 m_ObjectID = 0;
            Uint32 m_iHealth = 1;
            Uint32 m_iMaxHealth = 1;
            bool m_bDead = false;
    };

}

#endif // GAMEOBJECT_HPP
