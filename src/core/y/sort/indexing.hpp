//! \file

#ifndef Y_Indexing_Included
#define Y_Indexing_Included 1

#include "y/container/writable.hpp"
#include "y/container/light-array.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{

    //__________________________________________________________________________
    //
    //
    //
    //! Indexing Data
    //
    //
    //__________________________________________________________________________
    struct Indexing
    {
        //______________________________________________________________________
        //
        //
        //! make index using 'int compare(lhs,rhs)'
        //
        //______________________________________________________________________
        template <typename T,typename FUNC> static inline
        void Make(Writable<size_t>  &idx,
                  FUNC              &compare,
                  const Readable<T> &arr) noexcept
        {

            //------------------------------------------------------------------
            // initialize indices
            //------------------------------------------------------------------
            const size_t n = idx.size(); assert( arr.size() == idx.size() );
            for(size_t i=n;i>0;--i) idx[i] = i;

            //------------------------------------------------------------------
            // sort routine
            //------------------------------------------------------------------
            size_t   inc = 1;
            do
            {
                inc *=3;
                ++inc;
            }
            while( inc <= n );
            do
            {
                inc /= 3;
                for( size_t i=inc+1;i<=n;++i)
                {
                    const size_t   t = idx[i];
                    const T       &v = arr[t];
                    size_t         j = i;
                    size_t         d = j-inc;
                    assert( t      <= n );
                    assert( idx[d] <= n );
                    while( compare( v, arr[ idx[d] ] ) < 0 )
                    {
                        idx[j] = idx[d];
                        j = d;
                        if( j <= inc )
                            break;
                        d -= inc;
                    }
                    idx[ j ] = t;
                }
            }
            while( inc > 1 );
        }

        //______________________________________________________________________
        //
        //
        //! indexing C-style tableau
        //
        //______________________________________________________________________
        template <typename T, typename FUNC> static inline
        void Tableau(size_t * const  indx,
                     const size_t    size,
                     FUNC &          comp,
                     const T * const data)
        {
            assert(Good(indx,size));
            assert(Good(data,size));
            {
                LightArray<size_t>   idx(indx,size);
                const LightArray<T>  arr( (typename TypeTraits<T>::MutableType *)data, size);
                Make(idx,comp,arr);
            }
            for(size_t i=0;i<size;++i) --indx[i];
        }


        //______________________________________________________________________
        //
        //
        //! ranking
        /**
         \param arr array of items to be ranked
         \param idx indices
         \param wksp flat bytes, sizeof(wksp)>=n*sizeof(T)
         */
        //______________________________________________________________________
        template <typename T> static inline
        void Rank(Writable<T> &arr, const Readable<size_t> &idx, void * const wksp)
        {
            typedef typename TypeTraits<T>::MutableType MType;
            assert( arr.size() == idx.size() );
            const size_t  n = arr.size(); if(n<=1) return;
            MType * const w = static_cast<MType *>(wksp)-1;
            for(size_t i=n;i>0;--i)
            {
                Memory::OutOfReach::Move( &w[i],   &arr[i], sizeof(T) );
                Memory::OutOfReach::Zero( &arr[i], sizeof(T));
            }
            for(size_t i=n;i>0;--i)
            {
                const size_t j = idx[i];
                Memory::OutOfReach::Move( &arr[i], &w[j], sizeof(T));
                Memory::OutOfReach::Zero( &w[j], sizeof(T));
            }
        }

    };

}

#endif

