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

#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include <memory>
#include <SDL.h>
#include "Math.hpp"
#include "InterpolationSet.hpp"

namespace DemoEngine {

    template<typename T2 = double, typename T = double>
    inline CVector2<T> Interpolate( T2 time, const CVector2<T>& a, const CVector2<T>& b, T2 duration = (T2)1) {
        T x = Math::Interpolation::linearTween<T2,T>( time, a[0], b[0]-a[0], duration );
        T y = Math::Interpolation::linearTween<T2,T>( time, a[1], b[1]-a[1], duration );
        return ( CVector2<T>(x,y) );
    }

    template<typename F, typename T2 = double, typename T = double>
    inline CVector2<T> Interpolate( F f, T2 time, const CVector2<T>& a, const CVector2<T>& b, T2 duration = (T2)1) {
        T x = f( time, a[0], b[0]-a[0], duration );
        T y = f( time, a[1], b[1]-a[1], duration );
        return ( CVector2<T>(x,y) );
    }

    template<typename T2 = double, typename T = double>
    inline SDL_Color Interpolate( T2 time, const SDL_Color& a, const SDL_Color& b, T2 duration = (T2)1) {
        Uint8 r_new = Math::Interpolation::linearTween<T2,T>( time, a.r, b.r-a.r, duration );
        Uint8 g_new = Math::Interpolation::linearTween<T2,T>( time, a.g, b.g-a.g, duration );
        Uint8 b_new = Math::Interpolation::linearTween<T2,T>( time, a.b, b.b-a.b, duration );
        Uint8 a_new = Math::Interpolation::linearTween<T2,T>( time, a.unused, b.unused-a.unused, duration );
        return ( SDL_Color(r_new,g_new,b_new,a_new) );
    }

    template<typename F, typename T2 = double, typename T = double>
    inline SDL_Color Interpolate( F f, T2 time, const SDL_Color& a, const SDL_Color& b, T2 duration = (T2)1) {
        Uint8 r_new = f( time, a.r, b.r-a.r, duration );
        Uint8 g_new = f( time, a.g, b.g-a.g, duration );
        Uint8 b_new = f( time, a.b, b.b-a.b, duration );
        Uint8 a_new = f( time, a.unused, b.unused-a.unused, duration );
        return ( SDL_Color({r_new,g_new,b_new,a_new}) );
    }

    template<typename F, typename T2 = double, typename T = double>
    inline SDL_Color Interpolate( F f, T2 time, SDL_Color* a, SDL_Color* b, T2 duration = (T2)1) {
        if ( duration > 0 )
        {
            Uint8 r_new = f( time, a->r, b->r-a->r, duration );
            Uint8 g_new = f( time, a->g, b->g-a->g, duration );
            Uint8 b_new = f( time, a->b, b->b-a->b, duration );
            Uint8 a_new = f( time, a->unused, b->unused-a->unused, duration );
            return ( SDL_Color({r_new,g_new,b_new,a_new}) );
        }
        else
        {
            Uint8 r_new = a->r;
            Uint8 g_new = a->g;
            Uint8 b_new = a->b;
            Uint8 a_new = a->unused;
            return ( SDL_Color({r_new,g_new,b_new,a_new}) );
        }
    }

    template<typename F, typename T2 = double, typename T = double>
    inline SDL_Color Interpolate( F f, T2 time, CInterpolationSet<T2,T>& cset) {
        auto t = cset.Get(time);
        int s1 = std::get<0>(t);
        int s2 = std::get<1>(t);
        SDL_Color* a = std::get<2>(t);
        SDL_Color* b = std::get<3>(t);
        T2 duration = s2-s1;
        if ( duration > 0 )
        {
            Uint8 r_new = f( time-s1, a->r, b->r-a->r, duration );
            Uint8 g_new = f( time-s1, a->g, b->g-a->g, duration );
            Uint8 b_new = f( time-s1, a->b, b->b-a->b, duration );
            Uint8 a_new = f( time-s1, a->unused, b->unused-a->unused, duration );
            return ( SDL_Color({r_new,g_new,b_new,a_new}) );
        }
        else
        {
            Uint8 r_new = a->r;
            Uint8 g_new = a->g;
            Uint8 b_new = a->b;
            Uint8 a_new = a->unused;
            return ( SDL_Color({r_new,g_new,b_new,a_new}) );
        }
    }

