//! \file

#ifndef Y_Fit_Sample_Interface_Included
#define Y_Fit_Sample_Interface_Included 1

#include "y/mkl/fit/sample/abscissae.hpp"

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
            //! Sample description
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class Sample : public SampleAbscissae<ABSCISSA>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef SampleAbscissae<ABSCISSA>       MyAbscissae; //!< alias

                typedef typename MyAbscissae::Abscissa  Abscissa;    //!< alias
                typedef ORDINATE                        Ordinate;    //!< alias

                typedef typename MyAbscissae::Abscissae Abscissae;   //!< alias
                typedef Readable<ORDINATE>              Ordinates;   //!< alias

                


                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________

                //! access ordinates
                virtual const Ordinates & ordinates() const noexcept = 0;

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                virtual ~Sample() noexcept {}
            protected:
                explicit Sample() noexcept : MyAbscissae() {}

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Sample);
            };

        }

    }
}

#endif

