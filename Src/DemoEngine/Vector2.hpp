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

#ifndef CVECTOR2_HPP
#define CVECTOR2_HPP

#include <cmath>

namespace DemoEngine
{
	template<class T>
	class CVector2
	{
	public:
	    friend class CCollisionDetector;
		CVector2() {}
		CVector2( T x, T y) 							{ m_values[0] = x; m_values[1] = y; }
		CVector2( const CVector2& v ) 					{ m_values[0] = v.m_values[0]; m_values[1] = v.m_values[1]; }
		virtual ~CVector2() 							{}
		CVector2& operator=( const CVector2& v )
		{
		    if ( this != &v )
                m_values[0] = v.m_values[0]; m_values[1] = v.m_values[1];
            return *this;
        }
		CVector2 operator/( T value ) const 			{ return CVector2( m_values[0]/value, m_values[1]/value ); }
		CVector2 operator-( const CVector2& v) const 	{ return CVector2( m_values[0] - v.m_values[0], m_values[1] - v.m_values[1] ); }
		CVector2 operator+( const CVector2& v) const 	{ return CVector2( m_values[0] + v.m_values[0], m_values[1] + v.m_values[1] ); }
		CVector2 operator*( T value ) const 			{ return CVector2( m_values[0] * value, m_values[1] * value ); }
		friend CVector2 operator*( T value, const CVector2& v )
			{ return CVector2( v.m_values[0] * value, v.m_values[1] * value); }
		void operator-=( const CVector2& v) 			{ m_values[0] -= v.m_values[0]; m_values[1] -= v.m_values[1]; }
		void operator+=( const CVector2& v) 			{ m_values[0] += v.m_values[0]; m_values[1] += v.m_values[1]; }
		void operator*=( T v ) 							{ m_values[0] *= v; m_values[1] *= v; }
		void operator/=( T v ) 							{ m_values[0] /= v; m_values[1] /= v; }
		T& operator[](int index ) 						{ return m_values[index]; }
		T& At(int index ) 								{ return m_values[index]; }
		const T& operator[](int index) const 			{ return m_values[index]; }
		const T& At(int index )  const 				{ return m_values[index]; }
		bool operator==( const CVector2& v ) const 	{ return ( (m_values[0] == v.m_values[0]) && (m_values[1] == v.m_values[1]) ); }
		bool operator!=( const CVector2& v ) const 	{ return ( (m_values[0] != v.m_values[0]) || (m_values[1] != v.m_values[1]) ); }
		T LengthSqr() const 							{ return Dot(*this); }
		T Length() const 								{ return sqrtf(Dot(*this)); }
		void Normalize() 								{ (*this)/=Length(); }
		T Dot( const CVector2& v ) const 				{ return ( m_values[0] * v.m_values[0] + m_values[1] * v.m_values[1] ); }
	private:
		T m_values[2];
    };

	// typedefs
	typedef CVector2<int> CVector2i;
	typedef CVector2<float> CVector2f;
}

#endif // CVECTOR2_HPP
