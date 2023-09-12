//! \file

#ifndef Y_WOVEn_Indices_Included
#define Y_WOVEn_Indices_Included 1

#include "y/ordered/vector.hpp"

namespace Yttrium
{
    namespace WOVEn
    {

        class Indices : public OrderedVector<size_t,IncreasingComparator,Memory::Pooled>
        {
        public:
            typedef OrderedVector<size_t,IncreasingComparator,Memory::Pooled> BaseType;

            explicit Indices(const size_t n);
            virtual ~Indices() noexcept;
            Indices(const Indices &);
            
        private:
            Y_DISABLE_ASSIGN(Indices);
        };

    }
}

#endif

