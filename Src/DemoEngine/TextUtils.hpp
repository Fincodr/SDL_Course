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

#ifndef TEXTUTILS_HPP
#define TEXTUTILS_HPP

namespace DemoEngine {

    namespace TextUtils {

        using std::istream;
        using std::getline;
        using std::string;
        using std::cout;
        using std::endl;

        /** \brief Reads one line at the time from input stream
         */
        class LineReader {
            public:
                LineReader() : data("") {};
                virtual ~LineReader() {};
                inline friend std::istream &operator>>(std::istream &is, LineReader &l) {
                    std::getline(is, l.data);
                    return is;
                }
            operator std::string() const { return data; }
            protected:
            private:
                std::string data;
        };

        /** \brief Trims a std::string from both ends
         *
         * \param str std::string&
         * \return std::string
         *
         */
        inline std::string Trim(std::string& str)
        {
            str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
            str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
            return str;
        }
    }

}

#endif // TEXTUTILS_HPP
