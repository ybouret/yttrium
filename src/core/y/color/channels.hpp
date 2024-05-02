//! \file

#ifndef Y_Color_Channels_Included
#define Y_Color_Channels_Included 1

#include "y/color/scalar-conv.hpp"
#include "y/color/rgba.hpp"

namespace Yttrium
{
    namespace Color
    {



        template <typename T>
        struct Channels
        {
            static const unsigned NCH = 1;

            template <typename U> static inline
            void Ldz(U * const channel, const T &) noexcept
            {
                assert(0!=channel);
                channel[0] = 0;
            }

            template <typename U> static inline
            void Add(U * const channel, const U &cof, const T &arg) noexcept
            {
                assert(0!=channel);
                channel[0] += cof * static_cast<U>(arg);
            }

            template <typename U> static inline
            void Div(U * const channel, const U &den) noexcept
            {
                assert(0!=channel);
                channel[0] /= den;
            }

            template <typename U> static inline
            void Get(T &target, const U * const channel) noexcept
            {
                assert(0!=channel);
                target = ScalarConv<T,U>::From( channel[0] );
            }


        };

        template <template <typename> class COLOR, typename T>
        struct Channels< COLOR<T> >
        {
            static const unsigned NCH = COLOR<T>::NumChannels;
            static const unsigned ACH = COLOR<T>::AllChannels;

            template <typename U> static inline
            void Ldz(U * const channel, const COLOR<T> &C) noexcept
            {
                assert(0!=channel);
                for(unsigned i=0;i<NCH;++i)
                    channel[i] = 0;
                const T * const c = (const T *)&C;
                for(unsigned i=NCH;i<ACH;++i) channel[i] = c[i];
            }

            template <typename U> static inline
            void Add(U * const channel, const U &cof, const COLOR<T> &arg) noexcept
            {
                assert(0!=channel);
                const T * const c = (const T *)&arg;
                for(unsigned i=0;i<NCH;++i)
                    channel[i] += cof * static_cast<U>(c[i]);
            }

            template <typename U> static inline
            void Div(U * const channel, const U &den) noexcept
            {
                assert(0!=channel);
                for(unsigned i=0;i<NCH;++i)
                    channel[i] /= den;
            }

            template <typename U> static inline
            void Get(COLOR<T> &target, const U * const channel) noexcept
            {
                assert(0!=channel);
                T * const tgt = (T *)&target;
                for(unsigned i=0;i<ACH;++i) tgt[i] = ScalarConv<T,U>::From( channel[i] );
            }

        };

    }

}

#endif
