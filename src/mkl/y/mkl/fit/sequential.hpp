
//! \file

#ifndef Y_Fit_Sequential_Included
#define Y_Fit_Sequential_Included 1

#include "y/mkl/fit/type-for.hpp"
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
                typedef TypeFor<ABSCISSA,ORDINATE>     MyTypeFor;
                typedef typename MyTypeFor::Parameters Parameters;

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________

                //! initialize call at abscissa0
                inline ORDINATE set(const ABSCISSA           &abs0,
                                    const Parameters         &aorg,
                                    const Variables          &vars)
                {
                    std::cerr << "Sequential init@" << abs0 << std::endl;
                    const ORDINATE ord0 = init( prevAbscissa.build(abs0), aorg, vars);
                    return prevOrdinate.build(ord0);
                }

                //! run from previous coordinates to abscissa1
                inline ORDINATE run(const ABSCISSA           &abs1,
                                    const Parameters         &aorg,
                                    const Variables          &vars)
                {
                    const ABSCISSA &abs0 = *prevAbscissa;
                    const ORDINATE &ord0 = *prevOrdinate;
                    std::cerr << "Sequential move@" << abs0 << " -> " << abs1 << std::endl;

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
                                      const Parameters         &aorg,
                                      const Variables          &vars) = 0;

                virtual ORDINATE move(const ABSCISSA           &abs0,
                                      const ORDINATE           &ord0,
                                      const ABSCISSA           &abs1,
                                      const Parameters         &aorg,
                                      const Variables          &vars) = 0;

            };
        }
    }

}

#endif

