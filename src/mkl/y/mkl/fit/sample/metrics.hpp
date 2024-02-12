
//! \file

#ifndef Y_Fit_Sample_Metrics_Included
#define Y_Fit_Sample_Metrics_Included 1

#include "y/sequence/vector.hpp"
#include "y/memory/allocator/dyadic.hpp"
#include "y/mkl/fit/variables.hpp"

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
                virtual size_t numPoints() const noexcept = 0;

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                const String & key() const noexcept; //!< name

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~SampleMetrics() noexcept;      //!< cleanup
            protected:
                explicit SampleMetrics(const String &); //!< setup
                explicit SampleMetrics(const char   *); //!< setup

            public:
                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                const String                name; //!< identifier
                Vector<size_t,SampleMemory> indx; //!< index of data abscissae
                Variables                   vars; //!< fit variables
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleMetrics);
            };

        }

    }
}

#endif
