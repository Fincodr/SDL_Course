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

#ifndef HIGHSCORELIST_HPP
#define HIGHSCORELIST_HPP

#include <string>
#include <algorithm>
#include <set>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <stdexcept>
#include "DemoEngine/TextUtils.hpp"

using namespace DemoEngine;

using std::cout;
using std::endl;
using std::multiset;
using std::string;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::ostringstream;
using std::istream_iterator;
using std::ostream_iterator;
using std::runtime_error;

struct CHighscoreItem
{
    string name;
    int score;
    int level;
    int accuracy;
    bool operator<(const CHighscoreItem& rhs) const {
        return ( score > rhs.score ); // for reversed sort (highest score top)
    }
};


class CHighscoreList
{
public:

    typedef multiset<CHighscoreItem> highscores_t;
    const string kFilename = "saves.hs";

    CHighscoreList() //: m_mmapHighscores()
    {
        // When class is initialized, load current scores
        // automatically from file if exists or create
        // new file.
        Load();
    }

    void Load() throw( runtime_error )
    {

        // open file
        ifstream file(kFilename);

        // check that file is open
        if ( file.is_open() )
        {
            // create istream iterator that reads chars from file
            istream_iterator<TextUtils::LineReader> reader(file);

            // create end-of-stream iterator
            istream_iterator<TextUtils::LineReader> readerEOF;

            // create ostream iterator that outputs char contents with new lines
            ostream_iterator<string> output(cout, "\n");

            // temporal vectors for processing the input file
            std::vector<string> input;

            // Copy contents to a vector
            copy( reader, readerEOF, back_inserter(input) );

            for_each( input.begin(), input.end(), [&](string& text){
                istringstream s(text);
                CHighscoreItem item;
                s >> item.score;
                s >> item.name;
                if ( s.good() ) {
                    s >> item.level;
                    s >> item.accuracy;
                }
                else
                {
                    item.level = 0;
                    item.accuracy = 0;
                }
                m_mmapHighscores.insert(item);
            } );
        }
        else
        {
            // File not found, create demo highscores in memory.
            m_mmapHighscores = {
                { "TWO", 900, 5, 50 },
                { "ME", 1000, 10, 75 },
                { "WICKED", 5000, 15, 100 }
            };
        }

    }

    void AddEntry( string name, int score, int level, int accuracy )
    {
        m_mmapHighscores.insert({name,score,level,accuracy});
        Save();
    }

    highscores_t& GetList() {
        return( m_mmapHighscores );
    }

    void Debug()
    {
        for ( auto& item : m_mmapHighscores )
        {
            cout << item.name << ", score: " << item.score << ", level: " << item.level << ", accuracy: " << item.accuracy << "%" << endl;
        }
    }

protected:
    void Save()
    {
        ofstream file(kFilename);

        if ( file.is_open() )
        {
            for ( auto& item : m_mmapHighscores )
            {
                file << item.score << " " << item.name << " " << item.level << " " << item.accuracy << endl;
            }
            file.flush();
            file.close();
        }
        else
        {
            throw runtime_error( "Couldn't save highscore file!" );
        }
    }


private:
    highscores_t m_mmapHighscores;
};

#endif // HIGHSCORELIST_HPP
