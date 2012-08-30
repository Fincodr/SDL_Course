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

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <stdexcept>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "Singleton.hpp"
#include "ResourceFactory.hpp"
#include "TextUtils.hpp"

namespace DemoEngine {

    // Import only what we need
    using std::runtime_error;
    using std::vector;
    using std::cout;
    using std::endl;
    using std::ifstream;
    using std::string;
    using std::istringstream;
    using std::ostringstream;
    using std::unordered_map;
    using std::istream_iterator;
    using std::ostream_iterator;
    using std::copy;
    using std::find;
    using std::back_inserter;

    static const string kFramesText = "frames";
    static const string kSequencesText = "sequences";

    class CAnimation
    {
        public:
            CAnimation();
            virtual ~CAnimation();
            virtual inline void SetFPS( float fFPS ) { SetFrameTime( 1.0/fFPS ); }      // Sets animation time in frames per second
            virtual inline void SetFrameTime( float fTime ) { m_fFrameTime = fTime; }   // Sets display time for a single frame in seconds
            virtual inline float GetFrameTime() const { return m_fFrameTime; }          // Returns frame time in seconds
            virtual inline void SetFrame( int frame ) { m_iFrame = frame; }             // Sets current frame
            virtual inline bool HasAnimation() const { return !m_hSequences.empty(); }
            virtual inline const SDL_Rect & GetFrameRectangle() { return m_hSequence[m_iFrame]; }
            virtual void SetAnimation( string anim_name, bool bLoop = false );
            virtual void Update( float fPassedTime );
            virtual void LoadAnimation( const char *szFilename ) throw( runtime_error );
            virtual bool IsFinished();
            virtual void DebugPrint();
            virtual void ApplyAnimation() {};
            SDL_Rect& GetAnimationRect( const string& name, Uint32 frame );
            size_t GetAnimationFrames( const string& name );
        protected:
            typedef unordered_map<string,vector<SDL_Rect>> SequenceFrames_t;
            SequenceFrames_t            m_hSequences;    // Assosiative hashtable for storing sequences and frames
            std::vector<SDL_Rect>       m_hSequence;
            size_t m_iFrame = 0;        // Current frame in current sequence
            bool m_bLooping = false;    // Is animation looping
            float m_fFrameTime = 0.0f;  // What is display time for a single frame
            float m_fCumTime = 0.0f;    // Cumulative passed time
        private:
    };

    typedef CSingleton<CResourceFactory<int, CAnimation>> AnimationFactory;

}

#endif // ANIMATION_HPP
