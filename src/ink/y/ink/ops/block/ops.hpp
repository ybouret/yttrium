
//! \file

#ifndef Y_Ink_Block_Ops_Included
#define Y_Ink_Block_Ops_Included 1

#include "y/ink/ops/block/average.hpp"
#include "y/sort/nw.hpp"
#include "y/type/utils.hpp"

namespace Yttrium
{
    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //
        //! Operations on blocks for single channel
        //
        //
        //______________________________________________________________________
        template <typename T>
        struct BlockOps
        {
            //__________________________________________________________________
            //
            //! out = min(arr[N])
            //__________________________________________________________________
            template <size_t N>
            static void Minimum(T &out, const T * const arr)
            {
                assert(0!=arr);
                T res = arr[0];
                for(size_t i=1;i<N;++i) res = Min(res,arr[i]);
                out = res;
            }

            //__________________________________________________________________
            //
            //! out = max(arr[N])
            //__________________________________________________________________
            template <size_t N>
            static void Maximum(T &out, const T * const arr)
            {
                assert(0!=arr);
                T res = arr[0];
                for(size_t i=1;i<N;++i) res = Max(res,arr[i]);
                out = res;
            }

            //__________________________________________________________________
            //
            //! out = median(arr[N])
            //__________________________________________________________________
            template <size_t N>
            static void Median(T &out, T * const arr)
            {
                T * const tableau = arr-1;
                NetworkSort::Algo<N>::Increasing(tableau);
                out = arr[N>>1];
            }

            //__________________________________________________________________
            //
            //! out = average(arr[N])
            //__________________________________________________________________
            template <size_t N>
            static void Average(T &out, const T * const arr)
            {
                BlockAverage<T>:: template Compute<N>(out,arr);
            }
        };

        //______________________________________________________________________
        //
        //
        //
        //! Operations on blocks for multiple channels
        //
        //
        //______________________________________________________________________
        template <template <typename> class COLOR, typename T>
        struct BlockOps< COLOR<T> >
        {
            typedef COLOR<T>            ColorType;              //!< alias
            static const unsigned NCH = ColorType::NumChannels; //!< alias

            //__________________________________________________________________
            //
            //! convert array of colors to arrays of color
            //__________________________________________________________________
            template <size_t N> static void Load(T chan[NCH][N], const ColorType * const arr ) noexcept
            {
                for(size_t i=0;i<N;++i)
                {
                    const ColorType C = arr[i];
                    const T * const c = (const T *) &C;
                    for(size_t j=0;j<NCH;++j)
                    {
                        chan[j][i] = c[j];
                    }
                }
            }

            //__________________________________________________________________
            //
            //! Minimum per channel
            //__________________________________________________________________
            template <size_t N>
            static void Minimum(ColorType &out, const ColorType * const arr)
            {
                assert(0!=arr);
                T chan[NCH][N];
                Load<N>(chan,arr);
                T * const c = (T*)&out;
                for(size_t j=0;j<NCH;++j)
                    BlockOps<T>::template Minimum<N>(c[j], chan[j]);
            }

            //__________________________________________________________________
            //
            //! Maximum per channel
            //__________________________________________________________________
            template <size_t N>
            static void Maximum(ColorType &out, const ColorType * const arr)
            {
                assert(0!=arr);
                T chan[NCH][N];
                Load<N>(chan,arr);
                T * const c = (T*)&out;
                for(size_t j=0;j<NCH;++j)
                    BlockOps<T>::template Maximum<N>(c[j], chan[j]);
            }

            //__________________________________________________________________
            //
            //! Median per channel
            //__________________________________________________________________
            template <size_t N>
            static void Median(ColorType &out, const ColorType * const arr)
            {
                assert(0!=arr);
                T chan[NCH][N];
                Load<N>(chan,arr);
                T * const c = (T*)&out;
                for(size_t j=0;j<NCH;++j)
                    BlockOps<T>::template Median<N>(c[j], chan[j]);
            }

            //__________________________________________________________________
            //
            //! Average per channel
            //__________________________________________________________________
            template <size_t N>
            static void Average(ColorType &out, const ColorType * const arr)
            {
                assert(0!=arr);
                T chan[NCH][N];
                Load<N>(chan,arr);
                T * const c = (T*)&out;
                for(size_t j=0;j<NCH;++j)
                    BlockOps<T>::template Average<N>(c[j], chan[j]);
            }

        };

    }

}

#endif

