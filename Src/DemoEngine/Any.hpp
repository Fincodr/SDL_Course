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

#ifndef ANY_HPP
#define ANY_HPP

//#include <sstream>
#include <ostream>
#include <stdexcept>
#include "AnyBase.hpp"

namespace DemoEngine {

    using std::runtime_error;
    using std::ostream;

    class CAny : private CAnyBase
    {

        typedef enum class
        {
            Void,
            Bool,
            Octet,
            Short,
            UnsignedShort,
            Long,
            UnsignedLong,
            LongLong,
            UnsignedLongLong,
            Float,
            Double,
            String,
        } TYPE;

        public:
            virtual ~CAny(){};

            CAny()
            {
                longLongValue = 0;
                type = (int)TYPE::Void;
            }

            CAny( bool value )
            {
                boolValue = value;
                type = (int)TYPE::Bool;
            }

            CAny( Uint8 value )
            {
                octetValue = value;
                type = (int)TYPE::Octet;
            }

            CAny( Sint16 value )
            {
                shortValue = value;
                type = (int)TYPE::Short;
            }

            CAny( Uint16 value )
            {
                unsignedShortValue = value;
                type = (int)TYPE::UnsignedShort;
            }

            CAny( Sint32 value )
            {
                longValue = value;
                type = (int)TYPE::Long;
            }

            CAny( Uint32 value )
            {
                unsignedLongValue = value;
                type = (int)TYPE::UnsignedLong;
            }

            CAny( Sint64 value )
            {
                longLongValue = value;
                type = (int)TYPE::LongLong;
            }

            CAny( Uint64 value )
            {
                unsignedLongValue = value;
                type = (int)TYPE::UnsignedLong;
            }

            CAny( float value )
            {
                floatValue = value;
                type = (int)TYPE::Float;
            }

            CAny( double value )
            {
                doubleValue = value;
                type = (int)TYPE::Double;
            }

            CAny( std::string value )
            {
                stringValue = value;
                type = (int)TYPE::String;
            }

            friend ostream &operator<<(ostream &stream, CAny ob);

            operator Uint8() const
            {
                if (getType() != (int)TYPE::Octet)
                    throw ( runtime_error( "Any::Uint8() -> Wrong typecast" ) );
                return octetValue;
            }

            operator Sint16() const
            {
                if (getType() != (int)TYPE::Short)
                    throw ( runtime_error( "Any::Sint16() -> Wrong typecast" ) );
                return shortValue;
            }

            operator Uint16() const
            {
                if (getType() != (int)TYPE::UnsignedShort)
                    throw ( runtime_error( "Any::Uint16() -> Wrong typecast" ) );
                return unsignedShortValue;
            }

            operator Sint32() const
            {
                if (getType() != (int)TYPE::Long)
                    throw ( runtime_error( "Any::Sint32() -> Wrong typecast" ) );
                return longValue;
            }

            operator Uint32() const
            {
                if (getType() != (int)TYPE::UnsignedLong)
                    throw ( runtime_error( "Any::Uint32() -> Wrong typecast" ) );
                return unsignedLongValue;
            }

            operator Sint64() const
            {
                if (getType() != (int)TYPE::LongLong)
                    throw ( runtime_error( "Any::Sint64() -> Wrong typecast" ) );
                return longLongValue;
            }

            operator Uint64() const
            {
                if (getType() != (int)TYPE::UnsignedLongLong)
                    throw ( runtime_error( "Any::Uint64() -> Wrong typecast" ) );
                return unsignedLongLongValue;
            }

            operator bool() const
            {
                if (getType() != (int)TYPE::Bool)
                    throw ( runtime_error( "Any::bool() -> Wrong typecast" ) );
                return boolValue;
            }

            operator float() const
            {
                if (getType() != (int)TYPE::Float)
                    throw ( runtime_error( "Any::float() -> Wrong typecast" ) );
                return floatValue;
            }

            operator double() const
            {
                if (getType() != (int)TYPE::Double)
                    throw ( runtime_error( "Any::double() -> Wrong typecast" ) );
                return doubleValue;
            }

            operator std::string() const
            {
                if (getType() != (int)TYPE::String)
                    throw ( runtime_error( "Any::string() -> Wrong typecast" ) );
                return stringValue;
            }

            int getType() const
            {
                return (type);
            }

        protected:
        private:
    };

}

#endif // ANY_HPP
