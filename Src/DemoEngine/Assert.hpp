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

/***************************************
 ** Assert for DemoEngine
 **
 ** Created: 2012-08-03
 **
 ** Description: Provides a simple TEST_ASSERT macro for testing condition
 **
 ** Usage:
 **
 **   int var = 1;
 **   TEST_ASSERT( var == 0, "Var must be zero!" );
 **
 **   If ASSERT_THROW_EXCEPTION is defined: Throws AssertFailedException with the provided message
 **   or if undefined will write the message to standard error output and terminate the program
 **   with exit(1).
 ***************************************/

#ifndef ASSERT_HPP
#define ASSERT_HPP

#include <string>
#include <sstream>
#include <iostream>

#define ASSERT_THROW_EXCEPTION

#ifdef ASSERT_THROW_EXCEPTION

    #include <exception>

    namespace DemoEngine {

        class AssertFailedException : public std::exception {

        public:
            AssertFailedException( const char* fileName, unsigned int lineNumber, const char* msg) {
                std::ostringstream s;
                s << "Assertion failed with message: '" << msg << "' in file: " << fileName << " #" << lineNumber;
                m_strWhat = s.str();
            }
            virtual const char* what() const throw() { return m_strWhat.c_str(); }
            virtual ~AssertFailedException() throw() {}
        private:
            std::string m_strWhat = { "" };

        };

    }

#endif // ASSERT_THROW_EXCEPTION

inline void AssertErrorHandler( const char* fileName, unsigned int lineNumber, const char* msg ) {
    #ifdef ASSERT_THROW_EXCEPTION
    throw DemoEngine::AssertFailedException( fileName, lineNumber, msg );
    #else
    std::cerr << msg << " in file " << fileName << " #" << lineNumber << std::endl << std::flush;
    exit(1);
    #endif
}

#undef ASSERT
#define ASSERT( condition, msg )    \
    if(!(condition)) {              \
        std::ostringstream s;       \
        s << msg;                   \
        AssertErrorHandler(         \
            __FILE__, __LINE__,     \
            s.str().c_str() );      \
    }

#ifdef DEBUG
#define TEST_ASSERT(condition,msg) ASSERT(condition,msg)
#else
#define TEST_ASSERT(condition,msg) // do nothing then ;-)
#endif

#endif // ASSERT_HPP
