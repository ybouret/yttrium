
//! \file

#ifndef Y_Fit_Sample_Abscissae_Included
#define Y_Fit_Sample_Abscissae_Included 1

#include "y/mkl/fit/sample/metrics.hpp"
#include "y/sort/indexing.hpp"

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
            //! Abscissae access and internal memory for Sample
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA>
            class SampleAbscissae : public SampleMetrics
            {
            public:
                //______________________________________________________________
                //
                //
                // Definitions
                //
                //______________________________________________________________
                typedef ABSCISSA           Abscissa;  //!< alias
                typedef Readable<Abscissa> Abscissae; //!< alias

                //______________________________________________________________
                //
                //
                // Interface
                //
                //______________________________________________________________

                //! access to abscissae
                virtual const Abscissae & abscissae() const noexcept = 0;

                //______________________________________________________________
                //
                //
                // Method
                //
                //______________________________________________________________
               
                //! build indices and clean memory
                inline void prepare()
                {
                    assert( dimension() == abscissae().size() );
                    const size_t n = dimension();
                    indx.adjust(n,0);
                    Indexing::Make(indx,Compare,abscissae());
                }

                //! inline comparator
                static inline int Compare(const Abscissa &lhs, const Abscissa &rhs) noexcept {
                    return lhs < rhs ? -1 : ( rhs < lhs ? 1 : 0);
                }

                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                inline virtual ~SampleAbscissae() noexcept {}
            protected:
                template <typename ID>
                inline explicit SampleAbscissae(const ID &id) noexcept : SampleMetrics(id) {}



            private:
                Y_DISABLE_COPY_AND_ASSIGN(SampleAbscissae);

            };

        }

    }
}

#endif

