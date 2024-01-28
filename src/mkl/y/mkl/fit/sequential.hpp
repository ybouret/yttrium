
//! \file

#ifndef Y_Fit_Sequential_Included
#define Y_Fit_Sequential_Included 1

#include "y/mkl/fit/variables.hpp"
#include "y/container/readable.hpp"
#include "y/memory/solitary/workspace.hpp"

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

                inline ORDINATE set(const ABSCISSA           &abs0,
                                    const Readable<ABSCISSA> &aorg,
                                    const Variables          &vars)
                {
                    const ORDINATE ord0 = init( prevAbscissa.build(abs0), aorg, vars);
                    return prevOrdinate.build(ord0);
                }

                inline ORDINATE run(const ABSCISSA           &abs1,
                                    const Readable<ABSCISSA> &aorg,
                                    const Variables          &vars)
                {
                    const ABSCISSA &abs0 = *prevAbscissa;
                    const ORDINATE &ord0 = *prevOrdinate;
                    const ORDINATE  ord1 = move(abs0,ord0,abs1,aorg,vars);
                    (void) prevAbscissa.build(abs1);
                    return prevOrdinate.build(ord1);
                }

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline virtual ~Sequential() noexcept  {} //!< cleanup
            protected:
                inline explicit Sequential()  {} //!< setup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Sequential);
                Memory::Workspace<ABSCISSA> prevAbscissa;
                Memory::Workspace<ORDINATE> prevOrdinate;

                virtual ORDINATE init(const ABSCISSA           &abs0,
                                      const Readable<ABSCISSA> &aorg,
                                      const Variables          &vars) = 0;

                virtual ORDINATE move(const ABSCISSA           &abs0,
                                      const ORDINATE           &ord0,
                                      const ABSCISSA           &abs1,
                                      const Readable<ABSCISSA> &aorg,
                                      const Variables          &vars) = 0;

            };
        }
    }

}

#endif

