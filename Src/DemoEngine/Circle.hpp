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

#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "Positional.hpp"
#include "TwoDimensional.hpp"
#include "Fillable.hpp"
#include "Colored.hpp"

#include <iostream>

namespace DemoEngine
{
    using std::cout;
    using std::endl;

	class CCircle : public CPositional<int>, public CFillable, public CColored {
		public:
		    friend class CCollisionDetector;
			CCircle() { }
			CCircle( int x, int y, int r ) { SetPosition(x,y); SetRadius(r); }
			virtual ~CCircle() { }
			void SetRadius( int r) { m_iRadius = r; }
			inline int GetRadius() { return m_iRadius; }
		protected:
			int m_iRadius = 0;
	};
}

#endif // CIRCLE_HPP
