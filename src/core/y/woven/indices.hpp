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
            typedef OrderedVector<size_t,IncreasingComparator,Memory::Pooled> BaseType; //!< alias

            explicit Indices(const size_t n); //!< setup
            virtual ~Indices() noexcept;      //!< cleanup
            Indices(const Indices &);         //!< copy
            
        private:
            Y_DISABLE_ASSIGN(Indices);
        };

    }
}

#endif

