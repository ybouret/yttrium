
//! \file

#ifndef Y_Fit_Sample_Heavy_Included
#define Y_Fit_Sample_Heavy_Included 1

#include "y/mkl/fit/sample/interface.hpp"

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
            //! Heavy Sample
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA, typename ORDINATE>
            class HeavySample : public Sample<ABSCISSA,ORDINATE>
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
                template <typename ID>
                inline explicit HeavySample(const ID     &id,
                                            const size_t nmin = 0) :
                SampleType(id),
                _num(0),
                _abs(nmin,AsCapacity),
                _ord(nmin,AsCapacity)
                {

                }
                //! cleanup
                inline virtual ~HeavySample() noexcept {}

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                inline virtual size_t    dimension() const noexcept { return _num; }
                inline const Abscissae & abscissae() const noexcept { return _abs; }
                inline const Ordinates & ordinates() const noexcept { return _ord; }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(HeavySample);
                size_t                        _num;
                Vector<ABSCISSA,SampleMemory> _abs;
                Vector<ORDINATE,SampleMemory> _ord;
            };
        }
    }

}

#endif

