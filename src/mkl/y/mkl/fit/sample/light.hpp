//! \file

#ifndef Y_Fit_Sample_Light_Included
#define Y_Fit_Sample_Light_Included 1

#include "y/mkl/fit/sample/interface.hpp"
#include "y/container/light-array.hpp"

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
            //! Light Sample, based on persistent user's data
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class LightSample : public Sample<ABSCISSA,ORDINATE>
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef Sample<ABSCISSA,ORDINATE>      SampleType; //!< alias
                typedef typename SampleType::Abscissae Abscissae;  //!< alias
                typedef typename SampleType::Ordinates Ordinates;  //!< alias

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________

                //! setup with name and persistent data
                template <typename ID>
                inline explicit LightSample(const ID       &       id,
                                            const ABSCISSA * const XX,
                                            const ORDINATE * const YY,
                                            const size_t           NN) :
                SampleType(id),
                _num( NN ),
                _abs( (ABSCISSA *)XX, NN),
                _ord( (ORDINATE *)YY, NN)
                {
                }

                //! cleanup
                inline virtual ~LightSample() noexcept {}

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________
                inline virtual size_t    dimension() const noexcept { return _num; }
                inline const Abscissae & abscissae() const noexcept { return _abs; }
                inline const Ordinates & ordinates() const noexcept { return _ord; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LightSample);
                const size_t               _num;
                const LightArray<ABSCISSA> _abs;
                const LightArray<ORDINATE> _ord;
            };

        }

    }

}

#endif

