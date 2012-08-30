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

#ifndef PROPERTIES_HPP
#define PROPERTIES_HPP

#include "Any.hpp"
#include <unordered_map>
#include <string>

namespace DemoEngine {

    using std::unordered_map;
    using std::string;

    typedef unordered_map< string, CAny > PropertiesItems_t;
    typedef unordered_map< string, PropertiesItems_t > Properties_t;

    class CProperties
    {
        public:
            virtual ~CProperties() {}
            CProperties() {}

            Properties_t& Properties() {
                return m_Properties;
            }

            PropertiesItems_t& Section( string section ) {
                auto& pSection = m_Properties[section];
                return pSection;
            }

            bool Exists( string section, string property ) {
                auto it = m_Properties.find(section);
                if ( it != m_Properties.end() ) {
                    // found section
                    auto props = m_Properties[section];
                    auto itp = props.find(property);
                    if ( itp != props.end() ) {
                        // found property
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    // section missing
                    return false;
                }
            }

            CAny& Property( string section, string property ) {
                return ( m_Properties[section][property]);
                /*
                auto& pSection = m_Properties[section];
                auto& pProperty = pSection[property];
                return ( pProperty );
                */
            }

            CAny& Property( string section, string property, CAny defvalue ) {
                if ( Exists( section, property  ) )
                    return ( Property( section, property ) );
                m_Properties[section][property] = defvalue;
                return ( m_Properties[section][property] );
            }

        protected:
        private:
            Properties_t m_Properties = {};
    };

}

#endif // PROPERTIES_HPP
