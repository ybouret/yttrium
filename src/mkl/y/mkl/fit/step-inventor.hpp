//! \file

#ifndef Y_Fit_Step_Inventor_Included
#define Y_Fit_Step_Inventor_Included 1

#include "y/mkl/fit/least-squares.hpp"
#include "y/mkl/fit/domain.hpp"
#include "y/mkl/algebra/lu.hpp"
#include "y/mkl/tao/seq/level1.hpp"

namespace Yttrium
{

    namespace MKL
    {

        namespace Fit
        {


            //! helper to show process
#define Y_MKL_FIT(MSG) do { if(verbose) std::cerr << MSG << std::endl; } while(false)

            //! helper to degrade step/return false on overflow
#define Y_MKL_FIT_DEGRADE() do {            \
/**/ if(++p>pmax) { p=pmax; return false; } \
} while(false)

            //__________________________________________________________________
            //
            //
            //
            //! Findind Out step according to a full metrics
            //
            //
            //__________________________________________________________________
            template <typename ABSCISSA>
            class StepInventor : public Oversized
            {
            public:
                //______________________________________________________________
                //
                //
                // C++
                //
                //______________________________________________________________
                explicit StepInventor();          //!< setup
                virtual ~StepInventor() noexcept; //!< cleanup

                //______________________________________________________________
                //
                //
                // Methods
                //
                //______________________________________________________________
                void     prepare(const size_t nvar); //!< prepare memory
                ABSCISSA xFactor(const int p) const; //!< compute factor with pmin<=p<=pmax

                
            private:
                bool buildCurvature(const Matrix<ABSCISSA> &alpha,
                                    const int               param,
                                    const Booleans         &used);


            public:
                //______________________________________________________________
                //
                //! compute constrained, approximated step
                /**
                 \param lsqf    computed LeastSquares
                 \param aorg    fit parameters
                 \param adom    domains of definition
                 \param p       correcting parameter, possibly upgraded
                 \param used    used parameters
                 \param verbose verbosity
                 \return false if singular matrix or always out of bound step
                 */
                //______________________________________________________________
                bool buildStep(const LeastSquaresCom<ABSCISSA> & lsqf,
                               const Readable<ABSCISSA> &        aorg,
                               const Domain<ABSCISSA>   &        adom,
                               int                      &        p,
                               const Booleans           &        used,
                               const bool                        verbose);


                //______________________________________________________________
                //
                //
                // Members
                //
                //______________________________________________________________
                Matrix<ABSCISSA>              curv; //!< approx curvature
                Matrix<ABSCISSA>              hess; //!< approx hessian
                Vector<ABSCISSA,MemoryModel>  step; //!< approx step
                Vector<ABSCISSA,MemoryModel>  atry; //!< trial parameters
                Vector<ABSCISSA,MemoryModel>  atmp; //!< probe if necessary
                LU<ABSCISSA>                  lu;   //!< linear algebra
                const int                     pmin; //!< -int(DIG)-1  => lambda=1
                const int                     pmax; //!< MAX_10_EXP   => overflow

            private:
                const ABSCISSA                zero;
                const ABSCISSA                one;
                const ABSCISSA                two;
                const ABSCISSA                ten;
                const ABSCISSA                tenth;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(StepInventor);
            };

        }

    }

}

#endif
