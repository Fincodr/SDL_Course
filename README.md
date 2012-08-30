# sdl-demo-engine

Demo engine using C++ and SDL (for SDL Programming course provided by North Karelia University of Applied Sciences)

## SOURCE CODE

1. All assets are in Build/Assets folder (some assets have been converted to better formats like png and ogg)
2. Source code in Src folder

## BUILDING EXECUTABLES

Load the Code::Blocks project and compile with std=c++11 settings OR create new project in XCode using Cocoa app template, set SDL include paths etc. C++ 11 compliant compiler (Clang) used in XCode is not fully standard compilance or perfect, so there is some classes that need some tweaking to get the clang to compile them. All stuff is C++11 standard compilant but Clang crashes on some easy stuff like remove_if + lambda instructions, sorry about that.