    template<typename T2 = double, typename T = double>
    inline SDL_Color InterpolateSDLColor( T2 time, CInterpolationSetComplex<T2,T>& cset) {
        auto t = cset.Get(time);
        int s1 = std::get<0>(t);
        int s2 = std::get<1>(t);
        SDL_Color* a = std::get<2>(t);
        SDL_Color* b = std::get<3>(t);
        Math::Interpolation::func_t f = std::get<4>(t);
        T2 duration = s2-s1;
        if ( duration > 0 )
        {
            Uint8 r_new = f( time-s1, a->r, b->r-a->r, duration );
            Uint8 g_new = f( time-s1, a->g, b->g-a->g, duration );
            Uint8 b_new = f( time-s1, a->b, b->b-a->b, duration );
            Uint8 a_new = f( time-s1, a->unused, b->unused-a->unused, duration );
            return ( SDL_Color({r_new,g_new,b_new,a_new}) );
        }
        else
        {
            Uint8 r_new = a->r;
            Uint8 g_new = a->g;
            Uint8 b_new = a->b;
            Uint8 a_new = a->unused;
            return ( SDL_Color({r_new,g_new,b_new,a_new}) );
        }
    }

    template<typename T2 = double, typename T = double>
    inline SDL_Color InterpolateSDLColor( T2 time, std::unique_ptr<CInterpolationSetComplex<T2,T>>& cset) {
        auto t = cset->Get(time);
        int s1 = std::get<0>(t);
        int s2 = std::get<1>(t);
        SDL_Color* a = std::get<2>(t);
        SDL_Color* b = std::get<3>(t);
        Math::Interpolation::func_t f = std::get<4>(t);
        T2 duration = s2-s1;
        if ( duration > 0 )
        {
            Uint8 r_new = f( time-s1, a->r, b->r-a->r, duration );
            Uint8 g_new = f( time-s1, a->g, b->g-a->g, duration );
            Uint8 b_new = f( time-s1, a->b, b->b-a->b, duration );
            Uint8 a_new = f( time-s1, a->unused, b->unused-a->unused, duration );
            return ( SDL_Color({r_new,g_new,b_new,a_new}) );
        }
        else
        {
            Uint8 r_new = a->r;
            Uint8 g_new = a->g;
            Uint8 b_new = a->b;
            Uint8 a_new = a->unused;
            return ( SDL_Color({r_new,g_new,b_new,a_new}) );
        }
    }

    template<typename timeType, typename valueType>
    inline double Interpolate( timeType time, CInterpolationSet<timeType,valueType>& cset) {
        auto t = cset.Get(time);
        timeType s1 = std::get<0>(t);
        timeType s2 = std::get<1>(t);
        valueType a = std::get<2>(t);
        valueType b = std::get<3>(t);
        Math::Interpolation::func_t f = std::get<4>(t);
        timeType duration = s2-s1;
        if ( duration > 0 )
        {
            return ( f( time-s1, a, b-a, duration ) );
        }
        else
        {
            return ( b );
        }
    }

    template<typename timeType, typename valueType>
    inline double Interpolate( timeType time, std::unique_ptr<CInterpolationSet<timeType,valueType>>& cset) {
        auto t = cset->Get(time);
        timeType s1 = std::get<0>(t);
        timeType s2 = std::get<1>(t);
        valueType a = std::get<2>(t);
        valueType b = std::get<3>(t);
        Math::Interpolation::func_t f = std::get<4>(t);
        timeType duration = s2-s1;
        if ( duration > 0 )
        {
            return ( f( time-s1, a, b-a, duration ) );
        }
        else
        {
            return ( b );
        }
    }

    typedef CSingleton<CResourceFactory<int, CInterpolationSetComplex<float,SDL_Color>>> SDLColorInterpolationSetFactory;

}

#endif // INTERPOLATION_HPP
