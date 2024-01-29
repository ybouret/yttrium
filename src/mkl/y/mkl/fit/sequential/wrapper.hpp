
//! \file

#ifndef Y_Fit_Sequential_Wrapper_Included
#define Y_Fit_Sequential_Wrapper_Included 1

#include "y/mkl/fit/sequential.hpp"

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
            //! convert an out-of-order function into a sequential function
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class SequentialWrapper : public Sequential<ABSCISSA,ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef TypeFor<ABSCISSA,ORDINATE>           MyType;         //!< alias
                typedef Sequential<ABSCISSA,ORDINATE>        SequentialFunc; //!< alias
                typedef typename MyType::OutOfOrderFunc      OutOfOrderFunc; //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup
                inline explicit SequentialWrapper(const OutOfOrderFunc &h) : SequentialFunc(), func(h) {}

                //! cleanup
                inline virtual ~SequentialWrapper() noexcept {}

                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                OutOfOrderFunc func; //!< copy of the function

            private:
                Y_DISABLE_COPY_AND_ASSIGN(SequentialWrapper);

                virtual ORDINATE init(const ABSCISSA           &abs0,
                                      const Readable<ABSCISSA> &aorg,
                                      const Variables          &vars)
                {
                    return func(abs0,aorg,vars);
                }

                virtual ORDINATE move(const ABSCISSA           &abs0,
                                      const ORDINATE           &ord0,
                                      const ABSCISSA           &abs1,
                                      const Readable<ABSCISSA> &aorg,
                                      const Variables          &vars)
                {
                    (void)abs0;
                    (void)ord0;
                    return func(abs1,aorg,vars);
                }

            };

        }

    }

}

#endif

