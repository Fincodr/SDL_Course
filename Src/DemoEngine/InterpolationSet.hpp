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

#ifndef INTERPOLATIONSET_HPP
#define INTERPOLATIONSET_HPP

#include <unordered_map>
#include <list>
#include <tuple>
#include <functional>
#include "Math.hpp"

namespace DemoEngine {

    // Generic interpolation set for complex objects
    template<typename T, class T2>
    class CInterpolationSetComplex {
    public:

        CInterpolationSetComplex() : m_lstValues(), m_minValue(), m_maxValue() {
            m_lstValues.clear();
        }

        virtual ~CInterpolationSetComplex() {};

        virtual void AddValue( T point, T2 value, Math::Interpolation::func_t f = nullptr ) {
            m_lstValues.push_back( std::make_tuple( point, value, f ) );
            // must be sorted after inserting elements
            m_lstValues.sort( [](const std::tuple<T,T2,Math::Interpolation::func_t> &left, const std::tuple<T,T2,Math::Interpolation::func_t> &right){
                              return std::get<0>(left) < std::get<0>(right);
            } );
            // take first and last values as min and max
            m_minValue = std::get<1>(m_lstValues.front());
            m_maxValue = std::get<1>(m_lstValues.back());
            if ( point < m_minPoint ) m_minPoint = point;
            if ( point > m_maxPoint ) m_maxPoint = point;
        }

        virtual std::tuple<T,T,T2*,T2*,Math::Interpolation::func_t> Get( T point ) {
            // Figure out the corrent
            T s1 = m_minPoint;
            T s2 = m_maxPoint;
            T2& c1 = m_minValue;
            T2& c2 = m_maxValue;
            Math::Interpolation::func_t f = nullptr;
            for ( auto& p : m_lstValues )
            {
                if ( std::get<0>(p) > point ) {
                    // returned point is higher than where we are now
                    // so this must be our end point
                    // and we should already have the start point
                    // in memory
                    c2 = std::get<1>(p);
                    s2 = std::get<0>(p);
                    f = std::get<2>(p);
                    break;
                }
                else
                {
                    c1 = std::get<1>(p);
                    s1 = std::get<0>(p);
                }
            }
            if ( f == nullptr )
                f = Math::Interpolation::linearTween<double,Uint8>;
            return ( std::make_tuple(s1, s2, &c1, &c2, f) );
        }
    private:
        std::list<std::tuple<T,T2,Math::Interpolation::func_t>> m_lstValues;
        T m_minPoint = 0;
        T m_maxPoint = 0;
        T2 m_minValue;
        T2 m_maxValue;
    };

    // Generic interpolation set for simple objects (float, double etc..)
    template<typename timeType, typename valueType>
    class CInterpolationSet {
    public:

        CInterpolationSet() : m_lstValues(), m_minValue(), m_maxValue() {
            m_lstValues.clear();
        }

        virtual ~CInterpolationSet() {
            m_lstValues.clear();
        }

        virtual void AddValue( timeType point, valueType value, Math::Interpolation::func_t f = nullptr ) {
            m_lstValues.push_back( std::make_tuple( point, value, f ) );
            // must be sorted after inserting elements
            m_lstValues.sort( [](const std::tuple<timeType,valueType,Math::Interpolation::func_t> &left, const std::tuple<timeType,valueType,Math::Interpolation::func_t> &right){
                              return std::get<0>(left) < std::get<0>(right);
            } );
            // take first and last values as min and max
            m_minValue = std::get<1>(m_lstValues.front());
            m_maxValue = std::get<1>(m_lstValues.back());
            if ( point < m_minPoint ) m_minPoint = point;
            if ( point > m_maxPoint ) m_maxPoint = point;
        }

        virtual std::tuple<timeType,timeType,valueType,valueType,Math::Interpolation::func_t> Get( timeType point ) {
            // Figure out the corrent
            timeType s1 = m_minPoint;
            timeType s2 = m_maxPoint;
            valueType c1 = m_minValue;
            valueType c2 = m_maxValue;
            Math::Interpolation::func_t f = nullptr;
            for ( auto& p : m_lstValues )
            {
                if ( std::get<0>(p) > point ) {
                    // returned point is higher than where we are now
                    // so this must be our end point
                    // and we should already have the start point
                    // in memory
                    c2 = std::get<1>(p);
                    s2 = std::get<0>(p);
                    f = std::get<2>(p);
                    break;
                }
                else
                {
                    c1 = std::get<1>(p);
                    s1 = std::get<0>(p);
                }
            }
            if ( f == nullptr )
                f = Math::Interpolation::linearTween<timeType,valueType>;
            return ( std::make_tuple(s1, s2, c1, c2, f) );
        }
    private:
        std::list<std::tuple<timeType,valueType,Math::Interpolation::func_t>> m_lstValues;
        timeType m_minPoint = 0;
        timeType m_maxPoint = 0;
        valueType m_minValue;
        valueType m_maxValue;
    };

    typedef CSingleton<CResourceFactory<int, CInterpolationSet<float,float>>> InterpolationSetFactory;
}

#endif // INTERPOLATIONSET_HPP
