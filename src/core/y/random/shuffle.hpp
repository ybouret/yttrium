
// \file

#ifndef Y_Random_Shuffle_Included
#define Y_Random_Shuffle_Included 1

#include "y/random/bits.hpp"

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

            //! generic call on base[0..size*itemize-1]
            static void Tableau(void        *base,
                                const size_t size,
                                const size_t itemSize,
                                Bits        &ran) noexcept;


            //! generic call on Tableau[0..num-1]
            template <typename T> static inline
            void Tableau(T           *arr,
                         const size_t num,
                         Bits        &ran) noexcept
            {
                assert(Good(arr,num));
                Tableau(arr,num,sizeof(T),ran);
            }

            //! shuffling list
            template <typename LIST> static inline
            void List(LIST &L, Bits &ran) noexcept
            {
                typename LIST::SelfType temp;
                while(L.size>0)
                {
                    temp.pushTail( L.pop( L.fetch( ran.lt(L.size) ) ) );
                }
                temp.swapWith(L);
            }

            //! shuffling pool
            template <typename POOL> static inline
            void Pool(POOL &P, Bits &ran) noexcept
            {
                typename POOL::SelfType temp;
                while(P.size>0)
                {
                    if( ran.choice() )
                        temp.store( P.query() );
                    else
                        temp.stash( P.query() );
                }
                temp.swapWith(P);
            }


        };


    }

}

#endif

