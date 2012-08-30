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

#include "ShooterGame.hpp"

using namespace DemoEngine;

int main( int argc, char **argv )
{
    DISCARD_UNUNSED_PARAMETER( argc );
    DISCARD_UNUNSED_PARAMETER( argv );

    try {

        // Set properties (these are also the edefaults)
        /*
        CSingleton<CProperties>::Instance()->Section("Video") =
        {
            {
                "FullScreen",
                (bool)false
            },
            {
                "Width",
                (Uint32)640,
            },
            {
                "Height",
                (Uint32)480
            },
            {
                "Bits",
                (Uint32)32
            }
        };
        */

        CShooterGame app;
        while ( app.Execute() );

    }
    catch (std::exception &ex ) {

        cout << "Runtime Error Exception: " << ex.what() << endl;

    }

	return( 0 );
}
