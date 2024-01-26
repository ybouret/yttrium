
//! \file

#ifndef Y_Fit_Sequential_Included
#define Y_Fit_Sequential_Included 1

#include "y/mkl/fit/variables.hpp"
#include "y/container/readable.hpp"

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
            //! Sequential Function
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA,typename ORDINATE>
            class Sequential
            {
            public:

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________

                virtual ORDINATE start(const ABSCISSA           &,
                                       const Readable<ABSCISSA> &aorg) = 0;


                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline virtual ~Sequential() noexcept {} //!< cleanup
            protected:
                inline explicit Sequential() noexcept {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Sequential);
            };
        }
    }

}

#endif

