
//! \file

#ifndef Y_Fit_Least_Squares_Common_Included
#define Y_Fit_Least_Squares_Common_Included 1

#include "y/mkl/fit/samples.hpp"
#include "y/mkl/fit/sequential.hpp"
#include "y/mkl/antelope/caddy.hpp"
#include "y/container/matrix.hpp"
#include "y/oversized.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {

            template <typename ABSCISSA>
            class LeastSquaresCom : public Oversized
            {
            public:
                typedef          Antelope::Add<ABSCISSA>       XAdd;           //!< alias
                typedef          Antelope::Caddy<ABSCISSA>     Caddy;          //!< alias
                typedef          typename Caddy::XNode         XNode;          //!< alias

                XAdd                          xadd; //!< to perform additions
                Caddy                         xlst; //!< to perform additions on beta
                Vector<ABSCISSA,MemoryModel>  beta; //!< gradient of D2
                Matrix<ABSCISSA>              curv; //!< approx curvature of D2
                const size_t                  npts; //!< last dimensions
                const ABSCISSA                zero; //!< alias
                const ABSCISSA                half; //!< alias
                const ABSCISSA                one;  //!< alias
                                                    //!
                virtual ~LeastSquaresCom() noexcept;
            protected:
                explicit LeastSquaresCom();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(LeastSquaresCom);
            };

        }

    }

}

#endif
