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

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <memory>

namespace DemoEngine {

    using std::unique_ptr;
    using std::make_shared;

    template<typename T>
    class CSingleton
    {
        public:
            virtual ~CSingleton() {}
            // Delete ctor, copy and assign operators to force Singleton pattern usage
            CSingleton()=delete;
            CSingleton(const CSingleton& other)=delete;
            CSingleton& operator=(const CSingleton& other)=delete;
            static unique_ptr<T>& Instance()
            {
                static auto m_pClass = unique_ptr<T>(new T);
                return m_pClass;
            }
        protected:
        private:
    };

}


#endif // SINGLETON_HPP
