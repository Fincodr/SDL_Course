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

#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <algorithm>

namespace DemoEngine {

    namespace Math
    {

        const double kPI = 3.1415926535897932384626433832795;

        namespace Limits
        {

            template<typename T>
            inline T clampmax(T x, T a)
            {
                return x < a ? x : a;
            }

            template<typename T>
            inline T clampmin(T x, T a)
            {
                return x > a ? x : a;
            }

            template<typename T>
            inline T clamp(T x, T a, T b)
            {
                return x < a ? a : (x > b ? b : x);
            }
        }

        namespace Coordinates
        {
            inline float GetAngleForLine(float x1, float y1, float x2, float y2)
            {
                return ( atan2(x2 - x1, y2 - y1) * 180 / kPI + 180 );
            }
            inline float GetDistance(float x1, float y1, float x2, float y2)
            {
                float vx = x2-x1;
                float vy = y2-y1;
                return ( sqrt(vx*vx + vy*vy) );
            }
        }

        namespace Colors
        {

            typedef struct HSL_COLOR {
                unsigned int h;
                double s;
                double l;
            } HSL;

            typedef struct HSV_COLOR {
                unsigned int h;
                double s;
                double v;
            } HSV;

            typedef struct COLOUR_COLOR {
                double r;
                double g;
                double b;
            } COLOUR;

            // http://local.wasp.uwa.edu.au/~pbourke/texture_colour/convert/
            //
            // C++ and SDL conversion by <Fincodr@mxl.fi>

            /*
               Calculate HSL from RGB
               Hue is in degrees
               Lightness is between 0 and 1
               Saturation is between 0 and 1
            */

            inline HSL RGB2HSL(COLOUR c1)
            {
               double themin,themax,delta;
               HSL c2;

               themin = std::min(c1.r,std::min(c1.g,c1.b));
               themax = std::max(c1.r,std::max(c1.g,c1.b));
               delta = themax - themin;
               c2.l = (themin + themax) / 2;
               c2.s = 0;
               if (c2.l > 0 && c2.l < 1)
                  c2.s = delta / (c2.l < 0.5 ? (2*c2.l) : (2-2*c2.l));
               c2.h = 0;
               if (delta > 0) {
                  if (themax == c1.r && themax != c1.g)
                     c2.h += (c1.g - c1.b) / delta;
                  if (themax == c1.g && themax != c1.b)
                     c2.h += (2 + (c1.b - c1.r) / delta);
                  if (themax == c1.b && themax != c1.r)
                     c2.h += (4 + (c1.r - c1.g) / delta);
                  c2.h *= 60;
               }
               return(c2);
            }

            /*
               Calculate RGB from HSL, reverse of RGB2HSL()
               Hue is in degrees
               Lightness is between 0 and 1
               Saturation is between 0 and 1
            */
            inline COLOUR HSL2RGB(HSL c1)
            {
               COLOUR c2,sat,ctmp;

               while (c1.h > 360)
                  c1.h -= 360;

               if (c1.h < 120) {
                  sat.r = (120 - c1.h) / 60.0;
                  sat.g = c1.h / 60.0;
                  sat.b = 0;
               } else if (c1.h < 240) {
                  sat.r = 0;
                  sat.g = (240 - c1.h) / 60.0;
                  sat.b = (c1.h - 120) / 60.0;
               } else {
                  sat.r = (c1.h - 240) / 60.0;
                  sat.g = 0;
                  sat.b = (360 - c1.h) / 60.0;
               }
               sat.r = std::min(sat.r,(double)1);
               sat.g = std::min(sat.g,(double)1);
               sat.b = std::min(sat.b,(double)1);

               ctmp.r = 2 * c1.s * sat.r + (1 - c1.s);
               ctmp.g = 2 * c1.s * sat.g + (1 - c1.s);
               ctmp.b = 2 * c1.s * sat.b + (1 - c1.s);

               if (c1.l < 0.5) {
                  c2.r = c1.l * ctmp.r;
                  c2.g = c1.l * ctmp.g;
                  c2.b = c1.l * ctmp.b;
               } else {
                  c2.r = (1 - c1.l) * ctmp.r + 2 * c1.l - 1;
                  c2.g = (1 - c1.l) * ctmp.g + 2 * c1.l - 1;
                  c2.b = (1 - c1.l) * ctmp.b + 2 * c1.l - 1;
               }

               return(c2);
            }

