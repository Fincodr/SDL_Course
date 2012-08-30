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

#ifndef POSITIONAL_HPP
#define POSITIONAL_HPP

#include "Vector2.hpp"

namespace DemoEngine
{
    template<typename T>
	class CPositional
	{
	public:
	    friend class CCollisionDetector;

		CPositional() : m_vPosition() {}
		virtual ~CPositional(){}

		CPositional( int x, int y )  { m_vPosition[0] = x; m_vPosition[1] = y; }
		CPositional( const CVector2<T>& v )  { m_vPosition = v; }

		inline void SetPosition( int x, int y ) { m_vPosition[0] = x; m_vPosition[1] = y; }
		inline void SetPosition( const CVector2<T>& v ) { m_vPosition = v; }
		inline CVector2<T>& GetPosition() { return m_vPosition; }
		inline const CVector2<T> & GetPosition() const { return m_vPosition; }

		inline void SetX( int x ) { m_vPosition[0] = x; }
		inline void SetY( int y ) { m_vPosition[1] = y; }

		inline int GetX() { return m_vPosition[0]; }
		inline int GetY() { return m_vPosition[1]; }

		void Move( int x, int y ) { m_vPosition[0] += x; m_vPosition[1]+=y; }
		void Move( const CVector2<T>& v ) { m_vPosition += v; }

		CVector2<T> m_vPosition;
  };
}

#endif // POSITIONAL_HPP
