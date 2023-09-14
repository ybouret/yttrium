//! \file

#ifndef Y_WOVEn_Indices_Included
#define Y_WOVEn_Indices_Included 1

#include "y/ordered/vector.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        //______________________________________________________________________
        //
        //
        //
        //! Set of indices to keep track of used vector
        //
        //
        //______________________________________________________________________
        class Indices : public OrderedVector<size_t,IncreasingComparator,Memory::Pooled>
        {
        public:
            typedef Yttrium::OrderedVector<size_t,IncreasingComparator,Memory::Pooled> BaseType; //!< alias

            explicit Indices(const size_t n); //!< setup
            virtual ~Indices() noexcept;      //!< cleanup
            Indices(const Indices &);         //!< copy

            //! record all indices corresponding to not-zero values of source
            template <typename SOURCE> inline
            void record(SOURCE &source) {
                Indices &self = *this;
                for(size_t j=source.size();j>0;--j)
                { if(0!=source[j]) self |= j; }
            }


        private:
            Y_DISABLE_ASSIGN(Indices);
        };

    }
}

#endif