            // from koders.com
            inline COLOUR HSV2RGB(HSV hsv) {
               /*
                * Purpose:
                * Convert HSV values to RGB values
                * All values are in the range [0.0 .. 1.0]
                * Except hue is in degrees
                */
                COLOUR rgb;
                float S, H, V, F, M, N, K;
                int   I;

                S = hsv.s;  /* Saturation */
                H = (float)hsv.h/360;  /* Hue */
                V = hsv.v;  /* value or brightness */

                if ( hsv.s == 0 ) {
                    /*
                    * Achromatic case, set level of grey
                    */
                    rgb.r = V;
                    rgb.g = V;
                    rgb.b = V;
                } else {
                    /*
                    * Determine levels of primary colours.
                    */
                    if (H >= 1.0) {
                        H = 0.0;
                    } else {
                        H = H * 6;
                    }
                    I = (int) H;   /* should be in the range 0..5 */
                    F = H - I;     /* fractional part */

                    M = V * (1 - S);
                    N = V * (1 - S * F);
                    K = V * (1 - S * (1 - F));

                    if (I == 0) { rgb.r = V; rgb.g = K; rgb.b = M; }
                    if (I == 1) { rgb.r = N; rgb.g = V; rgb.b = M; }
                    if (I == 2) { rgb.r = M; rgb.g = V; rgb.b = K; }
                    if (I == 3) { rgb.r = M; rgb.g = N; rgb.b = V; }
                    if (I == 4) { rgb.r = K; rgb.g = M; rgb.b = V; }
                    if (I == 5) { rgb.r = V; rgb.g = M; rgb.b = N; }
                }
                return rgb;
            }

            inline SDL_Color HSL2SDLColor(HSL c1)
            {
                COLOUR c = HSL2RGB(c1);
                return SDL_Color({ static_cast<Uint8>(c.r*255), static_cast<Uint8>(c.g*255), static_cast<Uint8>(c.b*255), static_cast<Uint8>(255) });
            }

            inline SDL_Color HSV2SDLColor(HSV c1)
            {
                COLOUR c = HSV2RGB(c1);
                return SDL_Color({ static_cast<Uint8>(c.r*255), static_cast<Uint8>(c.g*255), static_cast<Uint8>(c.b*255), static_cast<Uint8>(255) });
            }

        }

        namespace Interpolation
        {
            typedef std::function<double(double,double,double,double)> func_t;

            // Math from: http://gizma.com/easing/
            //
            // time       = current time
            // duration   = the final time value (duration of the interpolating, default is 1.0)
            // startValue = start value
            // deltaValue = change in value (= end-start)
            //
            // If you do not have C++11 compiler
            // remove templated Sin, Cos, Sqrt and Pow2 functions and
            // replace every word "Sin->sin", "Cos->cos", "Sqrt"->"sqrt" and "Pow2"->"pow(2, "
            // to remove the math cache optimizations.
            //
            // C++ conversion, template addition and Math cache optimizations by <Fincodr@mxl.fi>

            namespace Cache {

                template<typename valueType = double, typename inputType = double>
                inline const valueType Sin(inputType a) {
                    static std::unordered_map<inputType,valueType> SinCache;
                    auto it = SinCache.find(a);
                    if ( it != SinCache.end() ) {
                        return it.second;
                    }
                    else
                    {
                        return ( static_cast<valueType>(SinCache[a] = sin(a)) );
                    }
                }

                template<typename valueType = double, typename inputType = double>
                inline const valueType Cos(inputType a) {
                    static std::unordered_map<inputType,valueType> CosCache;
                    auto it = CosCache.find(a);
                    if ( it != CosCache.end() ) {
                        return it->second;
                    }
                    else
                    {
                        return ( static_cast<valueType>(CosCache[a] = cos(a)) );
                    }
                }

                template<typename valueType = double, typename inputType = double>
                inline const valueType Sqrt(inputType a) {
                    static std::unordered_map<inputType,valueType> SqrtCache;
                    auto it = SqrtCache.find(a);
                    if ( it != SqrtCache.end() ) {
                        return it->second;
                    }
                    else
                    {
                        return ( static_cast<valueType>(SqrtCache[a] = sqrt(a)) );
                    }
                }

                template<typename valueType = double, typename inputType = double>
                inline const valueType Pow2(inputType a) {
                    static std::unordered_map<inputType,valueType> PowCache;
                    auto it = PowCache.find(a);
                    if ( it != PowCache.end() ) {
                        return it->second;
                    }
                    else
                    {
                        return ( static_cast<valueType>(PowCache[a] = pow(2, a)) );
                    }
                }

            }

            // simple linear tweening - no easing, no acceleration
            template<typename positionType = double, typename valueType = double>
            inline double linearTween(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                return (deltaValue*(valueType)time/(valueType)duration + startValue);
            }

