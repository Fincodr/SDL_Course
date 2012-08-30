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

#ifndef ACCELERATIONAL_HPP
#define ACCELERATIONAL_HPP

#include "Vector2.hpp"

namespace DemoEngine
{
    template<typename T>
	class CAccelerational
	{
	public:
	    friend class CCollisionDetector;

		CAccelerational() : m_vSpeed(), m_vAcceleration() {}
		virtual ~CAccelerational(){}

        bool IsMoving() { return m_bMovingX && m_bMovingY; }
        bool IsMovingX() { return m_bMovingX; }
        bool IsMovingY() { return m_bMovingY; }
        void SetMoving( bool bMoving ) { m_bMovingX = bMoving; m_bMovingY = bMoving; }
        void SetMovingX( bool bMoving ) { m_bMovingX = bMoving; }
        void SetMovingY( bool bMoving ) { m_bMovingY = bMoving; }

        CVector2<T>& GetSpeed() { return m_vSpeed; }
        void SetSpeed( float fSpeedX, float fSpeedY ) { m_vSpeed = { fSpeedX, fSpeedY }; }

        void SetAcceleration( float fAccX, float fAccY ) { m_vAcceleration = {fAccX, fAccY}; }
        CVector2<T>& GetAcceleration() { return m_vAcceleration; }

    private:
        CVector2<T> m_vSpeed;
        CVector2<T> m_vAcceleration;
        bool m_bMovingX = false;
        bool m_bMovingY = false;
  };
}

#endif // ACCELERATIONAL_HPP
