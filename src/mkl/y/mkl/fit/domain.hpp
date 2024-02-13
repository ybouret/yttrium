
//! \file

#ifndef Y_Fit_Domain_Included
#define Y_Fit_Domain_Included 1

#include "y/mkl/intervals.hpp"
#include "y/mkl/fit/memory-model.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {

            template <typename ABSCISSA>
            class Domain : public Intervals<ABSCISSA,MemoryModel>
            {
            public:
                typedef Intervals<ABSCISSA,MemoryModel> IntervalsType;

                inline explicit Domain(const size_t dims) : IntervalsType(dims) {}
                inline virtual ~Domain() noexcept {}
                inline Domain(const Domain &other) : IntervalsType(other) {}

            private:
                Y_DISABLE_ASSIGN(Domain);
            };

        }

    }

}

#endif