            // quadratic easing in - accelerating from zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInQuad(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration;
                return deltaValue*time*time + startValue;
            }

            // quadratic easing out - decelerating to zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeOutQuad(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration;
                return -deltaValue * time*(time-2) + startValue;
            }

            // quadratic easing in/out - acceleration until halfway, then deceleration
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInOutQuad(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration/2;
                if (time < 1) return deltaValue/2*time*time + startValue;
                time--;
                return -deltaValue/2 * (time*(time-2) - 1) + startValue;
            }

            // cubic easing in - accelerating from zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInCubic(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration;
                return deltaValue*time*time*time + startValue;
            }

            // cubic easing out - decelerating to zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeOutCubic(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration;
                time--;
                return deltaValue*(time*time*time + 1) + startValue;
            }

            // cubic easing in/out - acceleration until halfway, then deceleration
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInOutCubic(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration/2;
                if (time < 1) return deltaValue/2*time*time*time + startValue;
                time -= 2;
                return deltaValue/2*(time*time*time + 2) + startValue;
            }

            // quartic easing in - accelerating from zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInQuart(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration;
                return deltaValue*time*time*time*time + startValue;
            }

            // quartic easing out - decelerating to zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeOutQuart(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration;
                time--;
                return -deltaValue * (time*time*time*time - 1) + startValue;
            }

            // quartic easing in/out - acceleration until halfway, then deceleration
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInOutQuart(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration/2;
                if (time < 1) return deltaValue/2*time*time*time*time + startValue;
                time -= 2;
                return -deltaValue/2 * (time*time*time*time - 2) + startValue;
            }

            // quintic easing in - accelerating from zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInQuint(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration;
                return deltaValue*time*time*time*time*time + startValue;
            }

            // quintic easing out - decelerating to zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeOutQuint(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration;
                time--;
                return deltaValue*(time*time*time*time*time + 1) + startValue;
            }

            // quintic easing in/out - acceleration until halfway, then deceleration
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInOutQuint(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration/2;
                if (time < 1) return deltaValue/2*time*time*time*time*time + startValue;
                time -= 2;
                return deltaValue/2*(time*time*time*time*time + 2) + startValue;
            }

            // sinusoidal easing in - accelerating from zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInSine(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                return -deltaValue * Cache::Cos(time/duration * (kPI/2)) + deltaValue + startValue;
            }

            // sinusoidal easing out - decelerating to zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeOutSine(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                return deltaValue * Cache::Sin(time/duration * (kPI/2)) + startValue;
            }

            // sinusoidal easing in/out - accelerating until halfway, then decelerating
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInOutSine(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                return -deltaValue/2 * (Cache::Cos(kPI*time/duration) - 1) + startValue;
            }

            // exponential easing in - accelerating from zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInExpo(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                return deltaValue * Cache::Pow2( 10 * (time/duration - 1) ) + startValue;
            }

            // exponential easing out - decelerating to zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeOutExpo(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                return deltaValue * ( -Cache::Pow2( -10 * time/duration ) + 1 ) + startValue;
            }

            // exponential easing in/out - accelerating until halfway, then decelerating
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInOutExpo(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration/2;
                if (time < 1) return deltaValue/2 * Cache::Pow2( 10 * (time - 1) ) + startValue;
                time--;
                return deltaValue/2 * ( -Cache::Pow2( -10 * time) + 2 ) + startValue;
            }

            // circular easing in - accelerating from zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInCirc(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration;
                return -deltaValue * (Cache::Sqrt(1 - time*time) - 1) + startValue;
            }

            // circular easing out - decelerating to zero velocity
            template<typename positionType = double, typename valueType = double>
            inline valueType easeOutCirc(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration;
                time--;
                return deltaValue * Cache::Sqrt(1 - time*time) + startValue;
            }

            // circular easing in/out - acceleration until halfway, then deceleration
            template<typename positionType = double, typename valueType = double>
            inline valueType easeInOutCirc(positionType time, valueType startValue, valueType deltaValue, positionType duration = (positionType)1) {
                if ( time < 0 ) time = 0;
                if ( time > duration ) time = duration;
                time /= duration/2;
                if (time < 1) return -deltaValue/2 * (Cache::Sqrt(1 - time*time) - 1) + startValue;
                time -= 2;
                return deltaValue/2 * (Cache::Sqrt(1 - time*time) + 1) + startValue;
            }

        }

    }

}


#endif // MATH_HPP
