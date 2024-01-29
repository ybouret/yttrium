
//! \file

#ifndef Y_Fit_Samples_Included
#define Y_Fit_Samples_Included 1

#include "y/mkl/fit/sample/interface.hpp"
#include "y/associative/suffix/set.hpp"

namespace Yttrium
{
    namespace MKL
    {

        namespace Fit
        {

            template <typename ABSCISSA, typename ORDINATE>
            class Samples : public SuffixSet<String, typename Sample<ABSCISSA,ORDINATE>::Pointer>
            {
            public:
                typedef Sample<ABSCISSA,ORDINATE>       SampleType;
                typedef typename SampleType::Pointer    SamplePointer;
                typedef SuffixSet<String,SamplePointer> SampleDB;

                explicit Samples() {}
                virtual ~Samples() noexcept {}
                

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Samples);
            };
        }

    }

}

#endif

