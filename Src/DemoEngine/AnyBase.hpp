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

#ifndef ANYBASE_HPP
#define ANYBASE_HPP

#include <SDL.h>
#include <string>

namespace DemoEngine {

    using std::string;

    class CAnyBase
    {
    public:
        // Basic types can be in same union
        union
        {
            bool        boolValue;
            Uint8       octetValue;
            Sint16      shortValue;
            Uint16      unsignedShortValue;
            Sint32      longValue;
            Uint32      unsignedLongValue;
            Sint64      longLongValue;
            Uint64      unsignedLongLongValue;
            float       floatValue;
            double      doubleValue;
        };
        // Complex types must be outside of union
        string stringValue;
        int type = 0;
        CAnyBase() : stringValue("") {};
        virtual ~CAnyBase() {};
    };
}

#endif // ANYBASE_HPP
