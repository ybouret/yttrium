
//! \file

#ifndef Y_Fit_Sample_Metrics_Included
#define Y_Fit_Sample_Metrics_Included 1

#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/counted.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {

            //__________________________________________________________________
            //
            //! internal memory alocator
            //__________________________________________________________________
            typedef Memory::Dyadic SampleMemory;

            //__________________________________________________________________
            //
            //
            //
            //! base class for Sample
            //
            //
            //__________________________________________________________________
            class SampleMetrics : public Object, public Counted
            {
            public:
                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
               
                //! number of data point
                virtual size_t dimension() const noexcept = 0;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~SampleMetrics() noexcept;
            protected:
                explicit SampleMetrics() noexcept;

            public:
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Vector<size_t,SampleMemory> indx; //!< index of data abscissae

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleMetrics);
            };

        }

    }
}

#endif
