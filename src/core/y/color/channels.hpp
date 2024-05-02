//! \file

#ifndef Y_Color_Channels_Included
#define Y_Color_Channels_Included 1

#include "y/color/rgba.hpp"
#include <cmath>

namespace Yttrium
{
    namespace Color
    {

        //! Real to pixel, todo
        template <typename TARGET, typename REAL>
        struct RealToPixel
        {
            //! real to real
            static inline TARGET Convert(const REAL f) noexcept { return static_cast<TARGET>(f); }
        };

        //! Real to pixel, todo
        template <typename REAL>
        struct RealToPixel<uint8_t,REAL>
        {
            //! real to closest byte
            static inline uint8_t Convert(const REAL f) noexcept {
                static const REAL half(0.5);
                return static_cast<uint8_t>(floor(f+half));
            }
        };


        //______________________________________________________________________
        //
        //
        //
        //! Single channel to floating-point operations
        //
        //
        //______________________________________________________________________
        template <typename T>
        struct Channels
        {
            //__________________________________________________________________
            //
            //! 1 pixel = 1 channel
            //__________________________________________________________________
            static const unsigned NCH = 1;

            //__________________________________________________________________
            //
            //! set to zero
            //__________________________________________________________________
            template <typename U> static inline
            void Ldz(U * const channel) noexcept
            {
                assert(0!=channel);
                channel[0] = 0;
            }

            //__________________________________________________________________
            //
            //! add weighted value
            //__________________________________________________________________
            template <typename U> static inline
            void Add(U * const channel, const U &cof, const T &arg) noexcept
            {
                assert(0!=channel);
                channel[0] += cof * static_cast<U>(arg);
            }

            //__________________________________________________________________
            //
            //! divide by denominator
            //__________________________________________________________________
            template <typename U> static inline
            void Div(U * const channel, const U &den) noexcept
            {
                assert(0!=channel);
                channel[0] /= den;
            }

            //__________________________________________________________________
            //
            //! convert target to closest real
            //__________________________________________________________________
            template <typename U> static inline
            void Get(T &target, const U * const channel) noexcept
            {
                assert(0!=channel);
                target = RealToPixel<T,U>::Convert( channel[0] );
            }


        };


        //______________________________________________________________________
        //
        //
        //
        //! Multiple channels to floating-points operations
        //
        //
        //______________________________________________________________________
        template <template <typename> class COLOR, typename T>
        struct Channels< COLOR<T> >
        {
            //__________________________________________________________________
            //
            //! 1 pixel = NCH channels
            //__________________________________________________________________
            static const unsigned NCH = COLOR<T>::NumChannels;

            //__________________________________________________________________
            //
            //! set all channels to zero
            //__________________________________________________________________
            template <typename U> static inline
            void Ldz(U * const channel) noexcept
            {
                assert(0!=channel);
                for(unsigned i=0;i<NCH;++i)
                    channel[i] = 0;
            }

            //__________________________________________________________________
            //
            //! add weighted components
            //__________________________________________________________________
            template <typename U> static inline
            void Add(U * const channel, const U &cof, const COLOR<T> &arg) noexcept
            {
                assert(0!=channel);
                const T * const c = (const T *)&arg;
                for(unsigned i=0;i<NCH;++i)
                    channel[i] += cof * static_cast<U>(c[i]);
            }

            //__________________________________________________________________
            //
            //! divide all channels by denominator
            //__________________________________________________________________
            template <typename U> static inline
            void Div(U * const channel, const U &den) noexcept
            {
                assert(0!=channel);
                for(unsigned i=0;i<NCH;++i)
                    channel[i] /= den;
            }

            //__________________________________________________________________
            //
            //! retrieve closest pixel of concerned channel
            /**
             \remark target should copy the orinal pixel before operations
             */
            //__________________________________________________________________
            template <typename U> static inline
            void Get(COLOR<T> &target, const U * const channel) noexcept
            {
                assert(0!=channel);
                T * const tgt = (T *)&target;
                for(unsigned i=0;i<NCH;++i) tgt[i] = RealToPixel<T,U>::Convert( channel[i] );
            }

        };

    }

}

#endif
