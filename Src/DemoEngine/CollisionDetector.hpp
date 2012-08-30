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

#ifndef COLLISIONDETECTOR_HPP
#define COLLISIONDETECTOR_HPP

#include <memory>
#include <iostream>
#include "Pixel.hpp"
#include "Image.hpp"
#include "Circle.hpp"
#include "Rectangle.hpp"
#include "Vector2.hpp"

namespace DemoEngine
{
    using std::shared_ptr;
    using std::cout;
    using std::endl;

    /// Class for detecting collision between primitives and images.
    class CCollisionDetector : public CCircle, public CRectangle
    {
    public:
        /** \brief Checks whether two images collide.
        *
        * \param imgA const CImage& - First image.
        * \param posA const CVector2i& - Position of top left coordinate of imgA.
        * \param imgB const CImage& - Second image.
        * \param posB const CVector2i& - Position of top left coordinate of imgB.
        * \return bool - true if images collide, false otherwise.
        *
        */
        static bool Collides( const shared_ptr<CImage>& imgA, const CVector2i& posA, const shared_ptr<CImage>& imgB, const CVector2i& posB, shared_ptr<CRectangle>& hitRect );

        /** \brief Checks whether two circles collide.
        *
        * \param c1 const CCircle& - First circle.
        * \param c2 const CCircle& - Second circle.
        * \return bool - true if circles collide, false otherwise.
        *
        */
        static bool Collides( const shared_ptr<CCircle>& c1, const shared_ptr<CCircle>& c2 );

        /** \brief Checks whether two rectangles collide.
        *
        * \param r1 const CRectangle& - First rectangle.
        * \param r2 const CRectangle& - Second rectangle.
        * \return bool - true if rectangles collide, false otherwise.
        *
        */
        static bool Collides( const CRectangle& r1, const CRectangle& r2 );
        static bool Collides( const shared_ptr<CRectangle>& r1, const shared_ptr<CRectangle>& r2 );

        /** \brief Checks whether point is inside a circle.
        *
        * \param c const CCircle&
        * \param pos const CVector2i&
        * \return bool - true if pos is inside circle c, false otherwise.
        *
        */
        static bool Collides( const shared_ptr<CCircle>& c, const CVector2i& pos );

        /** \brief Checks whether point is inside a rectangle.
        *
        * \param r const CRectangle&
        * \param pos const CVector2i&
        * \return bool - true if pos is inside rectangle r, false otherwise.
        *
        */
        static bool Collides( const shared_ptr<CRectangle>&  r, const CVector2i& pos );

    protected:
        /** \brief Gets one pixel from specified surface (8/16/24/32 bit depth supported)
         *
         * \param surface SDL_Surface*
         * \param x int
         * \param y int
         * \return Uint32
         *
         */
    };
}

#endif // COLLISIONDETECTOR_HPP
