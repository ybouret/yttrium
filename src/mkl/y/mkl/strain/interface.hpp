
//! \file

#ifndef Y_MKL_Strain_Interface_Included
#define Y_MKL_Strain_Interface_Included 1

#include "y/container/recyclable.hpp"

namespace Yttrium
{
    namespace MKL
    {
        namespace Strain
        {
            //__________________________________________________________________
            //
            //
            //! named coefficients
            //
            //__________________________________________________________________
            enum Evaluated
            {
                ConstantTerm    =  1, //!< Interface[ConstantTerm]
                FirstDerivative  = 2, //!< Interface[FirstDerivative]
                SecondDerivative = 3  //!< Interface[SecondDerivative]
            };

            //__________________________________________________________________
            //
            //
            //
            //! base class for filters
            //
            //
            //__________________________________________________________________
            class Interface : public Recyclable
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                static  const size_t SIZE = SecondDerivative; //!< number of computed coefficients

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                virtual size_t load() const noexcept = 0; //!< current load

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~Interface() noexcept; //!< cleanup
            protected:
                explicit Interface() noexcept; //!< setup
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Interface);
            };


        }

    }

}

#endif
