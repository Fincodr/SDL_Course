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

#include "Animation.hpp"

namespace DemoEngine {

    CAnimation::CAnimation() : m_hSequences(), m_hSequence()
    {
        SetFPS( 1 );
    }

    CAnimation::~CAnimation()
    {
        //dtor
    }

    void CAnimation::SetAnimation( string anim_name, bool bLoop )
    {
        if ( m_hSequences.find(anim_name) != end(m_hSequences) )
        {
            m_hSequence = m_hSequences[anim_name];
        }
        else
        {
            // Since element was not found we should try to
            // take the first sequence.
            auto elem = begin(m_hSequences);
            m_hSequence = elem->second;
        }
        m_iFrame = 0;
        m_bLooping = bLoop;
    }

    void CAnimation::Update( float fPassedTime )
    {
        m_fCumTime += fPassedTime;

        if ( m_fCumTime > m_fFrameTime )
        {
            int frames = static_cast<int>(m_fCumTime / m_fFrameTime);
            m_iFrame += frames;
            size_t seq_size = m_hSequence.size();

            // If frames tend to jump
            if ( m_iFrame >= seq_size )
            {
                m_iFrame = m_bLooping ? m_iFrame - seq_size : seq_size - 1;
            }
            m_fCumTime -= m_fFrameTime * frames;
        }

        ApplyAnimation();
    }

    void CAnimation::LoadAnimation( const char * szFilename ) throw ( runtime_error )
    {
        #ifdef DEBUG
        cout << "CAnimation::Load( \"" << szFilename << "\" )" << endl;
        #endif
        // The idea is to create hashtable which would
        // store the sequences with the sequence name as
        // key and frames as value.
        //
        // Best way of doing hashtable in C++11 is unordered containers.
        //
        // http://bytes.com/topic/c/answers/436124-copy-istream_iterator-question
        // http://stackoverflow.com/questions/1567082/how-do-i-iterate-over-cin-line-by-line-in-c

        // open file
        ifstream file(szFilename);

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
            std::vector<SDL_Rect> frames;

            // Copy contents to a vector
            // Note: Since we want to preserve the comments in the sequences
            //       we can't use copy_if to skip all '#' comment lines.
            copy( reader, readerEOF, back_inserter(input) );

            // Find out where the blocks start and end.
            auto eofPos = end(input);
            auto framesPos = find( begin(input), eofPos, kFramesText );
            auto sequencesPos = find( framesPos+1, eofPos, kSequencesText );

            // Check that we have both blocks
            if ( framesPos != eofPos && sequencesPos != eofPos )
            {
                // Construct frames using for_each (skip comment lines at the same time)
                for_each( framesPos+1, sequencesPos, [&](string& elem){
                    if ( elem[0] != '#' ) {
                        istringstream s(elem);
                        SDL_Rect r;
                        s >> r.x;
                        s >> r.y;
                        s >> r.w;
                        s >> r.h;
                        frames.push_back(r);
                    }
                } );

                // Construct sequences using for_each
                // Take sequence name from the precending comment line (if exist)
                string strSequenceName = "";
                for_each( sequencesPos+1, eofPos, [&](string& elem){
                    if ( elem[0] == '#' ) {
                        // Any comment line will set the next sequence name
                        string temp = elem.substr(1);
                        strSequenceName = TextUtils::Trim(temp);
                    }
                    else
                    {
                        // store sequences into m_Sequences
                        int val;
                        std::vector<SDL_Rect> vec;
                        istringstream s(elem);
                        while( !(s >> val).fail() )
                        {
                            SDL_Rect rect = frames[val];
                            vec.push_back(rect);
                        }
                        m_hSequences[strSequenceName] = vec;
                    }
                } );
            }
            #ifdef DEBUG
            DebugPrint();
            #endif
        }
        else
        {
            throw runtime_error( "Couldn't open animation file." );
        }

    }

    SDL_Rect& CAnimation::GetAnimationRect( const string& name, Uint32 frame )
    {
        auto& seq = m_hSequences[name];
        return( seq[frame%seq.size()] );
    }

    size_t CAnimation::GetAnimationFrames( const string& name )
    {
        return( m_hSequences[name].size() );
    }

    bool CAnimation::IsFinished()
    {
        return (m_iFrame == m_hSequence.size()-1);
    }

    void CAnimation::DebugPrint()
    {
        #ifdef DEBUG
        for ( auto seq : m_hSequences ) {
            cout << "Sequence: " << seq.first << endl;
            for ( auto rect : seq.second ) {
                cout << "  Blit frame: " << rect.x << "," << rect.y << " (" << rect.w << "x" << rect.h << ")" << endl;
            }
        }
        #endif
    }

}

