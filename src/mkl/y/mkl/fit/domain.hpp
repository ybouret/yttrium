
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
            //__________________________________________________________________
            //
            //
            //
            //! Intervals of admissible ABSCISSA
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA>
            class Domain : public Intervals<ABSCISSA,MemoryModel>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Intervals<ABSCISSA,MemoryModel> IntervalsType; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline explicit Domain(const size_t dims) : IntervalsType(dims) {} //!< setup
                inline virtual ~Domain() noexcept {}                               //!< cleanup
                inline Domain(const Domain &other) : IntervalsType(other) {}       //!< copy

            private:
                Y_DISABLE_ASSIGN(Domain);
            };

        }

    }

}

#endif
