//! \file

#ifndef Y_Ink_Block_Average_Included
#define Y_Ink_Block_Average_Included 1

#include "y/type/traits.hpp"

namespace Yttrium
{
    namespace Ink
    {
        //______________________________________________________________________
        //
        //
        //
        //! taking average of an array with integral or floating point type
        //
        //
        //______________________________________________________________________
        template <typename T>
        struct BlockAverage
        {

        private:
            //__________________________________________________________________
            //
            //! assuming integral type
            //__________________________________________________________________
            template <size_t N> static inline
            void Compute( const Int2Type<true> &, T &out, const T * const arr)
            {
                uint64_t sum = arr[0];
                for(size_t i=1;i<N;++i) sum += arr[i];
                out = static_cast<T>(sum/N);
            }

            //__________________________________________________________________
            //
            //! assuming floating point type
            //__________________________________________________________________
            template <size_t N> static inline
            void Compute( const Int2Type<false> &, T &out, const T * const arr)
            {
                T       sum = arr[0];
                for(size_t i=1;i<N;++i) sum += arr[i];
                out = sum/T(N);
            }

        public:
            //__________________________________________________________________
            //
            //! call proper function, out = <arr[N]>
            //__________________________________________________________________
            template <size_t N> static inline
            void Compute(T &out, const T * const arr)
            {
                static const Int2Type<TypeTraits<T>::IsIntegral> Choice;
                Compute<N>(Choice,out,arr);
            }
        };
    }

}

#endif

