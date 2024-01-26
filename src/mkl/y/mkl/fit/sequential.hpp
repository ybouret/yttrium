
//! \file

#ifndef Y_Fit_Sequential_Included
#define Y_Fit_Sequential_Included 1

#include "y/config/starting.hpp"

namespace Yttrium
{
    namespace MKL
    {

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

            virtual ORDINATE start(const ABSCISSA &) = 0;
            

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

#endif

