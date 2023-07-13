
// \file

#ifndef Y_Random_Shuffle_Included
#define Y_Random_Shuffle_Included 1

#include "y/random/bits.hpp"
#include "y/memory/out-of-reach.hpp"

namespace Yttrium
{
    namespace Random
    {
        //______________________________________________________________________
        //
        //
        //
        //! Shuffling data with Bits
        //
        //
        //______________________________________________________________________
        struct Shuffle
        {

            //__________________________________________________________________
            //
            //! Exchange wrapper
            //__________________________________________________________________
            template <typename T> static inline
            void Exchange(T &lhs, T &rhs) noexcept {
                if(&lhs!=&rhs) Memory::OutOfReach::Swap(&lhs,&rhs,sizeof(T));
            }

            //__________________________________________________________________
            //
            //! Fisher-Yates
            //__________________________________________________________________
            template <typename ITERATOR>
            static void Range(ITERATOR curr, const size_t n, Bits &ran)
            {
                for(size_t i=n-1;i>0;--i)
                    Exchange(*(curr+i), *(curr+ran.leq(i)));
            }

            //__________________________________________________________________
            //
            //! shuffling list
            //__________________________________________________________________
            template <typename LIST> static inline
            void List(LIST &L, Bits &ran) noexcept
            {
                typename LIST::SelfType temp;
                while(L.size>0)
                    temp.pushTail( L.pop( L.fetch( ran.index(L.size) ) ) );
                temp.swapWith(L);
            }

            //__________________________________________________________________
            //
            //! shuffling pool
            //__________________________________________________________________
            template <typename POOL> static inline
            void Pool(POOL &P, Bits &ran) noexcept
            {
                typename POOL::SelfType temp;
                while(P.size>0)
                    temp.store( P.query( ran.index(P.size)) );
                temp.swapWith(P);
            }


        };


    }

}

#endif

