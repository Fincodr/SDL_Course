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

#include "Any.hpp"

namespace DemoEngine {

    ostream& operator<<(ostream &stream, CAny ob)
    {
        switch ( ob.getType() )
        {
        case (int)CAny::TYPE::Bool:
            stream << ob.boolValue;
            break;
        case (int)CAny::TYPE::Octet:
            stream << ob.octetValue;
            break;
        case (int)CAny::TYPE::Short:
            stream << ob.shortValue;
            break;
        case (int)CAny::TYPE::UnsignedShort:
            stream << ob.unsignedShortValue;
            break;
        case (int)CAny::TYPE::Long:
            stream << ob.longValue;
            break;
        case (int)CAny::TYPE::UnsignedLong:
            stream << ob.unsignedLongValue;
            break;
        case (int)CAny::TYPE::LongLong:
            stream << ob.longLongValue;
            break;
        case (int)CAny::TYPE::UnsignedLongLong:
            stream << ob.unsignedLongLongValue;
            break;
        case (int)CAny::TYPE::Float:
            stream << ob.floatValue;
            break;
        case (int)CAny::TYPE::Double:
            stream << ob.doubleValue;
            break;
        default:
            break;
        }
        return stream;
    }

}
